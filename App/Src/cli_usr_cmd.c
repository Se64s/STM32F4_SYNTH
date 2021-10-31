/**
 * @file    cli_usr_cmd.c
 * @author  Sebastián Del Moral Gallardo (sebmorgal@gmail.com).
 * @brief   CLI commands defined for shell library.
 * @version 0.1
 * @date    2021-07-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Private includes --------------------------------------------------------*/

#include "shell.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>

#include "sys_mcu.h"
#include "sys_log.h"

/* Private defines ---------------------------------------------------------*/

#define SHELL_RET_OK    ( 0 )
#define SHELL_RET_ERR   ( -1 )

#define SHELL_STR_OK    "OK \r\n"
#define SHELL_STR_ERR   "ERR\r\n"

/* Private typedef ---------------------------------------------------------*/
/* Private macro -----------------------------------------------------------*/

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/* Private functions prototypes --------------------------------------------*/

int cli_cmd_reset(int argc, char *argv[]);
int cli_cmd_logOn(int argc, char *argv[]);
int cli_cmd_logOff(int argc, char *argv[]);
int cli_cmd_setLogLvl(int argc, char *argv[]);

/* Private variable --------------------------------------------------------*/

/* List of commands implemented */
static const sShellCommand s_shell_commands[] = {
    { "logon", cli_cmd_logOn, "Enable global log" },
    { "logoff", cli_cmd_logOff, "Disable global log" },
    { "setloglvl", cli_cmd_setLogLvl, "Set new log level for defined interface. Interface [0-7], Level [0-3]" },
    { "reset", cli_cmd_reset, "Reset device" },
    { "help", shell_help_handler, "Lists all commands" },
};

const sShellCommand *const g_shell_commands = s_shell_commands;

/* Number of commands used */
const size_t g_num_shell_commands = ARRAY_SIZE(s_shell_commands);

/* Private functions definition --------------------------------------------*/

/**
 * @brief Force reset.
 * 
 * @param argc Number of arguments
 * @param argv List of arguments
 * @return int Status:  0, OK, !0, ERROR
 * 
 */
int cli_cmd_reset(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 1U )
    {
        iRetCode = SHELL_RET_ERR;
        shell_put_line(SHELL_STR_ERR);
    }
    else
    {
        shell_put_line(SHELL_STR_OK);
        sys_mcu_reset();
    }

    return iRetCode;
}

/**
 * @brief Enable log interface.
 * 
 * @param argc Number of arguments, 1
 * @param argv List of arguments, none
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_logOn(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 1U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        sys_log_set_global_log(true);
    }

    if ( iRetCode == SHELL_RET_OK )
    {
        shell_put_line(SHELL_STR_OK);
    }
    else
    {
        shell_put_line(SHELL_STR_ERR);
    }

    return iRetCode;
}

/**
 * @brief Enable log interface.
 * 
 * @param argc Number of arguments, 1
 * @param argv List of arguments, none
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_logOff(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 1U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        sys_log_set_global_log(false);
    }

    if ( iRetCode == SHELL_RET_OK )
    {
        shell_put_line(SHELL_STR_OK);
    }
    else
    {
        shell_put_line(SHELL_STR_ERR);
    }

    return iRetCode;
}

/**
 * @brief Enable log interface.
 * 
 * @param argc Number of arguments, 3.
 * @param argv List of arguments, log interface, new log level.
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_setLogLvl(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 3U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        /* Check interface */
        uint32_t u32If = (uint32_t)atoi(argv[1U]);

        if ( u32If < SYS_LOG_IF_NUM )
        {
            /* Check Level */
            uint32_t u32Lvl = (uint32_t)atoi(argv[2U]);

            if ( u32Lvl < SYS_LOG_LEVEL_NUM )
            {
                sys_log_set_output_level((sys_log_if_t)u32If, (sys_log_lvl_t)u32Lvl);
            }
            else
            {
                shell_put_line("Wrong Log Lvl!");
            }
        }
        else
        {
            shell_put_line("Wrong interface Id!");
        }
    }

    if ( iRetCode == SHELL_RET_OK )
    {
        shell_put_line(SHELL_STR_OK);
    }
    else
    {
        shell_put_line(SHELL_STR_ERR);
    }

    return iRetCode;
}

/* Public definition -------------------------------------------------------*/

/* EOF */