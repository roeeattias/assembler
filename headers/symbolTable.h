/**
 * @file symbolTable.h
 * @author Roee Attias(s)
 * @date 2024-03-12
 * @brief header file for the symbol table fields, functions and definitions.
 * 
 * This header file provides function prototypes and type definitions related to the
 * symbol table used for storing and managing macro information within the program.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/**
 * symbol table identefiers
*/
typedef enum SymbolsIdentefiers {
    MDEFINE_IDENTEFIER,
    DATA_IDENTEFIER,
    CODE_IDENTEFIER,
    EXTERNAL_IDENTEFIER
} SymbolsIdentefiers;

/**
 *  a struct holding the SymbolTable information
*/
typedef struct SymbolTable {
    char* symbol;
    unsigned int identifier:2;
    int value;
    struct SymbolTable *next;
} SymbolTable;

extern SymbolTable* symbol_table;

/**
 * Function insertSymbol
 * ---------------------
 * gets the symbol information (e.g. symbol name, identefier and value)
 * and insert it to the symbol table
 * 
 * @param symbol: string holding the symbol name
 * @param identerfier: an integer holding the id of the symbol (e.g. .code, .mdefine or .data)
 * @param value: the value of the symbol in the memory
*/
void insertSymbol(const char* symbol, const int identifier, const int value);

/**
 * Function lookupSymbol
 * ----------------------
 * searching a symbol in the symbol table
 * 
 * @param symbol: a string holding the symbol name to search
 * 
 * @return a pointer to the symbol matching the name passed as an argument
*/
SymbolTable* lookupSymbol(const char* symbol);

/**
 * Function freeSymbolTable
 * ------------------------
 * releasing the memory that was allocated to the symbol_table data structure
*/
void freeSymbolTable();

/**
 * Function: updateDataImageMemory
 * -------------------------------
 * updates the symbols that has the DATA identifier to me after the instruction image by adding to the address of each symbol
 * IC + STARTING_MEMORY_ADDRESS (100)
 * 
 * @param ic: the current ic counter (end of instruction image)
*/
void updateDataImageMemory(const unsigned int ic);

#endif