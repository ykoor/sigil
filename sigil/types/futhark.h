#ifndef FUTHARK_H_
#define FUTHARK_H_
#pragma once

#include "sigil.h"

typedef struct Futhark {
    Sigil first;
    Sigil rest;
} Futhark;

Sigil make_pair(Rune first, Rune rest);
Futhark *as_Futhark(Rune val);
Sigil pair_first(Rune val);
Sigil pair_rest(Rune val);
bool is_pair(Rune val);
#endif // FUTHARK_H_
