#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 256
#define MAX_INSTRUCTION_LENGTH 20

typedef struct {
    char *mnemonic;
    unsigned char opcode;
} Instruction;

Instruction instruction_set[] = {
    {"LOAD", 0x01},
    {"ADD",  0x02},
    {"SUB",  0x03},
    {"STORE", 0x04},
    {"HALT", 0xFF}
};

unsigned char memory[256] = {0}; 
int registerA = 0;

unsigned char get_opcode(const char *mnemonic) {
    for (int i = 0; i < sizeof(instruction_set) / sizeof(Instruction); i++) {
        if (strcmp(mnemonic, instruction_set[i].mnemonic) == 0) {
            return instruction_set[i].opcode;
        }
    }
    return 0xFF; 
}

void assemble(const char *code, unsigned char *machine_code, int *length) {
    char line[MAX_INSTRUCTION_LENGTH];
    char mnemonic[MAX_INSTRUCTION_LENGTH];
    int value;

    *length = 0;

    const char *pos = code;
    while (sscanf(pos, "%s %d", mnemonic, &value) == 2 || sscanf(pos, "%s", mnemonic) == 1) {
        unsigned char opcode = get_opcode(mnemonic);
        if (opcode == 0xFF) {
            fprintf(stderr, "Error: Unknown instruction %s\n", mnemonic);
            exit(1);
        }

        machine_code[(*length)++] = opcode;

        if (opcode == 0x01) { 
            machine_code[(*length)++] = (unsigned char)value;
        } else if (opcode == 0x04) { 
            machine_code[(*length)++] = (unsigned char)value;
        }

        while (*pos && *pos != '\n') pos++;
        while (*pos == '\n') pos++;
    }
}

void execute(unsigned char *machine_code, int length) {
    int pc = 0; 

    while (pc < length) {
        unsigned char opcode = machine_code[pc++];

        switch (opcode) {
            case 0x01: 
                registerA = machine_code[pc++];
                break;
            case 0x02: 
                registerA += machine_code[pc++];
                break;
            case 0x03:
                registerA -= machine_code[pc++];
                break;
            case 0x04: 
                memory[machine_code[pc++]] = registerA;
                break;
            case 0xFF: 
                return;
            default:
                fprintf(stderr, "Error: Invalid opcode %02X\n", opcode);
                exit(1);
        }
    }
}

int main() {
    const char *asm_code = 
        "LOAD 5\n"
        "ADD 3\n"
        "STORE 0\n"
        "HALT\n";

    unsigned char machine_code[MAX_CODE_LENGTH];
    int length;

    assemble(asm_code, machine_code, &length);
    
    printf("Machine Code: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", machine_code[i]);
    }
    printf("\n");

    execute(machine_code, length);
    printf("Value in memory[0]: %d\n", memory[0]);

    return 0;
}
