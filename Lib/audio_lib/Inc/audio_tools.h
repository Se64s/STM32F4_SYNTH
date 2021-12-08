/**
 * @file    audio_tools.h
 * @author  Sebastián Del Moral Gallardo.
 * @brief   Misc tools available in all audio module.
 * @version 0.1
 * @date    2021-10-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_TOOLS_H__
#define __AUDIO_TOOLS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Lineal mapping values.
 * 
 * @param x input value to map.
 * @param in_min input min value.
 * @param in_max input max value.
 * @param out_min output max value.
 * @param out_max output max value
 * @return float value scaled.
 */
float AUDIO_TOOL_lin_map(float x, float in_min, float in_max, float out_min, float out_max);

#ifdef __cplusplus
}
#endif
#endif /*__AUDIO_TOOLS_H__ */

/* EOF */
