#ifndef SYMBOL_H_
#define SIGIL_H_
#pragma once

#include "rune.h"

typedef struct Sigil {
    Rune name;
    Rune val;
} Sigil;

Rune make_sigil(Rune name, Rune val);
Sigil *as_sigil(Rune val);
bool is_sigil(Rune val);

#endif // SIGIL_H_
