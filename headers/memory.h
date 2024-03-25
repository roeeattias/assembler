/**
 * @file memory.h
 * @author Roee Attias(s)
 * @date 2024-03-14
 * @brief header file for the memory fields, functions and definitions.
 * 
 * This header file provides function prototypes and type definitions related to the
 * program memory used for storing and managing the data and instructions images within the program.
 * along with some of the machine code
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "errors.h"
#include "opcode.h"
#include "utils.h"
#include "symbolTable.h"

/** a struct represents a single bit */
typedef struct Bit {
    unsigned int on:1;
} Bit;

/** a struct represent a 14 bits binary word in the program */
typedef struct BinaryWord {
    Bit word[BINARY_WORD_SIZE];
} BinaryWord;

/* the program counters for data and instructions images */
extern unsigned int IC;
extern unsigned int DC;

/* the program data and instruction images where the machine code is stored */
extern BinaryWord data_image[RAM / 2];
extern BinaryWord *instruction_image[RAM / 2];

/* the program registers names */
extern char *registers[NUMBER_OF_REGISTERS];

/**
 * Function encodeData
 * -------------------
 * goes through the .data instruction, convert and encode the binary code in the memory (data_image)
 * and updates the DC counter to the next open spot at the data image
 * 
 * @param str: the full string to check whether the syntax is valid or not.
 * @param token: the tokenized string from the first number.
*/
void encodeData(char *str, const char *token);

/**
 * Function encodeString
 * ---------------------
 * goes through the .string instruction and encode the binary code for each character
 * in the memory (data_image) and updates the DC counter to the next open spot at the instruction image.
 * 
 * @param token: the string to encode including the "".
*/
void encodeString(const char *token);

/**
 * Function encodeOperation
 * ------------------------
 * goes through the line and alayzing the operation operand, addressing methods and encoding
 * the machine code into the instruction data.
 * 
 * @param token: the starting pointer to the operation
*/
void encodeOperation(char *str, char *token);

/**
 * Function resetCounters
 * ----------------------
 * resets the counters DC, IC back to 0.
*/
void resetCounters();

/**
 * Function getIC
 * --------------
 * getter function for the IC counter
 * 
 * @return: the IC counter
*/
unsigned int getIC();

/**
 * Function getDC
 * --------------
 * getter function for the DC counter
 * 
 * @return: the DC counter
*/
unsigned int getDC();

/**
 * Function: incrementIC
 * --------------------
 * incrementing the IC counter by 'amount'
 * 
 * @param amount: the amount to increment IC
*/
void incrementIC(const int amount);

/**
 * Function: incrementIC
 * --------------------
 * incrementing the IC counter by 'amount'
 * 
 * @param amount: the amount to increment IC
*/
void incrementDC(const int amount);

#endif