/**
 * @file    audio_engine.c
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Engine to handle audio generation in external dac.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_engine.h"

#include "wavetable.h"

#include "sys_i2s.h"
#ifdef AUDIO_TRASNFER_TRACE
#include "sys_gpio.h"
#endif // AUDIO_TRASNFER_TRACE

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Disable codec isr while updating the buffer */
#define DISABLE_ISR

/* Sys HAL defines */
#define AUDIO_I2S                   ( SYS_I2S_0 )
#define AUDIO_TEST_PIN              ( SYS_GPIO_0 )

/* System sample rate in Hz */
#define AUDIO_SAMPLE_RATE           ( 192000U )

/* Max value for dac */
#define AUDIO_FULL_AMPLITUDE        ( 32767.0F )
#define AUDIO_AMPLITUDE             ( AUDIO_FULL_AMPLITUDE / (float)AUDIO_VOICE_NUM )
#define AUDIO_AMPLITUDE_NONE        ( 0.0F )

/* Audio out sample buffer, 2 channels, 1 position for channel (16b) */
#define AUDIO_BUFF_DATA_WIDE        ( sizeof(uint16_t) )
#define AUDIO_BUFF_SIZE             ( 512U )
#define AUDIO_HALF_BUFF_SIZE        ( AUDIO_BUFF_SIZE / 2U )

/* Audio buffer critical positions for DMA transfer */
#define AUDIO_BUFF_INIT_INDEX       ( 0U )
#define AUDIO_BUFF_HALF_INDEX       ( AUDIO_HALF_BUFF_SIZE )

/* Number of elements of 32b in the buffer */
#define AUDIO_TRANSFER_SIZE         ( sizeof(u16AudioBuffer) / AUDIO_BUFF_DATA_WIDE )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* System voice list */
waveTableVoice_t xVoiceList[AUDIO_VOICE_NUM];

/* Audio buffer */
uint16_t u16AudioBuffer[AUDIO_BUFF_SIZE] = { 0U };

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Callback to handle i2s events.
 * 
 * @param event sys_i2s event.
 */
static void audio_i2s_cb(sys_i2s_event_t event);

/**
 * @brief Prepare data in audio buffer.
 * 
 * @param pu16Buffer Output audio buffer.
 * @param u16StartIndex Start of the index where put new data.
 */
static void audio_update_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex);

/* Private function definition -----------------------------------------------*/

static void audio_i2s_cb(sys_i2s_event_t event)
{
    switch ( event )
    {
        case SYS_I2S_EVENT_TX_DONE:
            audio_update_buffer(u16AudioBuffer, AUDIO_BUFF_HALF_INDEX);
            break;

        case SYS_I2S_EVENT_HALF_TX_DONE:
            audio_update_buffer(u16AudioBuffer, AUDIO_BUFF_INIT_INDEX);
            break;

        case SYS_I2S_EVENT_ERROR:
            /* Reset interface */
            sys_i2s_send(AUDIO_I2S, u16AudioBuffer, AUDIO_TRANSFER_SIZE);
            break;

        default:
            break;
    }
}

static void audio_update_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex)
{
#ifdef AUDIO_TRASNFER_TRACE
    sys_gpio_set_level(AUDIO_TEST_PIN, SYS_GPIO_STATE_SET);
#endif // AUDIO_TRASNFER_TRACE

    for (uint32_t i = 0; i < AUDIO_HALF_BUFF_SIZE; i += 2U)
    {
        int16_t i16Data = 0;

        // Agregate data from all voices
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            i16Data += (int16_t)WAVE_get_next_sample(&xVoiceList[u32Voice]);
        }

        // Channel L
        pu16Buffer[u16StartIndex++] = (uint16_t)i16Data;
        // Channel R
        pu16Buffer[u16StartIndex++] = (uint16_t)i16Data;
    }

#ifdef AUDIO_TRASNFER_TRACE
    sys_gpio_set_level(AUDIO_TEST_PIN, SYS_GPIO_STATE_RESET);
#endif // AUDIO_TRASNFER_TRACE
}

/* Public function prototypes ------------------------------------------------*/

sys_state_t AUDIO_init(void)
{
    sys_state_t eRetval = SYS_ERROR;

    /* Set all voices with known values */
    for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
    {
        WAVE_init_voice(&xVoiceList[u32Voice], AUDIO_SAMPLE_RATE, AUDIO_AMPLITUDE);
    }

    if ( sys_i2s_init(AUDIO_I2S, audio_i2s_cb) == SYS_SUCCESS
#ifdef AUDIO_TRASNFER_TRACE
        && sys_gpio_init(AUDIO_TEST_PIN, SYS_GPIO_MODE_OUT) == SYS_SUCCESS
#endif // AUDIO_TRASNFER_TRACE
    )
    {
        /* Start transfer */
        if ( sys_i2s_send(AUDIO_I2S, u16AudioBuffer, AUDIO_TRANSFER_SIZE) == SYS_SUCCESS )
        {
            eRetval = SYS_SUCCESS;
        }
    }

    return eRetval;
}

sys_state_t AUDIO_deinit(void)
{
    /* Set all voices with known values */
    for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
    {
        WAVE_init_voice(&xVoiceList[u32Voice], AUDIO_SAMPLE_RATE, AUDIO_AMPLITUDE_NONE);
    }

    return sys_i2s_deinit(AUDIO_I2S);
}

sys_state_t AUDIO_voice_set_state(audio_voice_id_t eVoice, bool bState)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, false);
#endif // DISABLE_ISR

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            WAVE_set_active(&xVoiceList[u32Voice], bState);
        }
    }
    else
    {
        (void)WAVE_set_active(&xVoiceList[eVoice], bState);
    }

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, true);
#endif // DISABLE_ISR

    return SYS_SUCCESS;
}

sys_state_t AUDIO_voice_set_freq(audio_voice_id_t eVoice, float fFreq)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, false);
#endif // DISABLE_ISR

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)WAVE_update_freq(&xVoiceList[u32Voice], fFreq);
        }
    }
    else
    {
        (void)WAVE_update_freq(&xVoiceList[eVoice], fFreq);
    }

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, true);
#endif // DISABLE_ISR

    return SYS_SUCCESS;
}

sys_state_t AUDIO_voice_set_waveform(audio_voice_id_t eVoice, audio_wave_id_t eWaveId)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);
    ERR_ASSERT(eWaveId < AUDIO_WAVE_NUM);

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, false);
#endif // DISABLE_ISR

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)WAVE_change_wave(&xVoiceList[u32Voice], eWaveId);
        }
    }
    else
    {
        (void)WAVE_change_wave(&xVoiceList[eVoice], eWaveId);
    }

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, true);
#endif // DISABLE_ISR

    return SYS_SUCCESS;
}

sys_state_t AUDIO_voice_update(audio_voice_id_t eVoice, audio_wave_id_t eWaveId, float fFreq, bool bState)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);
    ERR_ASSERT(eWaveId < AUDIO_WAVE_NUM);

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, false);
#endif // DISABLE_ISR

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)WAVE_update_freq(&xVoiceList[u32Voice], fFreq);
            (void)WAVE_change_wave(&xVoiceList[u32Voice], eWaveId);
            (void)WAVE_set_active(&xVoiceList[u32Voice], bState);
        }
    }
    else
    {
        (void)WAVE_update_freq(&xVoiceList[eVoice], fFreq);
        (void)WAVE_change_wave(&xVoiceList[eVoice], eWaveId);
        (void)WAVE_set_active(&xVoiceList[eVoice], bState);
    }

#ifdef DISABLE_ISR
    sys_i2s_isr_ctrl(AUDIO_I2S, true);
#endif // DISABLE_ISR

    return SYS_SUCCESS;
}

/* EOF */