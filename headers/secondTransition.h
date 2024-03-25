/**
 * @file secondTransition.h
 * @author Roee Attias(s)
 * @date 2024-03-21
 * @brief Header file for the assembler second transition functions.
 *
 * This header file provides functions for the second transition source code file,
 * handling tasks such as completing symbols machine code, adding symbols to externals and entry tables,
 * keeping track of the IC/DC flags etc.
*/

#ifndef SECOND_TRANSITION_H
#define SECOND_TRANSITION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "state.h"
#include "errors.h"
#include "utils.h"
#include "memory.h"
#include "symbolTable.h"

typedef enum Type {
    EXTERNAL_TYPE,
    ENTRY_TYPE
} Type;

typedef struct EntriesAndExternalsSymbols {
    char *name;
    unsigned int address;
    unsigned int type;
    struct EntriesAndExternalsSymbols* next;
} EntriesAndExternalsSymbols;

extern EntriesAndExternalsSymbols* externals_entries_table;

/**
 * Function: secondTransition
 * --------------------------
 * iterates through each line of the .am file and
 * calls `ft_processLine` to handle individual line-level processing tasks.
*/
void secondTransition();

/**
 * Function st_processLine
 * ---------------------
 * handles second transition tasks for a given line of the expanded source code,
 * It interacts with other modules like `firstTransition` the completion of the machine code process
 * 
 * @param buffer: A pointer to a character array containing the line to be processed.
*/
void st_processLine(char *buffer);

/**
 * Function insertExternalAndEntrySymbol
 * -------------------------------------
 * inserts the externals and entries symbols to the list
 * 
 * @param name: the name of the symbol to add
 * @param address: the address of the symbol binary word in the machine code
 * @param type: the type of the symbol (external or entry)
*/
void insertExternalAndEntrySymbol(const char *name, const unsigned int address, unsigned int type);

/**
 * Function freeEntriesAndExternalsSymbols
 * ---------------------------------------
 * releasing the memory that was dynamicly allocated for the linked list
*/
void freeEntriesAndExternalsSymbols();

#endif