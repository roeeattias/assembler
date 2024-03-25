#include "../headers/firstTransition.h"

void firstTransition()
{
    /* Allocate one extra byte for null terminator */
    char buffer[MAX_LINE_LENGTH + 1] = {0};
    /* getting the current file name being processed */
    char *base_filename = getCurrentFile();
    /* Combine lengths for accuracy and allocates only necessary space */
    char *filename = calloc(strlen(base_filename) + sizeof(".am") + 1, sizeof(char));
    FILE *source = NULL;

    /* failing if memory allocation for the filename has failed */
    FAIL_IF_MSG(filename == NULL, ERROR_ALLOCATION_FAILED);
    /* building the target file path (i.e. .am) */
    strcpy(filename, base_filename);
    strcat(filename, ".am");
    /* opening the file need to be processed */
    source = fopen(filename, "r");
    free(filename);
    filename = NULL;

    FAIL_IF_MSG(source == NULL, ERROR_FAILED_TO_OPEN_FILE);

    resetCounters(); /* reseting the counters (IC, DC)*/

    /* Read lines until EOF (End Of File) is reached */
    while (fgets(buffer, MAX_LINE_LENGTH + 1, source) != NULL) {
        incrementCurrentLine();
        /* ensuring that buffer got string inside it */
        if (buffer[0] != '\0' && buffer[0] != '\n' && buffer[0] != ';') {
            /* calling ft_processLine function to preform line-level processing */
            ft_processLine(buffer);
            /* reseting the buffer for next iteration */
            memset(buffer, '\0', MAX_LINE_LENGTH);
        }
    }
    if (!global_state.first_transition_failed) {
        updateDataImageMemory(getIC());
    }
    resetCurrentLine();
    fclose(source); /* closing the file after done using it */
}

void ft_processLine(char *buffer)
{
    char *token = NULL;
    char *symbol_name = NULL;
    char *copy_of_buffer = NULL;
    int symbol_value = 0;
    bool inSymbolDecleration = false;
    SymbolTable* symbol = NULL;

    copy_of_buffer = calloc(strlen(buffer) + 1, sizeof(char));
    /* coping the buffer to not modify it */
    strcpy(copy_of_buffer, buffer);
    FAIL_IF_MSG(copy_of_buffer == NULL, ERROR_ALLOCATION_FAILED);
    
    /* getting the first word of the line */
    token = strtok(buffer, " \t\n\f\r");
    RETURN_IF(token == NULL);

    /* checking if define is used */
    if (strcmp(token, DEFINE) == 0) {
        /* extracting the name of the define */
        token = strtok(NULL, " \t\n\f\r");
        /* checking if define has a name and if it is valid*/
        WARNING_IF_MSG(token == NULL, ERROR_DEFINE_WITHOUT_NAME);
        WARNING_IF_MSG(isalpha(token[0]) == 0, ERROR_INVALID_DEFINE_NAME)
        RETURN_IF(token == NULL || isalpha(token[0]) == 0);
        
        /* looking if the token is aleardy exists (warning if true) */
        symbol = lookupSymbol(token);
        WARNING_IF_MSG(symbol != NULL, ERROR_SYMBOL_ALREADY_EXISTS);
        RETURN_IF(symbol != NULL);

        /* allocating memory for the symbol name */
        symbol_name = calloc(strlen(token), sizeof(char));
        FAIL_IF_MSG(symbol_name == NULL, ERROR_ALLOCATION_FAILED);
        strcpy(symbol_name, token);
        /* checking that the define name is not a saved word of the languge */
        if (isProgramSavedWord(symbol_name)) {
            WARNING_IF_MSG(true, ERROR_INVALID_DEFINE_NAME);
            free(symbol_name);
            symbol_name = NULL;
            return;
        }

        /* checking if the define has a llegal format (using the '=' sign)*/
        token = strtok(NULL, " \t\n\f\r");
        WARNING_IF_MSG(strcmp(token, "=") != 0, ERROR_INVALID_DEFINE_STRUCTURE);
        RETURN_IF(strcmp(token, "=") != 0);

        token = strtok(NULL, " \t\n\f\r");
        WARNING_IF_MSG(!isStrNumber(token), ERROR_INVALID_DEFINE_STRUCTURE);
        RETURN_IF(!isStrNumber(token));

        /* getting the define value and insert it to the symbol table */
        symbol_value = atoi(token);
        insertSymbol(symbol_name, MDEFINE_IDENTEFIER, symbol_value);
        free(symbol_name); /* releasing the memry that was allocated to the symbol name */
        symbol_name = NULL;
        return;
    } else if (isValidLable(token)) {
        inSymbolDecleration = true;
        /* allocating memory for the symbol name */
        symbol_name = calloc(strlen(token), sizeof(char));
        strcpy(symbol_name, token);
        FAIL_IF_MSG(symbol_name == NULL, ERROR_ALLOCATION_FAILED);
        symbol_name[strlen(symbol_name) - 1] = '\0';
        if (isProgramSavedWord(symbol_name)) {
            WARNING_IF_MSG(true, ERROR_INVALID_LABLE_NAME);
            free(symbol_name);
            symbol_name = NULL;
            return;
        }
        /* moving the pointer foword to skip the lable decleration */
        copy_of_buffer = copy_of_buffer + strlen(symbol_name) + 1;
        token = strtok(NULL, " \t\n\f\r");
    }

    if (!strcmp(token, DATA) || !strcmp(token, STRING)) {
        /* 0 if .data, 1 if .string */
        const int instruction_type = strcmp(token, DATA);

        /* adding a new symbol if in symbol decleration */
        if (inSymbolDecleration) {
            symbol = lookupSymbol(token);
            if (symbol != NULL) {
                /* throw a warning if the symbol already exists */
                WARNING_IF_MSG(symbol != NULL, ERROR_SYMBOL_ALREADY_EXISTS);
                return;
            }
            /* inserting a new data symbol with value of DC */
            insertSymbol(symbol_name, DATA_IDENTEFIER, getDC());
            free(symbol_name);
            symbol_name = NULL;
        }

        /* getting the start of the data initializtion */
        token = strtok(NULL, " \t\n\f\r,");
        
        if (!instruction_type) {
            /* encoding the data in the memory in case of .data instruction */
            encodeData(copy_of_buffer, token);
        } else {
            /* encoding the string in the memory in case of .string instruction */
            encodeString(token);
        }
        return;
    } else if (!strcmp(token, EXTERN) || !strcmp(token, ENTRY)) {
        /* if the instruction is .extern or .entry */
        int instruction_type = strcmp(token, EXTERN);

        /* warning if there is no arguments to the instruction */
        token = strtok(NULL, " \t\n\f\r");
        WARNING_IF_MSG(token == NULL, ERROR_INVALID_EXTERN_FORMAT);
        RETURN_IF(token == NULL);
        if (!instruction_type) {
            /* iterating over the arguments and adding them as external symbols */
            while (token != NULL) {
                insertSymbol(token, EXTERNAL_IDENTEFIER, 0);
                token = strtok(NULL, " \t\n\f\r");
            }
        }
        return;
    } else if (inSymbolDecleration) {
        /* if in code symbol decleration */
        symbol = lookupSymbol(symbol_name);
        if (symbol != NULL) {
            WARNING_IF_MSG(true, ERROR_SYMBOL_ALREADY_EXISTS);
            return;
        } else {
            insertSymbol(symbol_name, CODE_IDENTEFIER, getIC() + STARTING_MEMORY_ADDRESS);
            free(symbol_name);
            symbol_name = NULL;;
        }
    }

    /* encoding the operation in the memory if no case of the above was matched */
    encodeOperation(copy_of_buffer, token);
}