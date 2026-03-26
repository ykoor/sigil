#include "../sigil/types/futhark.h"
#include "../sigil/types/rune.h"
#include "../sigil/types/sigil.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ERROR_UNBALANCED_PAREN, ERROR_UNBOUND_SYMBOL } ErrorCode;

typedef struct {
    char *src;
    int cursor;
} Lexer;

char lexer_current(Lexer *l) { return l->src[l->cursor]; }
char lexer_advance(Lexer *l) { return l->src[l->cursor++]; }
bool lexer_at_end(Lexer *l) { return l->src[l->cursor] == '\0'; }

void skip_ws(Lexer *l) {
    while (!lexer_at_end(l) && isspace(lexer_current(l)))
        lexer_advance(l);
}

char *collect_text(Lexer *l) {
    int start = l->cursor;
    while (!lexer_at_end(l) && lexer_current(l) != '(' &&
           lexer_current(l) != ')' && !isspace(lexer_current(l)))
        lexer_advance(l);
    int len = l->cursor - start;
    char *text = malloc(len + 1);
    memcpy(text, l->src + start, len);
    text[len] = '\0';
    return text;
}

typedef enum {
    ATOM_TRUE,
    ATOM_FALSE,
    ATOM_NIL,
    ATOM_NUMBER,
    ATOM_SYMBOL
} AtomType;

static AtomType classify_atom(const char *text) {
    if (strcmp(text, "true") == 0)
        return ATOM_TRUE;
    if (strcmp(text, "false") == 0)
        return ATOM_FALSE;
    if (strcmp(text, "nil") == 0)
        return ATOM_NIL;
    char *end;
    strtod(text, &end);
    if (end != text && *end == '\0')
        return ATOM_NUMBER;
    return ATOM_SYMBOL;
}

Rune parse(Lexer *l);

static Rune collect_atom(Lexer *l) {
    char *text = collect_text(l);
    AtomType type = classify_atom(text);
    Rune result;
    switch (type) {
    case ATOM_TRUE:
        result = RUNE_TRUE;
        break;
    case ATOM_FALSE:
        result = RUNE_FALSE;
        break;
    case ATOM_NIL:
        result = RUNE_NIL;
        break;
    case ATOM_NUMBER:
        result = encode_double(strtod(text, NULL));
        break;
    case ATOM_SYMBOL:
        result = make_symbol(text);
        break;
    }
    free(text);
    return result;
}

static Rune collect_sigil(Lexer *l) {
    char *s = collect_text(l);
    Rune symbol = make_symbol(s);
    free(s);
    Rune val = parse(l);
    return make_sigil(symbol, val);
}

static Rune collect_list(Lexer *l) {
    skip_ws(l);
    if (lexer_current(l) == ')') {
        lexer_advance(l);
        return RUNE_NIL;
    }
    Rune val = parse(l);
    Rune tail = collect_list(l);
    return make_futhark(val, tail);
}

Rune parse(Lexer *l) {
    skip_ws(l);
    char c = lexer_current(l);

    if (c == '(') {
        lexer_advance(l);
        return collect_list(l);
    }
    if (c == '@') {
        lexer_advance(l);
        return collect_sigil(l);
    }
    if (c == '"') {
        lexer_advance(l);
        int start = l->cursor;
        while (!lexer_at_end(l) && lexer_current(l) != '"')
            lexer_advance(l);
        int len = l->cursor - start;
        char *text = malloc(len + 1);
        memcpy(text, l->src + start, len);
        text[len] = '\0';
        lexer_advance(l);
        Rune result = make_string(text);
        free(text);
        return result;
    }
    if (c == ';') {
        while (!lexer_at_end(l) && lexer_current(l) != '\n')
            lexer_advance(l);
        return parse(l);
    }

    return collect_atom(l);
}

void print_tree(Rune val, int depth) {
    for (int i = 0; i < depth; i++)
        printf("  ");

    if (is_sigil(val)) {
        Sigil *s = as_sigil(val);
        if (is_nil(s->symbol))
            printf("[sigil] <anon>\n");
        else
            printf("[sigil] @%s\n", as_symbol(s->symbol));
        print_tree(s->val, depth + 1);
    } else if (is_futhark(val)) {
        printf("[futhark]\n");
        Rune cur = val;
        while (is_futhark(cur)) {
            Sigil *f = futhark_first(cur);
            print_tree(make_sigil(f->symbol, f->val), depth + 1);
            Sigil *r = futhark_rest(cur);
            cur = is_nil(r->val) ? RUNE_NIL : make_sigil(r->symbol, r->val);
        }
    } else if (is_symbol(val)) {
        printf("[symbol] %s\n", as_symbol(val));
    } else if (is_string(val)) {
        printf("[string] \"%s\"\n", as_string(val));
    } else if (is_double(val)) {
        printf("[double] %g\n", decode_double(val));
    } else if (val == RUNE_TRUE) {
        printf("[bool] true\n");
    } else if (val == RUNE_FALSE) {
        printf("[bool] false\n");
    } else if (val == RUNE_NIL) {
        printf("[nil]\n");
    }
}

int main() {
    char src[256];
    snprintf(src, sizeof(src),
             "(@foo (1.2 2 3 (@helloooo (\"%s\" \"%s\" \"%s\"))) (@test 1)) "
             "(@cool (1 2 3 4 5))",
             "world", "world", "world");
    Lexer l = {.src = src, .cursor = 0};

    while (!lexer_at_end(&l)) {
        skip_ws(&l);
        if (lexer_at_end(&l))
            break;
        Rune result = parse(&l);
        print_tree(result, 0);
        printf("\n");
    }

    return 0;
}
