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
#include "audio_hal.h"
#include "audio_tools.h"
#include "audio_wavetable.h"
#include "audio_filter.h"
#include "audio_delay.h"

#include "arm_math.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* System sample rate in Hz */
#define AUDIO_SAMPLE_RATE           ( 48000U )

/* Max value for dac */
#define AUDIO_FULL_AMPLITUDE        ( 32767.0F )
#define AUDIO_FULL_AMPLITUDE_P      ( 32766 )
#define AUDIO_FULL_AMPLITUDE_M      ( -32767 )
#define AUDIO_AMPLITUDE             ( AUDIO_FULL_AMPLITUDE / (float)AUDIO_VOICE_NUM )
#define AUDIO_AMPLITUDE_NONE        ( 0.0F )

/* Audio out sample buffer, 2 channels, 1 position for channel (16b) */
#define AUDIO_BUFF_DATA_WIDE        ( sizeof(uint16_t) )
#define AUDIO_BUFF_SIZE             ( 512U )
#define AUDIO_HALF_BUFF_SIZE        ( AUDIO_BUFF_SIZE / 2U )

/* Delay buffer parameters */
#define AUDIO_DELAY_BUFF_SIZE       ( 9600U ) // Max 0.2s delay
#define AUDIO_DELAY_FEDDBACK        ( 0.9F )
#define AUDIO_DELAY_TIME            ( 0.1F )

/* Audio buffer critical positions for DMA transfer */
#define AUDIO_BUFF_INIT_INDEX       ( 0U )
#define AUDIO_BUFF_HALF_INDEX       ( AUDIO_HALF_BUFF_SIZE )

/* Number of elements of 32b in the buffer */
#define AUDIO_TRANSFER_SIZE         ( sizeof(u16AudioBuffer) / AUDIO_BUFF_DATA_WIDE )

/* Max midi note */
#define MAX_MIDI_NOTE               ( 0x7F )

/* Amplitude mapping*/
#define MAX_AMP_DB_MAP              ( 0.0F )
#define MIN_AMP_DB_MAP              ( -40.0F )

/* Filter default values */
#define FILTER_LP_RES_FREQ          ( 50000.0F )
#define FILTER_LP_RES_Q             ( 1.0F )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* System voice list */
AudioWaveTableVoice_t xVoiceList[AUDIO_VOICE_NUM];

/* Audio buffer */
uint16_t u16AudioBuffer[AUDIO_BUFF_SIZE] = { 0U };

/* Filter */
AudioFilter2ndOrder_t xFilter2ndOrder = { 0U };

/* Delay */
float fDelayBuffer[AUDIO_DELAY_BUFF_SIZE];
AudioDelayCtrl_t xDelayCtrl = { 0U };

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Callback to handle i2s events.
 * 
 * @param event sys_i2s event.
 */
static void audio_hal_cb(audio_hal_event_t event);

/**
 * @brief Prepare data in audio buffer.
 * 
 * @param pu16Buffer Output audio buffer.
 * @param u16StartIndex Start of the index where put new data.
 */
static void audio_update_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex);

/* Private function definition -----------------------------------------------*/

static void audio_hal_cb(audio_hal_event_t event)
{
    switch ( event )
    {
        case AUDIO_HAL_EVENT_TX_DONE:
            audio_update_buffer(u16AudioBuffer, AUDIO_BUFF_HALF_INDEX);
            break;

        case AUDIO_HAL_EVENT_HALF_TX_DONE:
            audio_update_buffer(u16AudioBuffer, AUDIO_BUFF_INIT_INDEX);
            break;

        case AUDIO_HAL_EVENT_ERROR:
            /* Reset interface */
            AUDIO_HAL_send_buffer(u16AudioBuffer, AUDIO_TRANSFER_SIZE);
            break;

        default:
            break;
    }
}

static void audio_update_buffer(uint16_t *pu16Buffer, uint16_t u16StartIndex)
{
    AUDIO_HAL_gpio_ctrl(true);

    for (uint32_t i = 0; i < AUDIO_HALF_BUFF_SIZE; i += 2U)
    {
        float fData = 0.0F;

        // Agregate data from all voices
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            // Agregate all voice data
            fData += AUDIO_WAVE_get_next_sample(&xVoiceList[u32Voice]);
        }

        // Add delay action
        fData = AUDIO_DELAY_process(&xDelayCtrl, fData);

        // Add filter action
        fData = AUDIO_FILTER_2nd_order_render(&xFilter2ndOrder, fData);

        // Check DAC boundaries to prevent signal rollback
        int16_t i16DacData = (int16_t)fData;
        if ( i16DacData > AUDIO_FULL_AMPLITUDE_P )
        {
            i16DacData = AUDIO_FULL_AMPLITUDE_P;
        }
        else if ( i16DacData < AUDIO_FULL_AMPLITUDE_M )
        {
            i16DacData = AUDIO_FULL_AMPLITUDE_M;
        }

        // Channel L
        pu16Buffer[u16StartIndex++] = i16DacData;
        // Channel R
        pu16Buffer[u16StartIndex++] = i16DacData;
    }

    AUDIO_HAL_gpio_ctrl(false);
}

/* Public function prototypes ------------------------------------------------*/

audio_ret_t AUDIO_init(void)
{
    audio_ret_t eRetval = AUDIO_WAVE_ERR;

    /* Init filter section */
    AUDIO_FILTER_compute_coefficients_LP_RES(&xFilter2ndOrder, AUDIO_SAMPLE_RATE, FILTER_LP_RES_FREQ, FILTER_LP_RES_Q);

    /* Init delay section */
    AUDIO_DELAY_init(&xDelayCtrl, AUDIO_SAMPLE_RATE, fDelayBuffer, AUDIO_DELAY_BUFF_SIZE);
    AUDIO_DELAY_update_delay(&xDelayCtrl, AUDIO_DELAY_TIME);
    AUDIO_DELAY_update_feedback(&xDelayCtrl, AUDIO_DELAY_FEDDBACK);

    /* Set all voices with known values */
    for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
    {
        AUDIO_WAVE_init_voice(&xVoiceList[u32Voice], AUDIO_SAMPLE_RATE, AUDIO_AMPLITUDE);
    }

    if ( AUDIO_HAL_init(audio_hal_cb) == AUDIO_WAVE_OK )
    {
        /* Start transfer */
        if ( AUDIO_HAL_send_buffer(u16AudioBuffer, AUDIO_TRANSFER_SIZE) == AUDIO_WAVE_OK )
        {
            eRetval = AUDIO_WAVE_OK;
        }
    }

    return eRetval;
}

audio_ret_t AUDIO_deinit(void)
{
    /* Set all voices with known values */
    for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
    {
        AUDIO_WAVE_init_voice(&xVoiceList[u32Voice], AUDIO_SAMPLE_RATE, AUDIO_AMPLITUDE_NONE);
    }

    AUDIO_HAL_deinit();

    return AUDIO_WAVE_OK;
}

audio_ret_t AUDIO_voice_set_state(audio_voice_id_t eVoice, bool bState)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);

    AUDIO_HAL_isr_ctrl(false);

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            AUDIO_WAVE_set_active(&xVoiceList[u32Voice], bState);
        }
    }
    else
    {
        (void)AUDIO_WAVE_set_active(&xVoiceList[eVoice], bState);
    }

    AUDIO_HAL_isr_ctrl(true);

    return AUDIO_WAVE_OK;
}

audio_ret_t AUDIO_voice_set_freq(audio_voice_id_t eVoice, float fFreq)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);

    AUDIO_HAL_isr_ctrl(false);

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)AUDIO_WAVE_update_freq(&xVoiceList[u32Voice], fFreq);
        }
    }
    else
    {
        (void)AUDIO_WAVE_update_freq(&xVoiceList[eVoice], fFreq);
    }

    AUDIO_HAL_isr_ctrl(true);

    return AUDIO_WAVE_OK;
}

audio_ret_t AUDIO_voice_set_midi_note(audio_voice_id_t eVoice, uint8_t u8MidiNote, uint8_t u8MidiVel)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);
    ERR_ASSERT(u8MidiNote <= MAX_MIDI_NOTE);
    ERR_ASSERT(u8MidiVel <= MAX_MIDI_NOTE);

    AUDIO_HAL_isr_ctrl(false);

    float fFreq = 440.0F * powf(2.0F, ((float)u8MidiNote - 69.0F) / 12.0F);
    float fdB = AUDIO_TOOL_lin_map((int)u8MidiVel, 1.0F, 127.0F, MIN_AMP_DB_MAP, MAX_AMP_DB_MAP);
    float fAmp = powf(10.0F, fdB / 20.0F);
    // float fAmp = 1.0F;

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)AUDIO_WAVE_update_freq(&xVoiceList[u32Voice], fFreq);
            (void)AUDIO_WAVE_update_amp(&xVoiceList[u32Voice], fAmp);
        }
    }
    else
    {
        (void)AUDIO_WAVE_update_freq(&xVoiceList[eVoice], fFreq);
        (void)AUDIO_WAVE_update_amp(&xVoiceList[eVoice], fAmp);
    }

    AUDIO_HAL_isr_ctrl(true);

    return AUDIO_WAVE_OK;
}

audio_ret_t AUDIO_voice_set_waveform(audio_voice_id_t eVoice, audio_wave_id_t eWaveId)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);
    ERR_ASSERT(eWaveId < AUDIO_WAVE_NUM);

    AUDIO_HAL_isr_ctrl(false);

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)AUDIO_WAVE_change_wave(&xVoiceList[u32Voice], eWaveId);
        }
    }
    else
    {
        (void)AUDIO_WAVE_change_wave(&xVoiceList[eVoice], eWaveId);
    }

    AUDIO_HAL_isr_ctrl(true);

    return AUDIO_WAVE_OK;
}

audio_ret_t AUDIO_voice_update(audio_voice_id_t eVoice, audio_wave_id_t eWaveId, float fFreq, bool bState)
{
    ERR_ASSERT(eVoice <= AUDIO_VOICE_NUM);
    ERR_ASSERT(eWaveId < AUDIO_WAVE_NUM);

    AUDIO_HAL_isr_ctrl(false);

    if (eVoice == AUDIO_VOICE_NUM)
    {
        for (uint32_t u32Voice = 0; u32Voice < (uint32_t)AUDIO_VOICE_NUM; u32Voice++)
        {
            (void)AUDIO_WAVE_update_freq(&xVoiceList[u32Voice], fFreq);
            (void)AUDIO_WAVE_change_wave(&xVoiceList[u32Voice], eWaveId);
            (void)AUDIO_WAVE_set_active(&xVoiceList[u32Voice], bState);
        }
    }
    else
    {
        (void)AUDIO_WAVE_update_freq(&xVoiceList[eVoice], fFreq);
        (void)AUDIO_WAVE_change_wave(&xVoiceList[eVoice], eWaveId);
        (void)AUDIO_WAVE_set_active(&xVoiceList[eVoice], bState);
    }

    AUDIO_HAL_isr_ctrl(true);

    return AUDIO_WAVE_OK;
}

/* EOF */