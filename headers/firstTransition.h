/**
 * @file firstTransition.h
 * @author Roee Attias(s)
 * @date 2024-03-11
 * @brief Header file for the assembler first transition functions.
 *
 * This header file provides functions for the first transition source code file,
 * handling tasks such as inserting symbols to the symbols table with thier values,
 * keeping track of the IC/DC flags etc.
*/

#ifndef FIRST_TRANSITION_H
#define FIRST_TRANSITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "errors.h"
#include "symbolTable.h"
#include "constants.h"
#include "state.h"
#include "utils.h"
#include "memory.h"

/**
 * Function firstTransition
 * ------------------------
 * iterates through each line of the .am file and
 * calls `ft_processLine` to handle individual line-level processing tasks.
 * It manages overall file parsing and handles any file-level operations
 * required for first transition stage like symbol table managment.
*/
void firstTransition();

/**
 * Function ft_processLine
 * ---------------------
 * handles first transition tasks for a given line of the expanded source code, (symbol table managment)
 * It interacts with other modules like `symbolTable` for symbols insertion and lookup
 * and allocates memory for the data_image if needed.
 * 
 * @param buffer: A pointer to a character array containing the line to be processed.
*/
void ft_processLine(char *buffer);

#endif