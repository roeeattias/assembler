#include "../headers/utils.h"

bool isStrNumber(const char *str)
{
    size_t i = 0;
    /* checking if the first character is a digit or a valid integer sign */
    if (isdigit(str[0]) == 0 && str[0] != '-' && str[0] != '+') {
        return false;
    }
    
    /* checking if each character after the first one is a digit */
    for (i = 1; i < strlen(str); i++) {
        if (isdigit(str[i]) == 0) {
            return false;
        }
    }
    return true;
}

bool isValidLable(const char *str)
{
    /* checking if the string has a valid length if it starts with a letter and has ':' at the end */
    if (strlen(str) > MAX_LABEL_LENGTH || isalpha(str[0]) == 0 || str[strlen(str) - 1] != ':') {
        return false;
    }
    return true;
}

bool isValidCommaSyntax(char *buffer)
{
    int i = 0;
    bool lastComma = false;

    /* moving the buffer pointer after the .data instruction and deleting whitespaces */
    buffer = buffer + sizeof(DATA);
    buffer = removeWhitespaces(buffer);

    if (buffer[0] == ',' || buffer[strlen(buffer) - 2] == ',') {
        return false; /* returning false if the list has comma at the start or at the end */
    }
    /* iterating over the list and validating comma structure */
    for (i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',') {
            if (lastComma) {
                /* if comma found and the last element was cooma as well and not a number/symbol */
                return false;
            } else {
                lastComma = true;
            }
        } else if (buffer[i] != '-' && buffer[i] != '+') {
            lastComma = false;
        }
    }
    return true;
}

bool isValidOperationSyntax(char *buffer, const char *operation, const unsigned int number_of_operands) {
    int number_of_commas = 0, i = 0, count = 0;
    char *copy_of_buffer = NULL;
    char *token = NULL;
    
    /* removing whitespaces from the buffer and moving the pointer after the opcode */
    buffer = removeWhitespaces(buffer);
    buffer = buffer + strlen(operation);

    /* computing the number of commas allowed for the operation based on number of arguments */
    number_of_commas = number_of_operands - 1;
    
    /* coping the buffer to not modify the original one */
    copy_of_buffer = calloc(strlen(buffer), sizeof(char));
    strcpy(copy_of_buffer, buffer);
    
    token = strtok(copy_of_buffer, " \t\n\f\r,");
    if (token != NULL && number_of_operands == 0) {
        free(copy_of_buffer);
        return false;
    }
    free(copy_of_buffer);

    if (buffer[0] == ',' || buffer[strlen(buffer) - 1] == ',') {    
        return false; /* returning false if there is a comma at the start or at the end of the operation */
    }
    /* counting the number of commas*/
    for (i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',') {
            count++;
            if (count > number_of_commas) {
                return false;
            }
        }
    }
    if (count < number_of_commas) {
        return false;
    }
    return true;
}

bool isDirectRegisterAddrMethod(const char *token)
{
    int i = 0;
    /* checking if the operand is a register */
    if (token[0] == 'r' && strlen(token) >= 2) {
        for (i = 0; i < NUMBER_OF_REGISTERS; i++) {
            if (strcmp(token, registers[i]) == 0) {
                return true;
            }
        }
    }
    return false;
}

bool isImmediateAddrMethod(const char *token)
{
    if (token[0] == '#') {
        return true;
    }
    return false;
}

bool isDirectAddrMethod(const char* token)
{
    /* checking if the operand is a standalone lable */
    unsigned char c;
    if (!isalpha(token[0])) {
        return false;
    }
    while ( (c = *token) && (isalpha(c) || isdigit(c))) ++token;
    return *token == '\0'; 
}

bool isFixedIndexAddrMethod(const char* token)
{
    int i = 0, close_count = 0, open_count = 0;
    bool has_open = false;
    if (token[strlen(token) - 1] != ']') {
        return false;
    }
    for (i = 0; i < strlen(token); i++) {
        if (token[i] == ']') {
            if (!has_open) return false; /* returning false if encounter with ] without [*/
            close_count++;
        } else if (token[i] == '[') {
            has_open = true;
            open_count++;
        }
    }
    /* returning false if the number of '[' is different from the number of ']' */
    if (open_count != close_count) return false;
    return true;
}

bool isLlegalAddressingMethod(const bool is_source, const Opcode *opcode, const AddrMethodsIndices method)
{
    unsigned int src = 0, dest = 0;
    switch (method)
    {
    case IMMEDIATE:
        src = opcode->src.immediate;
        dest = opcode->dest.immediate;
        break;
    case DIRECT:
        src = opcode->src.direct;
        dest = opcode->dest.direct;
        break;
    case FIXED_INDEX:
        src = opcode->src.fixed_index;
        dest = opcode->dest.fixed_index;
        break;
    case DIRECT_REGISTER:
        src = opcode->src.fixed_index;
        dest = opcode->dest.fixed_index;
        break;
    }
    /* checking if the addressing method is valid for the source / dest operands */
    if (is_source && !src) {
        WARNING_IF_MSG(true, ERROR_ILLEGAL_ADDRESSING_METHOD);
        return false;
    } else if (!is_source && !dest) {
        WARNING_IF_MSG(true, ERROR_ILLEGAL_ADDRESSING_METHOD);
        return false;
    }
    return true;
}

bool isProgramSavedWord(const char *token)
{
    /* checking if the token is a saved word of the program */
    if (isDirectRegisterAddrMethod(token) || lookupOpcode(token) != NULL ||
    !strcmp(token, DATA) || !strcmp(token, STRING) || !strcmp(token, ENTRY) ||
    !strcmp(token, EXTERN) || !strcmp(token, DEFINE)) {
        return true;
    }
    return false;
}

char* removeWhitespaces(char *str) {
    int i, j = 0;
    int len = strlen(str);
    
    if (str == NULL) {
        return NULL;
    }
    /* removing whitespaces if found them */
    for (i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';

    return str;
}

void shiftLeft(unsigned int *arr, const int size, const int amount)
{
    int i = 0;
    /* shifting each element left by amount indices */
    for (i = 0; i < size; i++) {
        arr[i] = arr[(i + amount) % size];
    }
}

unsigned int* itob(int num)
{
    unsigned int *result = calloc(BINARY_WORD_SIZE, sizeof(unsigned int));
    int i = 0;
    /* converting the num to 2's complement binary number */
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--) {
        result[i] = (num & 1);
        num >>= 1;
    }
    return result;
}

char* extractIndex(char *str)
{
    int i = 0;
    char *open_brackets = NULL;
    char *substring = NULL;

    /* finding the position of '[' */
    open_brackets = strchr(str, '[');
    i = (int)(open_brackets - str);
    substring = calloc(strlen(str+2), sizeof(char));
    strncpy(substring, str + i + 1, strlen(str + i) - 2); /* coping only the index itself to the substring */
    return substring;
}

char* extractSymbolFromIndex(char *str)
{
    int i = 0;
    char *open_brackets = NULL;
    char *substring = NULL;

    /* finding the position of '[ */
    open_brackets = strchr(str, '[');
    i = (int)(open_brackets - str);
    substring = calloc((i + 1) * sizeof(char), sizeof(char));
    strncpy(substring, str, i);
    substring[i] = '\0';
    return  substring;
}
