#ifndef FUTHARK_H
#define FUTHARK_H

#include "rune.h"
#include "sigil.h"

typedef struct Futhark {
    Sigil *first;
    Sigil *rest;
} Futhark;

Rune make_futhark(Rune first, Rune rest);
Futhark *as_futhark(Rune val);
Sigil *futhark_first(Rune val);
Sigil *futhark_rest(Rune val);
bool is_futhark(Rune val);

#endif
