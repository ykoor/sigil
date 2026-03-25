#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t Rune;

#define QNAN UINT64_C(0x7FF8000000000000)
#define SIGN_BIT UINT64_C(0x8000000000000000)
#define TYPE_BITS UINT64_C(0x0007000000000000)
#define PAYLOAD_BITS UINT64_C(0x0000FFFFFFFFFFFF)

#define POINTER_NAN (QNAN | SIGN_BIT)
#define POINTER_STRING (0x1ULL << 48)
#define POINTER_PAIR (0x2ULL << 48)
#define POINTER_SIGIL (0x3ULL << 48)
#define POINTER_SIGIL_DEF (0x4ULL << 48)

#define RUNE_NAN (QNAN | (0x0ULL << 48))
#define RUNE_FALSE (QNAN | (0x1ULL << 48))
#define RUNE_TRUE (QNAN | (0x2ULL << 48))
#define RUNE_NIL (QNAN | (0x3ULL << 48))

bool is_double(Rune val);
Rune encode_double(double n);
double decode_double(Rune val);

bool is_pointer(Rune val);
Rune encode_pointer(uint64_t type, void *pointer);
void *decode_pointer(Rune val);

bool is_nil(Rune val);
bool is_bool(Rune val);

Rune make_string(const char *text);
char *as_string(Rune val);
bool is_string(Rune val);

Rune make_sigil(const char *text);
char *as_sigil(Rune val);
bool is_sigil(Rune val);
