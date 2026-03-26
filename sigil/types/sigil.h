#ifndef SIGIL_H
#define SIGIL_H

#include "rune.h"

typedef struct Sigil {
    Rune symbol;
    Rune val;
} Sigil;

Rune make_sigil(Rune symbol, Rune val);
Sigil *as_sigil(Rune val);
bool is_sigil(Rune val);

#endif
