#include "../headers/symbolTable.h"

SymbolTable* symbol_table = NULL;

void insertSymbol(const char *symbol, const int identifier, const int value)
{
    SymbolTable *prev = NULL;
    SymbolTable *cur = symbol_table;
    SymbolTable *new_symbol = NULL;
    for (cur = symbol_table; cur != NULL;  cur = cur->next) {
        prev = cur;
    }

    new_symbol = calloc(1, sizeof(SymbolTable));

    new_symbol->symbol = calloc(strlen(symbol) + 1, sizeof(char));
    strcpy(new_symbol->symbol, symbol);
    new_symbol->identifier = identifier;
    new_symbol->value = value;

    if (prev != NULL)
        prev->next = new_symbol;
    else
        symbol_table = new_symbol;
}

SymbolTable* lookupSymbol(const char *symbol)
{
    SymbolTable *match = symbol_table;

    /* iterating over the symbol table and returning a pointer to the symbol
        if it was found otherwise returning null */
    for (; match != NULL; match = match->next) {
        if (strcmp(match->symbol, symbol) == 0)
            break;
    }

    return match;
}

void freeSymbolTable()
{
    /* iterating over the symbol table and releasing the memory that was
    allocated for it */
    SymbolTable *next;
    SymbolTable *cur;
    for (cur = symbol_table; cur != NULL; cur = next) {
        next = cur->next;
        free(cur->symbol);
        free(cur);
    }
    symbol_table = NULL;
}

void updateDataImageMemory(const unsigned int ic)
{
    /* updating each symbol address in the memory that is belongs to data allocation */
    SymbolTable** temp = &symbol_table;
    while (*temp != NULL) {
        if ((*temp)->identifier == DATA_IDENTEFIER) {
            (*temp)->value = (*temp)->value + ic + STARTING_MEMORY_ADDRESS;
        }
        temp = &(*temp)->next;
    }
}
