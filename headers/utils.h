/**
 * @file utils.h
 * @author Roee Attias(s)
 * @date 2024-03-14
 * @brief header file for utilities functions and definitions.
 * 
 * This header file provides function prototypes and type definitions that
 * acts as "helpers" function for the other program components.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "opcode.h"
#include "memory.h"
#include "constants.h"
#include "errors.h"

/**
 * Function: isStrNumber
 * ---------------------
 * checks if a string is a valid number
 * 
 * @param str: the string to validate
 * 
 * @return: a boolean value (true - is number) (false - is not a number)
*/
bool isStrNumber(const char *str);

/**
 * Function: isValidLable
 * ----------------------
 * checks if a string is a valid lable
 * 
 * @param str: the string to validate
 * 
 * @return: a boolean value (true - valid lable) (false - invalid lable)
*/
bool isValidLable(const char *str);

/**
 * Function: isValidCommaSyntax
 * ----------------------------
 * validating the comma syntax of .data instructions and returns boolean value based no the result
 * 
 * @param token: the .data instruction string
 * 
 * @return boolean value indicated if the instruction format is valid or not (true - valid, false - not valid)
*/
bool isValidCommaSyntax(char *token);

/**
 * Function: isValidOperationSyntax
 * --------------------------------
 * validating the comma syntax of machine operation and returns boolean value based no the result
 * 
 * @param token: the buffer the entire line of the operation
 * @param operation: the operation name
 * @param number_of_operands: the number of operands that are allowed to be used with the specific operation
 * 
 * @return boolean value indicated if the operation format is valid or not (true - valid, false - not valid)
*/
bool isValidOperationSyntax(char *buffer, const char *operation, const unsigned int number_of_operands);

/**
 * Function: isDirectRegisterAddrMethod
 * ------------------------------------
 * checking if the addressing method of the operand is direct register
 * 
 * @param token: the operand to check
 * 
 * @return a boolean value indicated if the addressing method of the operands is indeed a direct register 
*/
bool isDirectRegisterAddrMethod(const char *token);

/**
 * Function: isImmediateAddrMethod
 * -------------------------------
 * checking if the addressing method of the operand is immediate
 * 
 * @param token: the operand to check
 * 
 * @return a boolean value indicated if the addressing method of the operands is indeed immediate
*/
bool isImmediateAddrMethod(const char* token);

/**
 * Function: isDirectAddrMethod
 * ----------------------------
 * checking if the addressing method of the operand is direct
 * 
 * @param token: the operand to check
 * 
 * @return a boolean value indicated if the addressing method of the operands is indeed direct
*/
bool isDirectAddrMethod(const char* token);

/**
 * Function: isFixedIndexAddrMethod
 * --------------------------------
 * checking if the addressing method of the operand is fixed index
 * 
 * @param token: the operand to check
 * 
 * @return a boolean value indicated if the addressing method of the operands is indeed fixed index
*/
bool isFixedIndexAddrMethod(const char* token);

/**
 * Function: isLlegalAddressingMethod
 * ----------------------------------
 * checking if the addressing method of the operand is llegal based on the opcode provided
 * 
 * @param is_source: a boolean that indicated if the addressing method
 * being checked is of the source or destenation operands
 * @param opcode: the opcode object
 * @param address_method: an enum value indicated what is the addressing method to check
 * 
 * @return: true for valid and false for not valid
*/
bool isLlegalAddressingMethod(const bool is_source, const Opcode *opcode, const AddrMethodsIndices address_method);

/**
 * Function isProgramSavedWord
 * ---------------------------
 * checking if the token is a saved word of the program (register, instruction...)
 * 
 * @param token: the word to check
 * 
 * @return: a boolean value true - is a saved word otherwise false
*/
bool isProgramSavedWord(const char *token);

/**
 * Function: removeWhitespaces
 * ---------------------------
 * removing all of the whitespaces from a givin string and returning the new string back
 * 
 * @param str: the string to remove the whitespaces from
 * 
 * @return: the new string without the whitespaces
*/
char* removeWhitespaces(char *str);

/**
 * Function: shiftLeft
 * -------------------
 * shifting the index of each element in the arr amount times left
 * 
 * @param arr: the array containing the integers to shift
 * @param size: the number of elements int the array
 * @param amount: the number of indices to move left
*/
void shiftLeft(unsigned int *arr, const int size, const int amount);

/**
 * Function: itob (integer to binary)
 * ----------------------------------
 * converting an integer to his 2's complement binary representaion
 * 
 * @param num: the number to convert
 * 
 * @return: an unsigned int array containing the binary value
*/
unsigned int* itob(int num);

/**
 * Function: extractIndex
 * ----------------------
 * extracing the index out of the operands and returning the actual index
 * 
 * @param str: the string operand
 * 
 * @return: a string holding only the index value
*/
char *extractIndex(char *str);

/**
 * Function: extractSymbolFromIndex
 * --------------------------------
 * extracting the symbol from the index defention and returning only the symbol
 * 
 * @param str: the string holding the index defenition
 * 
 * @return: a string containing only the symbol
*/
char* extractSymbolFromIndex(char *str);

#endif