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

#include "audio_engine.h"

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
int cli_cmd_assert(int argc, char *argv[]);
int cli_cmd_logOn(int argc, char *argv[]);
int cli_cmd_logOff(int argc, char *argv[]);
int cli_cmd_setLogLvl(int argc, char *argv[]);
int cli_cmd_wave(int argc, char *argv[]);
int cli_cmd_midi(int argc, char *argv[]);
int cli_cmd_delay(int argc, char *argv[]);
int cli_cmd_filter(int argc, char *argv[]);

/* Private variable --------------------------------------------------------*/

/* List of commands implemented */
static const sShellCommand s_shell_commands[] = {
    { "wave", cli_cmd_wave, "Select output waveform. WaveId [0-4]" },
    { "midi", cli_cmd_midi, "Activate midi note. Voice [0-5], Note [0-126], State [0-1]" },
    { "delay", cli_cmd_delay, "Update DELAY section. Time (seconds), Feedback (0-1)" },
    { "filter", cli_cmd_filter, "Update FILTER section. Frquency (Hz), Q" },
    { "logon", cli_cmd_logOn, "Enable global log" },
    { "logoff", cli_cmd_logOff, "Disable global log" },
    { "setloglvl", cli_cmd_setLogLvl, "Set new log level for defined interface. Interface [0-7], Level [0-3]" },
    { "assert", cli_cmd_assert, "Force system assert" },
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
 * @brief Force assertin error subsystem
 * 
 * @param argc Number of arguments, 1
 * @param argv List of arguments, 1
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_assert(int argc, char *argv[])
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
        ERR_ASSERT(0U);
    }

    return iRetCode;
}

/**
 * @brief Select output waveform.
 * 
 * @param argc number of arguments, 2.
 * @param argv List of arguments, argv[0]: cmd name, argv[1] wave_id.
 * @return int Status:  0, OK, !0, ERROR.
 */
int cli_cmd_wave(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 2U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        /* Check interface */
        uint32_t u32WaveId = (uint32_t)atoi(argv[1U]);

        if ( u32WaveId < (uint32_t)AUDIO_WAVE_NUM )
        {
            audio_cmd_t xAudioCmd = { 0U };

            xAudioCmd.eCmdId = AUDIO_CMD_SET_WAVEFORM;
            xAudioCmd.xCmdPayload.xSetWave.eVoiceId = AUDIO_VOICE_NUM;
            xAudioCmd.xCmdPayload.xSetWave.eWaveId = u32WaveId;

            if (AUDIO_handle_cmd(xAudioCmd) != AUDIO_OK)
            {
                iRetCode = SHELL_RET_ERR;
            }
        }
        else
        {
            shell_put_line("Wrong wave id value!");
            iRetCode = SHELL_RET_ERR;
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

/**
 * @brief Simulate midi note activation.
 * 
 * @param argc number of arguments, 4.
 * @param argv List of arguments, argv[0]: cmd name, argv[1] voice_id, argv[2] vmidi note, argv[3] voice_state.
 * @return int Status:  0, OK, !0, ERROR.
 */
int cli_cmd_midi(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 4U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        /* Check interface */
        uint8_t u8Voice = (uint8_t)atoi(argv[1U]);
        uint8_t u8Note = (uint8_t)atoi(argv[2U]);
        uint8_t u8State = (uint8_t)atoi(argv[3U]);

        if ( (u8Voice < AUDIO_VOICE_NUM) && (u8Note < 127U) && (u8State <= 1U) )
        {
            audio_cmd_t xAudioCmd = { 0U };

            xAudioCmd.eCmdId = AUDIO_CMD_SET_MIDI_NOTE;
            xAudioCmd.xCmdPayload.xSetMidiNote.eVoiceId = u8Voice;
            xAudioCmd.xCmdPayload.xSetMidiNote.u8Note = u8Note;
            xAudioCmd.xCmdPayload.xSetMidiNote.u8Velocity = 120U;
            xAudioCmd.xCmdPayload.xSetMidiNote.bActive = (bool)u8State;

            if (AUDIO_handle_cmd(xAudioCmd) != AUDIO_OK)
            {
                iRetCode = SHELL_RET_ERR;
            }
        }
        else
        {
            shell_put_line("Wrong midi params!");
            iRetCode = SHELL_RET_ERR;
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

/**
 * @brief Update parameters for delay section
 * 
 * @param argc Number of arguments, 3
 * @param argv List of arguments, argv[0]: cmd name, argv[1] delay time in seconds , argv[2] feedback level (<1)
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_delay(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 3U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        /* Check interface */
        float fTime = (float)atof(argv[1U]);
        float fFeedback = (float)atof(argv[2U]);

        if ( fFeedback < 1.0F )
        {
            audio_cmd_t xAudioCmd = { 0U };

            xAudioCmd.eCmdId = AUDIO_CMD_UPDATE_DELAY;
            xAudioCmd.xCmdPayload.xUpdateDelay.fTime = fTime;
            xAudioCmd.xCmdPayload.xUpdateDelay.fFeedback = fFeedback;

            if (AUDIO_handle_cmd(xAudioCmd) != AUDIO_OK)
            {
                iRetCode = SHELL_RET_ERR;
            }
        }
        else
        {
            shell_put_line("Wrong Feedback value!");
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

/**
 * @brief Update parameters for filter section
 * 
 * @param argc Number of arguments, 3
 * @param argv List of arguments, argv[0]: cmd name, argv[1] cutof frequency, argv[2] filter Q
 * @return int Status:  0, OK, !0, ERROR
 */
int cli_cmd_filter(int argc, char *argv[])
{
    int iRetCode = SHELL_RET_OK;

    if ( argc != 3U )
    {
        iRetCode = SHELL_RET_ERR;
    }
    else
    {
        /* Check interface */
        float fFreqCutoff = (float)atof(argv[1U]);
        float fQ = (float)atof(argv[2U]);

        audio_cmd_t xAudioCmd = { 0U };

        xAudioCmd.eCmdId = AUDIO_CMD_UPDATE_FILTER;
        xAudioCmd.xCmdPayload.xUpdateFilter.fFreq = fFreqCutoff;
        xAudioCmd.xCmdPayload.xUpdateFilter.fQ = fQ;

        if (AUDIO_handle_cmd(xAudioCmd) != AUDIO_OK)
        {
            iRetCode = SHELL_RET_ERR;
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