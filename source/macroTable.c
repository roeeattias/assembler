#include "../headers/macroTable.h"

/* initializing the macro_table global variable to (null) */
MacroTable* macro_table = NULL;

void freeMacroTable() {
    MacroTable *temp = NULL;
    while (macro_table != NULL)
    {
        /* releasing each macro in the list */
        temp = macro_table;
        macro_table = macro_table->next;
        free(temp);
        temp = NULL;
    }
}

void insertMacro(const char* macro_name, const long start_position, const long end_position)
{
    MacroTable *prev = NULL;
    MacroTable *cur = macro_table;
    MacroTable *new_macro = NULL;
    for (cur = macro_table; cur != NULL;  cur = cur->next) {
        prev = cur;
    }

    new_macro = calloc(1, sizeof(MacroTable));

    new_macro->name = calloc(strlen(macro_name) + 1, sizeof(char));
    strcpy(new_macro->name, macro_name);
    new_macro->start_position = start_position;
    new_macro->end_position = end_position;
    new_macro->next = NULL;

    if (prev != NULL)
        prev->next = new_macro;
    else
        macro_table = new_macro;
}

MacroTable* lookupMacro(const char* name)
{
    MacroTable* match = macro_table;

    /* iterating over the symbol table and returning a pointer to the symbol
        if it was found otherwise returning null */
    for (; match != NULL; match = match->next) {
        if (strcmp(match->name, name) == 0) {
            break;
        }
    }
    /* returning (null) if no macro being found */
    return match;
}
