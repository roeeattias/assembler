/**
 * @file preProcessor.h
 * @author Roee Attias(s)
 * @date 2024-03-08 
 * @brief Header file for preprocessor functions.
 *
 * This header file provides functions for preprocessing source code files,
 * handling tasks such as macro expansion, directive processing, and code
 * transformation before the actual compilation stage.
*/

#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "state.h"
#include "constants.h"
#include "errors.h"
#include "macroTable.h"

/**
 * Function preProcessor
 * ---------------------
 * iterates through each line of the given source file and
 * calls `pp_processLine` to handle individual line-level preprocessing tasks.
 * It manages overall file parsing and handles any file-level operations
 * required for preprocessing.
 * 
 * @param source: A pointer to the open file stream for the source code.
*/
void preProcessor(FILE* source);

/**
 * Function preProcessor
 * ---------------------
 * handles preprocessing tasks for a given line of source code, (macro expansion)
 * It interacts with other modules like `macroTable` for macro insertion and lookup
 * and potentially modifies the buffer content as needed.
 * 
 * @param buffer: A pointer to a character array containing the line to be processed.
 * @param source: A pointer to the open file stream for the source code.
*/
void processLine(char* buffer, FILE* source);

#endif