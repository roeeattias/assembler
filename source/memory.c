#include "../headers/memory.h"

unsigned int IC = 0;
unsigned int DC = 0;

BinaryWord data_image[RAM / 2] = {0};
BinaryWord *instruction_image[RAM / 2] = {0};

char *registers[NUMBER_OF_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

void encodeData(char *str, const char *token)
{
    unsigned int *binary_number = NULL;
    int i = 0;
    SymbolTable* symbol = NULL;
    BinaryWord binary_word;

    /* checking if the .data instruction that was passed has a valid format */
    if (isValidCommaSyntax(str)) {
        /* going through each element in the list */
        while (token != NULL) {
            if (!isStrNumber(token)) {
                /* getting a symbol value in case that the element is not a number */
                symbol = lookupSymbol(token);
                WARNING_IF_MSG(symbol == NULL, ERROR_SYMBOL_DO_NOT_EXISTS);
                RETURN_IF(symbol == NULL || symbol->identifier != MDEFINE_IDENTEFIER);
                WARNING_IF_MSG(symbol->identifier != MDEFINE_IDENTEFIER, ERROR_SYMBOL_IS_NOT_MDEFINE);
                /* converting the symbol value to binary */
                binary_number = itob(symbol->value);
            } else {
                /* converting the number to binary */
                binary_number = itob(atoi(token));
            }
            /* setting all if the bits in the binary word to zeros */
            memset(binary_word.word, 0, sizeof(binary_word.word));
            for (i = 0; i < BINARY_WORD_SIZE; i++) {
                Bit bit;
                bit.on = binary_number[i];
                binary_word.word[i] = bit;
            }
            free(binary_number);
            binary_number = NULL;
            data_image[DC] = binary_word;
            DC++; /* incrementing DC to hold the next free spot of the data image */
            token = strtok(NULL, " \t\n\f\r,"); /* moving to next element */
        }
    } else {
        WARNING_IF_MSG(true, ERROR_INVALID_DATA_FORMAT);
    }
}

void encodeString(const char *token)
{
    int i = 0, j = 0;

    /* checking if the string has a valid format */
    if (token[0] != '"' || token[strlen(token) - 1] != '"') {
        WARNING_IF_MSG(true, ERROR_INVALID_STRING_FORMAT);
    } else {
        /* creating another binary word for the null terminator '0' */
        BinaryWord null_terminator;
        memset(null_terminator.word, 0, sizeof(null_terminator.word));

        /* iterating over each char in the string */
        for (i = 1; i < strlen(token) - 1; i++) {
            BinaryWord binary_word;
            memset(binary_word.word, 0, sizeof(binary_word.word));

            /* converting the char to binary and setting each bit to the binary word */
            for (j = 7; j >= 0; --j)
            {
                Bit bit;
                bit.on = (token[i] & (1 << j)) ? 1 : 0;
                binary_word.word[BINARY_WORD_SIZE - j - 1] = bit;
            }
            /* adding the binary word to the memory data image */
            data_image[DC] = binary_word;
            DC++; /* incrementing DC to hold the next free spot of the data image */
        }
        /* adding the null terminator to the data image */
        data_image[DC] = null_terminator;
        DC++; /* incrementing DC to hold the next free spot of the data image */
    }
}

void encodeOperation(char *str, char *token)
{
    /* allocating memory to the first word of the operation */
    BinaryWord *first_word = calloc(1, sizeof(BinaryWord));
    Bit bit1, bit2; /* the addressing method of each bit in the first word */
    Opcode *opcode = NULL;
    SymbolTable* symbol = NULL;
    char *binary_opcode = NULL;
    int i = 0, j = 0;
    bool was_source_register = false, is_source = false;

    /* checking if the opcode exists */
    opcode = lookupOpcode(token);
    WARNING_IF_MSG(opcode == NULL, ERROR_OPCODE_DO_NOT_EXISTS);
    RETURN_IF(opcode == NULL);

    /* calling isValidOperationSyntax to validate the format of the operation */
    if (!isValidOperationSyntax(str, token, opcode->number_of_operands)) {
        free(first_word);
        first_word = NULL;
        WARNING_IF_MSG(true, ERROR_INVALID_OPERATION_FORMAT);
        RETURN_IF(true);
    } else {
        /* setting the first word at the instruction image */
        instruction_image[IC] = first_word;
        IC++;
        /* getting the binary code of the opcode */
        binary_opcode = opcode->opcode;
        for (i = 4; i <= strlen(binary_opcode) + 3; i++) {
            Bit bit;
            bit.on = binary_opcode[i - 4] == '1' ? 1 : 0;
            first_word->word[i] = bit;   
        }
        i -= 1; /* setting the i to the next index of the first word */
        /* if the opcode has only one parameter adding 2 to i to make it go stright to the destenation operands bits */
        i += opcode->number_of_operands == 1 ? 2 : 0;
        str = str + strlen(token); /* moving the str pointer to skip the opcode */
        
        /* iterating through each operand in the operation and encoding his value to the instruction image */
        token = strtok(str, ",");
        is_source = opcode->number_of_operands == 2 ? true : false;
        while (token != NULL) {
            /* setting the additional binary word */
            BinaryWord *binary_word = calloc(1, sizeof(BinaryWord));
            if (isDirectRegisterAddrMethod(token)) {
                unsigned int *binary = {0};

                bit1.on = bit2.on = 1; /* the addressing method value of direct register */
                RETURN_IF(!isLlegalAddressingMethod(is_source, opcode, DIRECT_REGISTER));
                token += 1;
                binary = itob(atoi(token));
                if (was_source_register) {
                    /* adding the addressing method of the register to the first additional binary word
                    if there was a source register as well */
                    instruction_image[IC - 1]->word[BINARY_WORD_SIZE - 3].on = binary[BINARY_WORD_SIZE - 1];
                    instruction_image[IC - 1]->word[BINARY_WORD_SIZE - 4].on = binary[BINARY_WORD_SIZE - 2];
                    instruction_image[IC - 1]->word[BINARY_WORD_SIZE - 5].on = binary[BINARY_WORD_SIZE - 3];
                    free(binary_word);
                    binary_word = NULL;
                } else {
                    /* shifting the binary word left by 5 and adding the A,R,E value to it */
                    shiftLeft(binary, BINARY_WORD_SIZE, 5);
                    binary[BINARY_WORD_SIZE - 1] = binary[BINARY_WORD_SIZE - 2] = 0;
                    for (j = 0; j < BINARY_WORD_SIZE; j++) {
                        Bit bit;
                        bit.on = binary[j];
                        binary_word->word[j] = bit;
                    }
                    /* adding the binary word to memory and updating that there was a source register operand */
                    was_source_register = true;
                    instruction_image[IC] = binary_word;
                    IC++;
                }
            } else if (isImmediateAddrMethod(token)) {
                unsigned int *binary = NULL;
                
                bit1.on = bit2.on = 0;
                RETURN_IF(!isLlegalAddressingMethod(is_source, opcode, IMMEDIATE));
                /* checking if the operand is a symbol */
                if (!isStrNumber(token + 1)) {
                    symbol = lookupSymbol(token + 1);
                    WARNING_IF_MSG(symbol == NULL, ERROR_SYMBOL_DO_NOT_EXISTS);
                    RETURN_IF(symbol == NULL);
                    /* getting the symbol value and converting it to binary */
                    binary = itob(symbol->value);
                } else {
                    /* converting the number to binary */
                    binary = itob(atoi(token + 1));
                }
                /* shifting the binary word left by 2 and adding the A,R,E value to it */
                shiftLeft(binary, BINARY_WORD_SIZE, 2);
                binary[BINARY_WORD_SIZE - 1] = binary[BINARY_WORD_SIZE - 2] = 0;
                for (j = 0; j < BINARY_WORD_SIZE; j++) {
                    Bit bit;
                    bit.on = binary[j];
                    binary_word->word[j] = bit;
                }
                free(binary);
                binary = NULL;
                /* adding the additional binary word to memory */
                instruction_image[IC] = binary_word;
                IC++;
            } else if (isFixedIndexAddrMethod(token)) {
                unsigned int *binary = NULL;
                char *index = NULL;

                bit1.on = 1, bit2.on = 0;
                RETURN_IF(!isLlegalAddressingMethod(is_source, opcode, FIXED_INDEX));
                /* extracting the index from the operand ([5] -> 5) */
                index = extractIndex(token);
                /* checking if the index is a symbol */
                if (!isStrNumber(index)) {
                    symbol = lookupSymbol(index);
                    WARNING_IF_MSG(symbol == NULL, ERROR_SYMBOL_DO_NOT_EXISTS);
                    RETURN_IF(symbol == NULL);
                    /* getting the symbol value and converting it to binary */
                    binary = itob(symbol->value);
                } else {
                    /* converting the index to binary */
                    binary = itob(atoi(index));
                }

                /* shifting the binary word left by 2 and adding the A,R,E value to it */
                shiftLeft(binary, BINARY_WORD_SIZE, 2);
                binary[BINARY_WORD_SIZE - 1] = binary[BINARY_WORD_SIZE - 2] = 0;
                for (j = 0; j < BINARY_WORD_SIZE; j++) {
                    Bit bit;
                    bit.on = binary[j];
                    binary_word->word[j] = bit;
                }
                free(binary);
                free(index);
                binary = NULL;
                index = NULL;
                /* adding both the binary word of the symbol and the index value of it */
                instruction_image[IC] = NULL;
                IC++;
                instruction_image[IC] = binary_word;
                IC++;
            } else if (isDirectAddrMethod(token)) {
                bit1.on = 0, bit2.on = 1;
                RETURN_IF(!isLlegalAddressingMethod(is_source, opcode, DIRECT));
                instruction_image[IC] = NULL;
                IC++;
            } else {
                /* if no addressing method found throwing a warning */
                WARNING_IF_MSG(true, ERROR_ADDRESSING_METHOD_DO_NOT_EXISTS);
                return;
            }
            /* setting the first word opcode bits */
            first_word->word[i] = bit1;
            first_word->word[i+1] = bit2;
            i += 2; /* adding two to i for the next bit position */
            token = strtok(NULL, " \t\n\f\r");
            is_source = false;
        }
    }
}

void resetCounters()
{
    IC = 0;
    DC = 0;
}

unsigned int getIC()
{
    return IC;
}

unsigned int getDC()
{
    return DC;
}

void incrementIC(const int amount)
{
    IC = IC + amount;
}

void incrementDC(const int amount)
{
    DC = DC + amount;
}