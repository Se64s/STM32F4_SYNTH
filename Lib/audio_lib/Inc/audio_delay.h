/**
 * @file    audio_delay.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Module to generate delay effect.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_DELAY_H__
#define __AUDIO_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Delay control structure.
 * 
 */
typedef struct AudioDelayCtrl {
    float *fBuffer;
    uint32_t u32BufferSize;
    uint32_t u32WritePos;
    uint32_t u32ReadPos;
    uint32_t u32SampleRate;
    float fFeedback;
} AudioDelayCtrl_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init delay control structure.
 * 
 * @param pxDelayCtrl pointer of control structure to init.
 * @param u32SampleRate Sample rate used.
 * @param pfData pointer of buffer that will hold delay data.
 * @param u32BufferSize size of delay buffer.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_DELAY_init(AudioDelayCtrl_t *pxDelayCtrl, uint32_t u32SampleRate, float *pfData, uint32_t u32BufferSize);

/**
 * @brief Update the amount of delay in seconds to apply in delay structure.
 * 
 * @param pxDelayCtrl pointer of init delay structure.
 * @param fDelayTime time in seconds of delay to apply.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_DELAY_update_delay(AudioDelayCtrl_t *pxDelayCtrl, float fDelayTime);

/**
 * @brief Update feedback parameter of delay structure.
 * 
 * @param pxDelayCtrl pointer to initiated delay structure.
 * @param fFeedback new amount of feedback to apply. Value should be in range 0.0 to 0.99.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_DELAY_update_feedback(AudioDelayCtrl_t *pxDelayCtrl, float fFeedback);

/**
 * @brief Help function to get the maximun delay supported by the control structure.
 * 
 * @param pxDelayCtrl pointer to init delay structure.
 * @return float max delay allowed by the structure.
 */
float AUDIO_DELAY_get_max_delay(AudioDelayCtrl_t *pxDelayCtrl);

/**
 * @brief Process delay sample.
 * 
 * @param pxDelayCtrl pointer of control structure.
 * @param fInputSample input sample to process.
 * @return float output delayed sample.
 */
float AUDIO_DELAY_process(AudioDelayCtrl_t *pxDelayCtrl, float fInputSample);

#ifdef __cplusplus
}
#endif
#endif /* __AUDIO_DELAY_H__ */

/* EOF */
