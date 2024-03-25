#include "../headers/opcode.h"

/* the llegal opodes of the program */
Opcode opcode[NUM_OF_OPCODE] = {
    {"mov", "0000", 0x2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"cmp", "0001", 0x2, {1, 1, 1, 1}, {1, 1, 1, 1}},
    {"add", "0010", 0x2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"sub", "0011", 0x2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"not", "0100", 0x1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"clr", "0101", 0x1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"lea", "0110", 0x2, {0, 1, 1, 0}, {0, 1, 1, 1}},
    {"inc", "0111", 0x1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"dec", "1000", 0x1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"jmp", "1001", 0x1, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"bne", "1010", 0x1, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"red", "1011", 0x1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"prn", "1100", 0x1, {0, 0, 0, 0}, {1, 1, 1, 1}},
    {"jsr", "1101", 0x1, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"rts", "1110", 0x0, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"hlt", "1111", 0x0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

Opcode *lookupOpcode(const char *opcode_name)
{
    int i = 0;
    /* iterating through the opcodes and returning the found one */
    for (i = 0; i < NUM_OF_OPCODE; i++) {
        if (strcmp(opcode[i].name, opcode_name) == 0) {
            return &opcode[i];
        }
    }
    return NULL;
}