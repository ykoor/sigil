#define STACK_SIZE 1024
#include "types/rune.h"
#include "types/sigil.h"

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
