#ifndef FUTHARK_H_
#define FUTHARK_H_
#pragma once

#include "rune.h"

typedef struct Futhark {
    Rune first;
    Rune rest;
} Futhark;

Rune make_pair(Rune first, Rune rest);
Futhark *as_Futhark(Rune val);
Rune pair_first(Rune val);
Rune pair_rest(Rune val);
bool is_pair(Rune val);
#endif // FUTHARK_H_
