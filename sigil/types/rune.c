#include "rune.h"
#include <stdio.h>

bool is_double(Rune val) {
    return (val & QNAN) != QNAN;
}

Rune encode_double(double n) {
    Rune val;
    memcpy(&val, &n, sizeof(double));
    return val;
}

double decode_double(Rune val) {
    double n;
    memcpy(&n, &val, sizeof(double));
    return n;
}

bool is_pointer(Rune val) {
    return (val & POINTER_NAN) == POINTER_NAN;
}

Rune encode_pointer(uint64_t type, void *pointer) {
    return POINTER_NAN | type | (uint64_t)(uintptr_t)pointer;
}

void *decode_pointer(Rune val) {
    return (void *)(uintptr_t)(val & PAYLOAD_BITS);
}

bool is_nil(Rune val) {
    return val == RUNE_NIL;
}

bool is_bool(Rune val) {
    return val == RUNE_TRUE || val == RUNE_FALSE;
}

Rune make_string(const char *text) {
    char *copy = malloc(strlen(text) + 1);
    strcpy(copy, text);
    return encode_pointer(POINTER_STRING, copy);
}

char *as_string(Rune val) {
    return (char *)decode_pointer(val);
}

bool is_string(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_STRING;
}

Rune make_symbol(const char *text) {
    char *copy = malloc(strlen(text) + 1);
    strcpy(copy, text);
    return encode_pointer(POINTER_SYMBOL, copy);
}

char *as_symbol(Rune val) {
    return (char *)decode_pointer(val);
}

bool is_symbol(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_SYMBOL;
}
