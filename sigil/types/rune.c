#include "rune.h"

bool is_double(Rune val) { return (val & QNAN) != QNAN; }

Rune encode_double(double n) {
    Rune r;
    memcpy(&r, &n, sizeof r);
    return r;
}

double decode_double(Rune val) {
    double d;
    memcpy(&d, &val, sizeof d);
    return d;
}

bool is_pointer(Rune val) { return !is_double(val) && (val & SIGN_BIT); }

Rune encode_pointer(uint64_t type, void *pointer) {
    assert(((uintptr_t)pointer & ~(uintptr_t)PAYLOAD_BITS) == 0);
    return POINTER_NAN | type | ((uint64_t)(uintptr_t)pointer & PAYLOAD_BITS);
}

void *decode_pointer(Rune val) {
    return (void *)(uintptr_t)(val & PAYLOAD_BITS);
}

bool is_nil(Rune val) { return val == RUNE_NIL; }

bool is_bool(Rune val) { return val == RUNE_TRUE || val == RUNE_FALSE; }

Rune make_string(const char *text) {
    return encode_pointer(POINTER_STRING, strdup(text));
}

char *as_string(Rune val) { return (char *)decode_pointer(val); }

bool is_string(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_STRING;
}

Rune make_symbol(const char *text) {
    return encode_pointer(POINTER_SYMBOL, strdup(text));
}

char *as_symbol(Rune val) { return (char *)decode_pointer(val); }

bool is_symbol(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_SYMBOL;
}
