/**
 * @file    audio_effect.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Effect engine.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_EFFECT_H__
#define __AUDIO_EFFECT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/

/* Defined slots */
typedef enum {
    AUDIO_EFFECT_SLOT_0 = 0U,
    AUDIO_EFFECT_SLOT_1,
    AUDIO_EFFECT_SLOT_NUM,
} audio_effect_slot_t;

/* Effects available */
typedef enum {
    AUDIO_EFFECT_NONE = 0U,
    AUDIO_EFFECT_FILTER_LP,
    AUDIO_EFFECT_DELAY,
    AUDIO_EFFECT_NUM
} audio_effect_id_t;

/* All effect parameters */
typedef enum {
    AUDIO_EFFECT_PARAMETER_FILTER_FREQ = 0U,
    AUDIO_EFFECT_PARAMETER_FILTER_Q,
    AUDIO_EFFECT_PARAMETER_DELAY_TIME,
    AUDIO_EFFECT_PARAMETER_DELAY_FB,
    AUDIO_EFFECT_PARAMETER_NUM,
} audio_effect_parameter_id_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Init effect section
 * 
 * @param fSampleRate system sample rate in Hz.
 */
void AUDIO_EFFECT_init(float fSampleRate);

/**
 * @brief Set effect to effect slot.
 * 
 * @param eSlot effect slot to use.
 * @param eId Effect to assign to effect slot.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_EFFECT_set_slot(audio_effect_slot_t eSlot, audio_effect_id_t eId);

/**
 * @brief Enable effect slot.
 * 
 * @param eSlot effect slot to use.
 * @param bActive new activation state to assign to effect slot.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_EFFECT_activate_slot(audio_effect_slot_t eSlot, bool bActive);

/**
 * @brief Get current effect assigned to effect slot.
 * 
 * @param eSlot effect slot to retrieve.
 * @return audio_effect_id_t current effect assigned to slot eSlot.
 */
audio_effect_id_t AUDIO_EFFECT_get_slot(audio_effect_slot_t eSlot);

/**
 * @brief Get current activation state to a determine slot.
 * 
 * @param eSlot slot to check.
 * @return true effect slot active.
 * @return false effect slot deactivated.
 */
bool AUDIO_EFFECT_get_slot_act(audio_effect_slot_t eSlot);

/**
 * @brief Update effect parameter from parameter list.
 * 
 * @param eId parameter id
 * @param fNewValue new value to assign.
 * @return audio_ret_t operation result.
 */
audio_ret_t AUDIO_EFFECT_parameter_update(audio_effect_parameter_id_t eId, float fNewValue);

/**
 * @brief Get current value of effect parameter.
 * 
 * @param eId parameter id to check.
 * @return float parameter value.
 */
float AUDIO_EFFECT_parameter_get(audio_effect_parameter_id_t eId);

/**
 * @brief Render all activate effects from effect list.
 * 
 * @param fInputData input data to effect engine.
 * @return float output data after pass throught all effects.
 */
float AUDIO_EFFECT_render(float fInputData);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_EFFECT_H__ */

/* EOF */
