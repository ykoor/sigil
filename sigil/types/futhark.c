#include "futhark.h"

static Sigil *to_sigil(Rune val) {
    if (is_sigil(val))
        return as_sigil(val);
    return as_sigil(make_sigil(RUNE_NIL, val));
}

Rune make_futhark(Rune first, Rune rest) {
    Futhark *futhark = malloc(sizeof(Futhark));
    futhark->first = to_sigil(first);
    futhark->rest = to_sigil(rest);
    return encode_pointer(POINTER_FUTHARK, futhark);
}

Futhark *as_futhark(Rune val) { return (Futhark *)decode_pointer(val); }

Sigil *futhark_first(Rune val) { return as_futhark(val)->first; }

Sigil *futhark_rest(Rune val) { return as_futhark(val)->rest; }

bool is_futhark(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_FUTHARK;
}
