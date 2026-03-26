#include "futhark.h"
#include <stdlib.h>

Rune make_futhark(Rune first, Rune rest) {
    Futhark *futhark = malloc(sizeof(Futhark));
    Futhark->first = first;
    Futhark->rest = rest;
    return encode_pointer(POINTER_PAIR, futhark);
}

Futhark *as_futhark(Rune val) { return (Futhark *)decode_pointer(val); }
Rune futhark_first(Rune val) { return as_futhark(val)->first; }
Rune futhark_rest(Rune val) { return as_futhark(val)->rest; }
bool is_futhark(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_PAIR;
}
