/**
 * @file macroTable.h
 * @author Roee Attias(s)
 * @date 2024-03-08
 * @brief header file for macro table fields, functions and definitions.
 * 
 * This header file provides function prototypes and type definitions related to the
 * macro table used for storing and managing macro information within the program.
 */

#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* macroTable is a data structure responsable for holding the macros information in the program */
typedef struct MacroTable
{
    char *name;
    long start_position; /* starting position of the macro in the source file */
    long end_position; /* ending position of the macro in the source file */
    struct MacroTable* next; /* pointer to the next macro */
} MacroTable;

/* Global variable represents the macro table containing all the macros in the program */
/* defined as 'extern' to make it accessible and initialized once throughout the entire program */
extern MacroTable* macro_table;

/*
 * Function: freeMacroTable
 * ------------------------
 * releasing the memory that was allocated to the macro_table data structure
 */
void freeMacroTable();

/**
 * Function: lookupMacro
 * ---------------------
 * searching a macro in the table
 * 
 * @param name: string representing the macro name
 * 
 * @return: a pointer to the macro matching the name that was provided if found else returns (null)
 */
MacroTable* lookupMacro(const char* name);

/**
 * Function: insertMacro
 * --------------------
 * gets the macro indormation (e.g. macro name start/end position at the source file) and inserts it to the table
 * 
 * @param macro_name: string holding the name of the macro needed to be inserted
 * @param start_position: long int representing the starting position of the macro first line
 * @param end_position: long int representing the ending position of the macro last line (not including 'endmcr')
 */
void insertMacro(const char* macro_name, const long start_position, const long end_position);

#endif