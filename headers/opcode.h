/**
 * @file opcode.h
 * @author Roee Attias(s)
 * @date 2024-03-17
 * @brief Header file for the assembler allowed opcodes.
 *
 * This header file provides defenitions for the language opcodes,
 * handling tasks such as lookup for the opcode and storing thier values,
 */

#ifndef OPCODE_H
#define OPCODE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"


/* addressing methods values for easy identefication*/
typedef enum AddrMethodsIndices {
    IMMEDIATE,
    DIRECT,
    FIXED_INDEX,
    DIRECT_REGISTER
} AddrMethodsIndices;

/* strcut for holding 1 or 0 based on if the opcode is allowed to use specific addressing method */
typedef struct AddrMethods {
    unsigned int immediate:1;
    unsigned int direct:1;
    unsigned int fixed_index:1;
    unsigned int direct_register:1;
} AddrMethods;

/* struct for the opcode itself holding the name, the value and more information about opcode */
typedef struct Opcode {
    char name[4];
    char opcode[4];
    unsigned int number_of_operands:2;
    AddrMethods src;
    AddrMethods dest;
} Opcode;

/* array holding the opcodes objects */
extern Opcode opcode[NUM_OF_OPCODE];

/**
 * Function lookupOpcode
 * ---------------------
 * iterated through the opcode array and returning the pointer of the found opcode
 * otherwise returning NULL
 * 
 * @param opcode_name: the name for the opcode to search
 * 
 * @return: pointer to the matchin opcode and (null) if not opcode was found
*/
Opcode* lookupOpcode(const char *opcode_name);

#endif