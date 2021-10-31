/**
 * @file    shell.h
 * @author  Sebastián Del Moral Gallardo (sebmorgal@gmail.com)
 * @brief   Simple CLI library based on interrupt memfault article 
 *          (https://interrupt.memfault.com/blog/firmware-shell#building-a-basic-shell).
 * @version 0.1
 * @date    2021-07-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __SHELL_H
#define __SHELL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Exported includes --------------------------------------------------------*/
#include <stddef.h>

/* Exported defines ---------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/

typedef struct ShellCommand {
    const char *command;
    int (*handler)(int argc, char *argv[]);
    const char *help;
} sShellCommand;

typedef struct ShellImpl {
    int (*send_char)(char c);   /*<! Function to call whenever a character needs to be sent out. */
} sShellImpl;

/* Exported macro -----------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/

extern const sShellCommand *const g_shell_commands;
extern const size_t g_num_shell_commands;

/* Exported functions prototypes --------------------------------------------*/

/**
 * @brief Initializes the demo shell. To be called early at boot.
 * 
 * @param impl handler for shell interface.
 */
void shell_boot(const sShellImpl *impl);

/**
 * @brief Call this when a character is received. The character is processed synchronously.
 * 
 * @param c character received.
 */
void shell_receive_char(char c);

/**
 * @brief Print help command.
 * 
 * @param argc number of arguments.
 * @param argv string of arguments
 * @return int 
 */
int shell_help_handler(int argc, char *argv[]);

/**
 * @brief Prints a line then a newline.
 * 
 * @param str string to print.
 */
void shell_put_line(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __SHELL_H */

/* EOF */