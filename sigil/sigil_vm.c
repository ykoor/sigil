#include "types/rune.h"
#include "types/sigil.h"
#define STACK_SIZE 1024

typedef struct Stack {
    Sigil **sigils;
    Rune ip;
    size_t sigil_capacity;
    size_t sigil_count;
} Stack;

typedef enum Instruction {
    INST_NO_OP = 0,
    INST_PUSH,
    INST_POP,
    INST_JMP,
    INST_JMP_IF,
    INST_ADD,
    INST_SUB
} Instruction;

Stack new_stack() {
    return (Stack){
        .sigils = malloc(sizeof(Sigil *) * STACK_SIZE),
        .ip = 0,
        .sigil_capacity = STACK_SIZE,
        .sigil_count = 0,
    };
}
