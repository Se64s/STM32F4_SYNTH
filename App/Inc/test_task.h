/**
 * @file    task_test.h
 * @author  Sebastián Del Moral
 * @brief   Playground task.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_TASK_
#define __TEST_TASK_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TEST_TASK_STACK     ( 128 * 4 )
#define TEST_TASK_NAME      ( "Test Task" )
#define TEST_TASK_PRIORITY  ( osPriorityNormal )

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Test task resource init.
  * @param  None
  * @retval None
  */
void testTask_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_TASK_ */

/* EOF */

