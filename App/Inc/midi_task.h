/**
 * @file    midi_task.h
 * @author  Sebastián Del Moral
 * @brief   Midi task handler.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDI_TASK_
#define __MIDI_TASK_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MIDI_TASK_STACK     ( 128 * 4 )
#define MIDI_TASK_NAME      ( "Midi Task" )
#define MIDI_TASK_PRIORITY  ( osPriorityNormal )

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Test task resource init.
  * @param  None
  * @retval None
  */
void MidiTask_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MIDI_TASK_ */

/* EOF */

