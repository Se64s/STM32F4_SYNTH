/**
 * @file    test_task.c
 * @author  Sebastián Del Moral
 * @brief   Playground task.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "test_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "sys_log.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TEST_IF     ( SYS_LOG_IF_0 )
#define TEST_LVL    ( SYS_LOG_LEVEL_DBG )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Definitions for testTask */
osThreadId_t testTaskHandle;
const osThreadAttr_t testTask_attributes = {
    .name = TEST_TASK_NAME,
    .stack_size = TEST_TASK_STACK,
    .priority = (osPriority_t)TEST_TASK_PRIORITY,
};

/* Private function prototypes -----------------------------------------------*/

void testTask_main(void *argument);

/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
void testTask_main(void *argument)
{
    uint32_t u32Tick = 0;
    sys_log_print(TEST_IF, TEST_LVL, "Init Test task");

    /* Infinite loop */
    for(;;)
    {
        sys_log_print(TEST_IF, TEST_LVL, "Test Tick: %d",u32Tick++);
        osDelay(1000U);
    }
}

/* Public function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void testTask_Init(void)
{
    testTaskHandle = osThreadNew(testTask_main, NULL, &testTask_attributes);
}

/* EOF */
