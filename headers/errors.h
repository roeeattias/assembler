/**
 * @file errors.h
 * @author Roee Attias(s)
 * @date 2024-03-08
 * @brief Header file for error handling macros.

 * This header file provides macros for handling error situations in the C code.
 * These macros simplify error checking and reporting, promoting code readability
 * and maintainability.
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>
#include "state.h"

/* Macros for error handling situations */

/**
 * Macro FAIL_IF_MSG
 * -----------------
 * Terminates the program with an error message if the expression `EXP` evaluates to true.
 * 
 * @param EXP The expression to evaluate for error condition.
 * @param MSG The error message to print if the condition is true.
*/
#define FAIL_IF_MSG(EXP, MSG) {if(EXP){fprintf(stderr, MSG "\n"); exit(EXIT_FAILURE);}}

/**
 * Macro RETURN_IF
 * ---------------
 * Returns from the current function if the expression `EXP` evaluates to true.
 *
 * @param EXP The expression to evaluate for error condition.
 */
#define RETURN_IF(EXP) {if(EXP) {return;}}

/**
 * Macro WARNING_IF_MSG
 * --------------------
 * prints a warning / error caused by a line from the source code to stdout
 * includes the line that caused the error
 * 
 * @param EXP The expression to evaluate for error condition.
 * @param MSG The error message to print if the condition is true.
*/
#define WARNING_IF_MSG(EXP, MSG) {if(EXP) {fprintf(stdout, "%s.as:Error:%d:" MSG"\n", getCurrentFile(), getCurrentLine()); setTransitionFailed(1, true); setTransitionFailed(2, true);}}

#endif
