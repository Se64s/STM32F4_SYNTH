/**
 * @file    audio_test.h
 * @author  Sebastián Del Moral
 * @brief   Audio output handler.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_TASK_
#define __AUDIO_TASK_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AUDIO_TASK_STACK     ( 128 * 4 )
#define AUDIO_TASK_NAME      ( "Audio Task" )
#define AUDIO_TASK_PRIORITY  ( osPriorityNormal )

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Test task resource init.
  * @param  None
  * @retval None
  */
void AudioTask_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_TASK_ */

/* EOF */

