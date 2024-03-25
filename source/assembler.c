#include <stdio.h>
#include "../headers/errors.h"
#include "../headers/constants.h"
#include "../headers/state.h"
#include "../headers/macroTable.h"
#include "../headers/preProcessor.h"
#include "../headers/firstTransition.h"
#include "../headers/secondTransition.h"
#include "../headers/exportFiles.h"

int main(int argc, char **argv)
{
    FILE *source_file = NULL; /* pointer to the source file begin processed */
    char *filename = NULL; /* pointer to the file name as a string */
    int i = 1;
    
    /* failing if no files has bein passed as an aruments */
    FAIL_IF_MSG(argc <= 1, ERROR_NOT_ENOUGH_ARGUMENTS);
    
    do {
        /* setting the corrent file in the global state of the program */
        setCurrentFile(argv[i]);
        resetCurrentLine();

        /* Combine lengths for accuracy and allocates only necessary space */
        filename = calloc(strlen(argv[i]) + sizeof(".as") + 1, sizeof(char));

        /* failing if memory allocation for the filename has failed */
        FAIL_IF_MSG(filename == NULL, ERROR_ALLOCATION_FAILED);

        /* building the as file name */
        strcpy(filename, argv[i]);
        strcat(filename, ".as");

        /* opening the assembly source file */
        source_file = fopen(filename, "r");
        
        /* failing if an error occurred while opening the source file */
        FAIL_IF_MSG(source_file == NULL, ERROR_FAILED_TO_OPEN_FILE);

        /* calling to preProcessor function to start the Preprocessor stage, (result: .am file) */
        preProcessor(source_file);

        /* calling to firstTransition function to start the first transition stage, (result: symbol table)*/
        firstTransition();
        if (!global_state.first_transition_failed) {
            secondTransition();
        }
        if (!global_state.first_transition_failed && !global_state.second_transition_failed) {
            export_files();
        }

        /* memory release section */
        free(filename);
        filename = NULL;
        freeEntriesAndExternalsSymbols();
        freeSymbolTable();
        i++;
    } while (argv[i] != NULL);
   return 0;
}