/**
 * @file constants.h
 * @author Roee Attias(s)
 * @date 2024-03-08
 * @brief header file for defining different errors used in the program
 * 
 * This header file provides error messages declerations used within the program
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * Program constants
*/
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define NUM_OF_OPCODE 16
#define BINARY_WORD_SIZE 14
#define STARTING_MEMORY_ADDRESS 100
#define NUMBER_OF_REGISTERS 8
#define NUMBER_OF_BASE_FOUR_SIGNS 4
#define RAM 4096
/**
 * Errors messages constants
*/
#define ERROR_NOT_ENOUGH_ARGUMENTS "Error: No files specified. Please provide one or more files as arguments.\n"
#define ERROR_FAILED_TO_OPEN_FILE "Error: File not found. Please check the file path and try again.\n"
#define ERROR_LINE_LENGTH_EXCEEDED "Error: Line length exceeded. Please write assembly file with max of 80 characters in each line.\n"
#define ERROR_DECLERING_MACRO_WITHOUT_NAME "Error: Macro declaration missing identifier. Please provide a name for the macro.\n"
#define ERROR_CREATING_FILE "Error: Could not create the file."
#define ERROR_ALLOCATION_FAILED "Error: Not able to allocate memory due to internal error.\n"
#define ERROR_READING_FROM_FILE "Error: Failed reading content from file. Please validate the file content.\n"
#define ERROR_DECLERING_MACRO_WITH_EXISTING_NAME "Error: Trying to declere a new macro with a name already exists in the macro table.\n"
#define ERROR_DEFINE_WITHOUT_NAME "Defining without providing a name.\n"
#define ERROR_INVALID_DEFINE_NAME "Trying to use illegal name as a define.\n"
#define ERROR_INVALID_DEFINE_STRUCTURE "Trying to define with illegal structure.\nplease used this format for define instructions: .define <define_name> = <integer value>.\n"
#define ERROR_INVALID_LABLE_NAME "Trying to define a lable with illegal structure."
#define ERROR_INVALID_DATA_FORMAT "Trying to use .data with illegal format.\nPlease use the .data instruction as follow: .data <int>, <int>..."
#define ERROR_INVALID_STRING_FORMAT "Trying to use .string with illegal format.\nPlease use the .string instruction as follow: .string <string content>\n"
#define ERROR_INVALID_EXTERN_FORMAT "Trying to use .extern with illegal format.\nPlease use the .extern instruction without any lables as parameters."
#define ERROR_INVALID_OPERATION_FORMAT "Trying to use an operation with illegal format.\nPlease check the numbers of commas used or the type of the operands."
#define ERROR_ILLEGAL_ADDRESSING_METHOD "Trying to use illegal addressing method in operation."
#define ERROR_SYMBOL_ALREADY_EXISTS "Symbol already exists at the symbol table."
#define ERROR_SYMBOL_DO_NOT_EXISTS "Symbol do not exists in the symbol table."
#define ERROR_OPCODE_DO_NOT_EXISTS "Opcode do not exists in the opcodes list avilable for this project."
#define ERROR_SYMBOL_IS_NOT_MDEFINE "Symbol is not an mdefine variable, can't use it as a number."
#define ERROR_TO_MUCH_OPERANDS "Using to much operands in the operation."
#define ERROR_ADDRESSING_METHOD_DO_NOT_EXISTS "Using illegal addressing method in one of the operands used."

/* instructions */
#define DATA    ".data"
#define STRING  ".string"
#define ENTRY   ".entry"
#define EXTERN  ".extern"

#define DEFINE  ".define"

#endif