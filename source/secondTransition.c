#include "../headers/secondTransition.h"

EntriesAndExternalsSymbols* externals_entries_table = NULL;

void secondTransition()
{
    char buffer[MAX_LINE_LENGTH + 1] = {0};
    char *base_filename = getCurrentFile();
    char *filename = (char*)calloc(strlen(base_filename) + sizeof(".am") + 1, sizeof(char));
    FILE *source = NULL;

    FAIL_IF_MSG(filename == NULL, ERROR_ALLOCATION_FAILED);
    strcpy(filename, base_filename);
    strcat(filename, ".am");
    /* opening the file need to be processed */
    source = fopen(filename, "r");
    free(filename);
    filename = NULL;
    FAIL_IF_MSG(source == NULL, ERROR_FAILED_TO_OPEN_FILE);
    
    IC = 0;
    /* Read lines until EOF (End Of File) is reached */
    while (fgets(buffer, MAX_LINE_LENGTH + 1, source) != NULL) {
        incrementCurrentLine();
        /* ensuring that buffer got string inside it */
        if (buffer[0] != '\0' && buffer[0] != '\n' && buffer[0] != ';') {
            /* calling ft_processLine function to preform line-level processing */
            st_processLine(buffer);
        }
    }
    fclose(source); /* closing the file after done using it */
}

void st_processLine(char *buffer)
{
    char *token = NULL;
    bool is_source = true;

    /* getting the first word of the line */
    token = strtok(buffer, " \t\n\f\r");
    RETURN_IF(token == NULL);

    if (isValidLable(token)) {
        /* skipping the lable in the operation */
        token = strtok(NULL, " \t\n\f\r");
    }

    /* skipping line if instruction was found */
    if (!strcmp(token, DATA) || !strcmp(token, STRING)
    || !strcmp(token, EXTERN) || !strcmp(token, DEFINE)) {
        return;
    }

    /* adding the symbol to the entries array */
    if (!strcmp(token, ENTRY)) {
        SymbolTable* symbol = NULL;

        token = strtok(NULL, " \t\n\f\r");
        symbol = lookupSymbol(token);
        insertExternalAndEntrySymbol(symbol->symbol, symbol->value, ENTRY_TYPE);
        return;
    }

    token = strtok(NULL, " \t\n\f\r,");
    incrementIC(1);
    while (token != NULL) {
        if (isDirectRegisterAddrMethod(token)) {
            /* updating the current IC based on the operands */
            if (is_source) incrementIC(1);
        } else if (isImmediateAddrMethod(token)) {
            incrementIC(1);
        } else if (isFixedIndexAddrMethod(token)) {
            /* creating a binary word for the sake of completing the machine code for lables */
            BinaryWord *binary_word = calloc(1, sizeof(BinaryWord));
            unsigned int *binary = {0};
            char *symbol_name = NULL;
            SymbolTable *symbol = NULL;
            int i = 0;

            if (instruction_image[getIC()] != NULL) {
                return; /* checking if there is already a binary word in the instructions image */
            }

            symbol_name = extractSymbolFromIndex(token); /* getting only the symbol name from the indexed operand (LIST[sz] -> LIST) */
            symbol = lookupSymbol(symbol_name);
            free(symbol_name);
            symbol_name = NULL;
            WARNING_IF_MSG(symbol == NULL, ERROR_SYMBOL_DO_NOT_EXISTS);
            RETURN_IF(symbol == NULL);

            binary = itob(symbol->value);
            /* shifting the binary word left by 2 and adding the A,R,E value to it */
            shiftLeft(binary, BINARY_WORD_SIZE, 2);
            /* updating the A,R,E bits based on the symbol type */
            if (symbol->identifier == EXTERNAL_IDENTEFIER) {
                binary[BINARY_WORD_SIZE - 1] = 1;
            } else {
                binary[BINARY_WORD_SIZE - 2] = 1;
            }
            for (i = 0; i < BINARY_WORD_SIZE; i++) {
                Bit bit;
                bit.on = binary[i];
                binary_word->word[i] = bit;
            }
            free(binary);
            binary = NULL;
            instruction_image[getIC()] = binary_word; /* updating the instruction image with the new binary word of the symbol */
            incrementIC(2);
        } else if (isDirectAddrMethod(token)) {
            /* creating a binary word for the sake of completing the machine code for lables */
            BinaryWord *binary_word = (BinaryWord*)calloc(1, sizeof(BinaryWord));
            unsigned int *binary = {0};
            SymbolTable *symbol = NULL;
            int i = 0;

            if (instruction_image[getIC()] != NULL) {
                return; /* checking if there is already a binary word in the instruction image */
            }

            symbol = lookupSymbol(token);
            WARNING_IF_MSG(symbol == NULL, ERROR_SYMBOL_DO_NOT_EXISTS);
            RETURN_IF(symbol == NULL);

            binary = itob(symbol->value); /* converting the symbol to his binary value */
            shiftLeft(binary, BINARY_WORD_SIZE, 2);
            /* setting the A,R,E bits based on the symbol type */
            if (symbol->identifier == EXTERNAL_IDENTEFIER) {
                binary[BINARY_WORD_SIZE - 1] = 1;
            } else {
                binary[BINARY_WORD_SIZE - 2] = 1;
            }
            for (i = 0; i < BINARY_WORD_SIZE; i++) {
                Bit bit;
                bit.on = binary[i];
                binary_word->word[i] = bit;
            }
            free(binary);
            binary = NULL;
            instruction_image[getIC()] = binary_word; /* updating the instruction image with the new binary word of the symbol */
            if (symbol->identifier == EXTERNAL_IDENTEFIER) {
                /* inserting the extern symbol to the externals array for the .ext file creation process */
                insertExternalAndEntrySymbol(symbol->symbol, getIC() + STARTING_MEMORY_ADDRESS, EXTERNAL_TYPE);
            }
            incrementIC(1);
        }
        token = strtok(NULL, " \t\n\f\r");
        is_source = false; /* setting the is_source flag to flase meaning that the next operand whould not be the source operand */
    }
}

void insertExternalAndEntrySymbol(const char *name, const unsigned int address, unsigned int type)
{
    EntriesAndExternalsSymbols *prev = NULL;
    EntriesAndExternalsSymbols *cur = externals_entries_table;
    EntriesAndExternalsSymbols *new_symbol = NULL;

    for (cur = externals_entries_table; cur != NULL; cur = cur->next) {
        prev = cur;        
    }
    /* creating a new symbol */
    new_symbol = calloc(1, sizeof(EntriesAndExternalsSymbols));
    new_symbol->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(new_symbol->name, name);
    new_symbol->type = type;
    new_symbol->address = address;

    if (prev != NULL)
        prev->next = new_symbol;
    else
        externals_entries_table = new_symbol;
}

void freeEntriesAndExternalsSymbols()
{
    /* iterating over the symbol table and releasing the memory that was allocated for it */
    EntriesAndExternalsSymbols *next;
    EntriesAndExternalsSymbols *cur = externals_entries_table;
    for (cur = externals_entries_table; cur != NULL; cur = next) {
        next = cur->next;
        free(cur->name);
        free(cur);
    }
    externals_entries_table = NULL;
}