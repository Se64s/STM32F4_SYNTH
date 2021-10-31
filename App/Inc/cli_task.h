
/**
 * @file    cli_task.h
 * @author  Sebastián Del Moral
 * @brief   CLI task.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLI_TASK_
#define __CLI_TASK_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CLI_TASK_STACK     ( 128 * 4 )
#define CLI_TASK_NAME      ( "CLI Task" )
#define CLI_TASK_PRIORITY  ( osPriorityNormal )

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Test task resource init.
  * @param  None
  * @retval None
  */
void CliTask_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __CLI_TASK_ */

/* EOF */

