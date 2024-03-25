#include "../headers/preProcessor.h"

void preProcessor(FILE* source)
{
    /* Allocate one extra byte for null terminator */
    char buffer[MAX_LINE_LENGTH + 1] = {0};
    /* getting the current file name being processed */
    char *base_filename = getCurrentFile();
    /* Combine lengths for accuracy and allocates only necessary space */
    char *filename = calloc(strlen(base_filename) + sizeof(".am") + 1, sizeof(char));
    FILE *expanded_source_file = NULL;

    /* failing if memory allocation for the filename has failed */
    FAIL_IF_MSG(filename == NULL, ERROR_ALLOCATION_FAILED);
    /* building the target file path (i.e. .am) */
    strcpy(filename, base_filename);
    strcat(filename, ".am");
    /* creating the target file */
    expanded_source_file = fopen(filename, "w+");
    free(filename);
    filename = NULL;

    FAIL_IF_MSG(expanded_source_file == NULL, ERROR_CREATING_FILE);

    /* Read lines until EOF (End Of File) is reached */
    while (fgets(buffer, MAX_LINE_LENGTH + 1, source) != NULL) {
        /* ensuring that buffer got string inside it */
        if (buffer[0] != '\0' && buffer[0] != '\n' && buffer[0] != ';') {
            /* Check if the line is longer than maximum allowed */
            size_t line_length = strlen(buffer);
            FAIL_IF_MSG(line_length > MAX_LINE_LENGTH, ERROR_LINE_LENGTH_EXCEEDED);
            
            /* calling pp_processLine function to preform line-level processing */
            processLine(buffer, source);
            
            /* writing the buffer to the target file */
            fprintf(expanded_source_file, "%s", buffer);

            /* resetting the buffer to prevent left over characters from previous iterations */
            memset(buffer, '\0', MAX_LINE_LENGTH);
        }
    }
    /* calling freeMacroTable to release the memory that being allocated to the macro_table */
    freeMacroTable();
    /* closing the target file after done using it */
    fclose(expanded_source_file);
}

void processLine(char *buffer, FILE* source)
{
    char* copy_of_buffer = NULL;
    char* token = NULL;
    static char* macro_name;
    size_t bytes_read = 0;
    static long int start_macro_position;
    static long int end_macro_position;
    static bool is_mcr;
    MacroTable* macro = NULL;

    /* checking if copy_of_buffer allocated successfully */
    copy_of_buffer = calloc(strlen(buffer) + 1, sizeof(char));
    FAIL_IF_MSG(copy_of_buffer == NULL, ERROR_ALLOCATION_FAILED);

    /* copying the current line to not modify the original one */
    strcpy(copy_of_buffer, buffer);
    token = strtok(copy_of_buffer, " \t\n\f\r");

    /* checking if token got string (skipping empty lines) */
    RETURN_IF(token == NULL);
    
    /* seacrhing if using a defined macro */
    macro = lookupMacro(token);

    /* if macro exists in and starts coping the content to buffer */
    if (macro != NULL) {
        /* saving the last position we read from */
        long int last_position = ftell(source);
        /* calulating the content length need to be copied */
        long int content_length = macro->end_position - macro->start_position;
        
        if (MAX_LINE_LENGTH < content_length) {
            /* TODO: need to allocated more space to buffer */
        } else {
            /* moving the pointer to the start position of the macro we want to copy */
            fseek(source, macro->start_position, SEEK_SET);
            
            /* deleting the macro name from the buffer */
            memset(buffer, '\0', MAX_LINE_LENGTH);
            
            /* reading the macro value to the buffer */
            bytes_read = fread(buffer, 1, content_length, source);
            
            /* checking if the content was read successfully */
            if (bytes_read != content_length)
            {
                FAIL_IF_MSG(true, ERROR_READING_FROM_FILE);
            }
            /* returning the the last position we were */
            fseek(source, last_position, SEEK_SET);
        }
    } else if (strcmp(token, "mcr") == 0) {
        token = strtok(NULL, " \t\n\f\r");
        FAIL_IF_MSG(token == NULL, ERROR_DECLERING_MACRO_WITHOUT_NAME);
        
        macro = lookupMacro(token);
        FAIL_IF_MSG(macro != NULL, ERROR_DECLERING_MACRO_WITH_EXISTING_NAME);

        /* checking if copy_of_buffer allocated successfully */
        macro_name = calloc(strlen(token) + 1, sizeof(char));
        FAIL_IF_MSG(macro_name == NULL, ERROR_ALLOCATION_FAILED);

        strcpy(macro_name, token);
        start_macro_position = ftell(source);
        is_mcr = true;

        /* deleting the line where the mcr decleration was found from the file */
        memset(buffer, '\0', MAX_LINE_LENGTH);
    }
    
    /* checking if the is_mcr flag is on indicating that we are inside of a macro */
    if (is_mcr) {
        /* checking if we reached the end of the macro decleration */
        if (strcmp(token, "endmcr") == 0) {
            end_macro_position = ftell(source) - strlen(buffer);
            is_mcr = false;

            /* adding new macro to the macro table */
            insertMacro(macro_name, start_macro_position, end_macro_position);
            free(macro_name);
            macro_name = NULL;

            /* reseting the macro positions to defualt values */
            start_macro_position = 0;
            end_macro_position = 0;
        }
        /* deleting the line content before writing it to the file */
        memset(buffer, '\0', MAX_LINE_LENGTH);
    }
    free(copy_of_buffer);
    copy_of_buffer = NULL;
}