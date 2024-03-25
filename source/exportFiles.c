#include "../headers/exportFiles.h"

/* base 4 encoding symbols table */
char base_four_encoding[] = {'*', '#', '%', '!'};

void export_files()
{
    FILE *object = NULL;
    char *base_filename = getCurrentFile();
    char *filename = calloc(strlen(base_filename), sizeof(char));
    char *entries_name = NULL;
    char *externals_name = NULL;

    FAIL_IF_MSG(filename == NULL, ERROR_ALLOCATION_FAILED);
    /* building the filename to create .ob */
    strcpy(filename, base_filename);
    strcat(filename, ".ob");
    object = fopen(filename, "w");
    free(filename);
    filename = NULL;
    
    FAIL_IF_MSG(object == NULL, ERROR_CREATING_FILE);
    write_to_object_file(object); /* calling a function to create the object file and write the data to it */

    /* building the names for the external and entries files */
    entries_name = calloc(strlen(base_filename), sizeof(char));
    FAIL_IF_MSG(entries_name == NULL, ERROR_ALLOCATION_FAILED);
    strcpy(entries_name, base_filename);
    strcat(entries_name, ".ent");

    externals_name = calloc(strlen(base_filename), sizeof(char));
    FAIL_IF_MSG(externals_name == NULL, ERROR_ALLOCATION_FAILED);
    strcpy(externals_name, base_filename);
    strcat(externals_name, ".ext");

    write_to_ext_ent_files(entries_name, externals_name);
    
    free(entries_name);
    free(externals_name);
    entries_name = NULL;
    externals_name = NULL;
    
    fclose(object);
}

void write_to_object_file(FILE *file)
{
    int i = 0;
    int j = 0;
    int address = STARTING_MEMORY_ADDRESS;
    char *word_address = (char*)calloc(5 * sizeof(char), sizeof(char));
    
    fprintf(file, "  %d %d\n", getIC(), getDC()); /* writing the headline of the object file */
    for (i = 0; i < getIC(); i++) {
        sprintf(word_address, "%04d", address); /* coping the memory address with leading zeros */
        address++;
        fprintf(file, "%s ", word_address); /* writing the memory address with leading zeros */
        for (j = 0; j < BINARY_WORD_SIZE - 1; j += 2) {
            /* calculating the symbol and writing it to the file */
            int bit1 = instruction_image[i]->word[j].on == 1 ? 2 : 0;
            int bit2 = instruction_image[i]->word[j+1].on;
            char sign = base_four_encoding[bit1 + bit2];
            fprintf(file, "%c", sign);
        }
        fprintf(file, "\n");
    }
    for (i = 0; i < getDC(); i++) {
        sprintf(word_address, "%04d", address); /* coping the memory address with the leading zeros of the data image elements */
        address++;
        fprintf(file, "%s ", word_address);
        for (j = 0; j < BINARY_WORD_SIZE - 1; j += 2) {
            /* calculating the symbol and writing it to the file */
            int bit1 = data_image[i].word[j].on == 1 ? 2 : 0;
            int bit2 = data_image[i].word[j+1].on;
            char sign = base_four_encoding[bit1 + bit2];
            fprintf(file, "%c", sign);
        }
        fprintf(file, "\n");
    }
    free(word_address);
}

void write_to_ext_ent_files(const char *entries_name, const char *externals_name)
{
    EntriesAndExternalsSymbols *symbol = externals_entries_table;
    FILE *entries = NULL;
    FILE *externals = NULL;
    for (; symbol != NULL; symbol = symbol->next) {
        /* if the symbol is entry symbol write it to ent file otherwise to the external */
        if (symbol->type == ENTRY_TYPE) {
            if (entries == NULL) {
                entries = fopen(entries_name, "w");
            }
            fprintf(entries, "%s\t%04d\n", symbol->name, symbol->address);
        } else {
            if (externals == NULL) {
                externals = fopen(externals_name, "w");
            }
            fprintf(externals, "%s\t%04d\n", symbol->name, symbol->address);
        }
    }
    fclose(entries);
    fclose(externals);
}