#ifndef RUNE_H
#define RUNE_H

#include <assert.h>
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

#define POINTER_STRING (UINT64_C(0x1) << 48)
#define POINTER_PAIR (UINT64_C(0x2) << 48)
#define POINTER_SYMBOL (UINT64_C(0x3) << 48)
#define POINTER_SIGIL (UINT64_C(0x4) << 48)

#define RUNE_NAN (QNAN | (UINT64_C(0x0) << 48))
#define RUNE_FALSE (QNAN | (UINT64_C(0x1) << 48))
#define RUNE_TRUE (QNAN | (UINT64_C(0x2) << 48))
#define RUNE_NIL (QNAN | (UINT64_C(0x3) << 48))

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

Rune make_symbol(const char *text);
char *as_symbol(Rune val);
bool is_symbol(Rune val);

#endif
