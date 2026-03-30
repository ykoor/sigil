#include "../lib/todo.h"
#include "types/rune.h"
#include "types/sigil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024
#define FRAME_COUNT 1024

typedef struct NameSpaceEntry {
    const char *name;
    Sigil *value;
    struct NameSpaceEntry *next;
} NameSpaceEntry;

typedef struct NameSpace {
    NameSpaceEntry *entries;
    struct NameSpace *parent;
} NameSpace;

// TODO better names for frame count?
typedef struct Stack {
    Sigil **sigils;
    Rune ip;
    size_t sigil_capacity;
    size_t sigil_count;
    NameSpace **frames;
    size_t frame_count;
} Stack;

typedef enum Instruction {
    INST_INVALID = -1,
    INST_NO_OP = 0,
    INST_PUSH,
    INST_POP,
    INST_JMP,
    INST_JMP_IF,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_DUP,
    INST_SWAP,
    INST_LABEL,
    INST_HALT
} Instruction;

typedef struct {
    const char *name;
    Instruction inst;
} InstructionEntry;

static const InstructionEntry table[] = {
    {"no_op", INST_NO_OP}, {"push", INST_PUSH},     {"pop", INST_POP},
    {"jmp", INST_JMP},     {"jmp_if", INST_JMP_IF}, {"add", INST_ADD},
    {"sub", INST_SUB},     {"mul", INST_MUL},       {"div", INST_DIV},
    {"dup", INST_DUP},     {"swap", INST_SWAP},     {"label", INST_LABEL},
    {"halt", INST_HALT}};

#define TABLE_SIZE (sizeof(table) / sizeof(table[0]))

Instruction translate(const char *t) {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (strcmp(t, table[i].name) == 0)
            return table[i].inst;
    }
    return INST_INVALID;
}

#define NEW_STACK()                                                            \
    ({                                                                         \
        Stack *s = malloc(sizeof(Stack));                                      \
        if (!s) {                                                              \
            fprintf(stderr, "failed to allocate stack\n");                     \
            exit(1);                                                           \
        }                                                                      \
        s->sigils = malloc(sizeof(Sigil *) * STACK_SIZE);                      \
        if (!s->sigils) {                                                      \
            fprintf(stderr, "failed to allocate stack sigils\n");              \
            exit(1);                                                           \
        }                                                                      \
        s->ip = 0;                                                             \
        s->sigil_capacity = STACK_SIZE;                                        \
        s->sigil_count = 0;                                                    \
        s;                                                                     \
    })

#define POP(s)                                                                 \
    ({                                                                         \
        Stack *__s = (s);                                                      \
        stack_underflow(__s, 1);                                               \
        __s->sigils[--__s->sigil_count];                                       \
    })

#define PUSH(s, val)                                                           \
    do {                                                                       \
        stack_overflow(s);                                                     \
        (s)->sigils[(s)->sigil_count++] = (val);                               \
    } while (0)

#define PEEK(s)                                                                \
    ({                                                                         \
        Stack *__s = (s);                                                      \
        stack_underflow(__s, 1);                                               \
        __s->sigils[__s->sigil_count - 1];                                     \
    })

#define INC_IP(s) ((s)->ip++)
#define SET_IP(s, addr) ((s)->ip = (addr))

void stack_overflow(Stack *stack) {
    if (stack->sigil_count >= stack->sigil_capacity) {
        fprintf(stderr, "stack overflow\n");
        exit(1);
    }
}

void stack_underflow(Stack *stack, int count) {
    if ((int)stack->sigil_count < count) {
        fprintf(stderr, "stack underflow: need %d, have %zu\n", count,
                stack->sigil_count);
        exit(1);
    }
}

void run_stack_op(Stack *stack, Instruction inst, Sigil *operand) {
    switch (inst) {
    case INST_NO_OP:
        INC_IP(stack);
        break;
    case INST_PUSH:
        PUSH(stack, operand);
        INC_IP(stack);
        break;
    case INST_POP:
        POP(stack);
        INC_IP(stack);
        break;
    case INST_ADD: {
        Sigil *b = POP(stack);
        Sigil *a = POP(stack);
        PUSH(stack, as_sigil(make_sigil(a->symbol, a->val + b->val)));
        INC_IP(stack);
        break;
    }
    case INST_SUB: {
        Sigil *b = POP(stack);
        Sigil *a = POP(stack);
        PUSH(stack, as_sigil(make_sigil(a->symbol, a->val - b->val)));
        INC_IP(stack);
        break;
    }
    case INST_MUL: {
        Sigil *b = POP(stack);
        Sigil *a = POP(stack);
        PUSH(stack, as_sigil(make_sigil(a->symbol, a->val * b->val)));
        INC_IP(stack);
        break;
    }
    case INST_DIV: {
        Sigil *b = POP(stack);
        Sigil *a = POP(stack);
        TODO("1/0 need some error handling");
        PUSH(stack, as_sigil(make_sigil(a->symbol, a->val / b->val)));
        INC_IP(stack);
        break;
    }
    case INST_DUP:
        stack_underflow(stack, 1);
        PUSH(stack, PEEK(stack));
        INC_IP(stack);
        break;
    case INST_SWAP: {
        stack_underflow(stack, 2);
        Sigil *b = POP(stack);
        Sigil *a = POP(stack);
        PUSH(stack, b);
        PUSH(stack, a);
        INC_IP(stack);
        break;
    }

    case INST_JMP: {
        TODO("INST_JMP");
        Sigil *addr = POP(stack);
        TODO("Should we change this just to an int. might be better");
        stack->ip = addr->val;
        break;
    }

    case INST_JMP_IF: {
        TODO("INST_JMP_IF");
        stack_underflow(stack, 2);
        Sigil *addr = POP(stack);
        Sigil *result = POP(stack);

        if (result->val != 0) {
            stack->ip = addr->val;
        } else {
            stack->ip++;
        }
        break;
    }

    case INST_HALT: {
        TODO("INST_HALT");
        break;
    }

    case INST_INVALID: {
        TODO("INST_INVALID");
        break;
    }

    case INST_LABEL: {
        TODO("INST_LABEL");
        break;
    }
    default:
        fprintf(stderr, "unhandled instruction: %d\n", inst);
        exit(1);
    }
}

int main() { NEW_STACK(); }
