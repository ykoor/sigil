#include "rune.h"
#include "sigil.h"

Rune make_symbol_def(Rune name, Rune val) {
    Sigil *def = malloc(sizeof(Sigil));
    def->name = name;
    def->val = val;
    return encode_pointer(POINTER_ATOM_DEF, def);
}

Sigil *as_sigil(Rune val) { return (Sigil *)decode_pointer(val); }
bool is_symbol_def(Rune val) {
    return is_pointer(val) && (val & TYPE_BITS) == POINTER_ATOM_DEF;
}
