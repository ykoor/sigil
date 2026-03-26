#include "sigil.h"

Rune make_sigil(Rune symbol, Rune val) {
    Sigil *s = malloc(sizeof(Sigil));
    s->symbol = symbol;
    s->val = val;
    return encode_pointer(POINTER_SIGIL, s);
}

Sigil *as_sigil(Rune val) { return (Sigil *)decode_pointer(val); }

bool is_sigil(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_SIGIL;
}
