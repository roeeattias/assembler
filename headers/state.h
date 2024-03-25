/**
 * @file state.h
 * @author Roee Attias(s)
 * @date 2024-03-08
 * @brief Header file for the assembler state information.
 *
 * This header file defines data structures and functions for managing the
 * state information of the entire program. The state information includes
 * the current status stage of the code (e.g., preprocessor) and the currently processed file
 */

#ifndef STATE_H
#define STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FIRST_TRANSITION 1
#define SECOND_TRANSITION 2

/* Structure for program State */
typedef struct State {
  char* current_file;
  unsigned int currect_line;
  bool first_transition_failed;
  bool second_transition_failed;
} State;


/* Global State Variable (Declared as extern) */
extern State global_state;


/* Setters */

/**
 * Function: setCurrentFile
 * ------------------------
 * Sets the name of the currently processed source file.
 *
 * @param file The name of the currently processed source file.
 */
void setCurrentFile(const char* file);

/**
 * Function setFirstTransitionFailed
 * ---------------------------------
 * setting a boolean value that indicated if the first transition proccess was failed
 * 
 * @param value: the boolean value - true for failed and false for success
 * @param transition: the number of the transition that failed. (1 - first transition) (2 - second transition)
*/
void setTransitionFailed(const int transition, const bool value);

/**
 * Function: resetCurrentLine
 * --------------------------
 * reseting the current line back to 0
*/
void resetCurrentLine();

/**
 * Function: incrementCurrentLine
 * ------------------------------
 * incrementing the current line being proccessed by the source code
*/
void incrementCurrentLine();


/* Getters */

/**
 * Function: getCurrentFile
 * -----------------------
 * Gets the name of the currently processed source file.
 *
 * @return A pointer to the string containing the current file name.
*/
char* getCurrentFile();

/**
 * Function: getCurrentLine
 * ------------------------
 * Gets the current line being proccessed by the assembler
 * 
 * @return: unsigned int value of the current line
*/
unsigned int getCurrentLine();


#endif
