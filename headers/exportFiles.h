#ifndef EXPORT_FILES
#define EXPORT_FILES

#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "symbolTable.h"
#include "firstTransition.h"
#include "secondTransition.h"
#include "state.h"
#include "constants.h"
#include "errors.h"

extern char base_four_encoding[NUMBER_OF_BASE_FOUR_SIGNS];

/**
 * Function export_files
 * ---------------------
 * handling the file export and writing
*/
void export_files();

/**
 * Function write_to_object_file
 * -----------------------------
 * write to the object file and handle the base 4 encoding
 * 
 * @param file: the pointer to the created object file
*/
void write_to_object_file(FILE *file);

/**
 * Function write_to_ext_ent_files
 * -------------------------------
 * iterating over the linked list holding the entries and externals symbols and write to the neccery file
 * 
 * @param entries_name: then name of the entries file (ent)
 * @param externals_name: then name of the externals file (ext)
*/
void write_to_ext_ent_files(const char *entries_name, const char *externals_name);

#endif