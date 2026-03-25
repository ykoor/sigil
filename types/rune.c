#include "rune.h"

bool is_double(Rune val) { return (val & QNAN) != QNAN; }
Rune encode_double(double n) { return *(Rune *)(&n); }
double decode_double(Rune val) { return *(double *)(&val); }

Rune encode_pointer(uint64_t type, void *pointer) {
    return POINTER_NAN | type | ((uint64_t)(uintptr_t)pointer & PAYLOAD_BITS);
}
void *decode_pointer(Rune val) {
    return (void *)(uintptr_t)(val & PAYLOAD_BITS);
}
bool is_pointer(Rune val) { return !is_double(val) && (val & SIGN_BIT); }

bool is_nil(Rune val) { return val == RUNE_NIL; }
bool is_bool(Rune val) { return val == RUNE_TRUE || val == RUNE_FALSE; }

Rune make_string(const char *text) {
    return encode_pointer(POINTER_STRING, strdup(text));
}
char *as_string(Rune val) { return (char *)decode_pointer(val); }
bool is_string(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_STRING;
}

Rune make_sigil(const char *text) {
    return encode_pointer(POINTER_SIGIL, strdup(text));
}
char *as_sigil(Rune val) { return (char *)decode_pointer(val); }
bool is_sigil(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_SIGIL;
}
