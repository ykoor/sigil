#include "types/futhark.h"
#include "types/rune.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Value list_of(int count, ...) {
    va_list args;
    va_start(args, count);

    Value *values = malloc(sizeof(Value) * count);
    for (int i = 0; i < count; i++)
        values[i] = va_arg(args, Value);
    va_end(args);

    Value result = VALUE_NIL;
    for (int i = count - 1; i >= 0; i--)
        result = make_pair(values[i], result);

    free(values);
    return result;
}

typedef enum {
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_IDENTIFIER,
    TOKEN_ATOM_DEF,
    TOKEN_VALUE,
    TOKEN_COMMENT,
    TOKEN_EOF
} TokenType;

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
           lexer_current(l) != ')' && !isspace(lexer_current(l))) {
        lexer_advance(l);
    }

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
    ATOM_SYMBOL,
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

static Value collect_atom(Lexer *l) {
    char *text = collect_text(l);
    AtomType type = classify_atom(text);
    Value result;

    switch (type) {
    case ATOM_TRUE:
        result = VALUE_TRUE;
        break;
    case ATOM_FALSE:
        result = VALUE_FALSE;
        break;
    case ATOM_NIL:
        result = VALUE_NIL;
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

Value parse(Lexer *l);

Value collect_symbol(Lexer *l) {
    char *s = collect_text(l);
    Value name = make_symbol(s);
    free(s);
    Value val = parse(l);
    return make_atom_def(name, val);
}

Value collect_list(Lexer *l) {
    skip_ws(l);

    if (lexer_current(l) == ')') {
        lexer_advance(l);
        return VALUE_NIL;
    }

    Value val = parse(l);
    Value tail = collect_list(l);
    return make_pair(val, tail);
}

Value parse(Lexer *l) {
    skip_ws(l);
    char c = lexer_current(l);

    if (c == '(') {
        lexer_advance(l);
        return collect_list(l);
    }
    if (c == '@') {
        lexer_advance(l);
        return collect_symbol(l);
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
        Value result = make_string(text);
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

void print_tree(Value val, int depth) {
    for (int i = 0; i < depth; i++)
        printf("  ");

    if (is_atom_def(val)) {
        AtomDef *def = as_atom_def(val);
        printf("[atom_def] @%s\n", as_symbol(def->name));
        print_tree(def->val, depth + 1);
    } else if (is_symbol(val)) {
        printf("[symbol] %s\n", as_symbol(val));
    } else if (is_string(val)) {
        printf("[string] \"%s\"\n", as_string(val));
    } else if (is_double(val)) {
        printf("[double] %g\n", decode_double(val));
    } else if (val == VALUE_TRUE) {
        printf("[bool] true\n");
    } else if (val == VALUE_FALSE) {
        printf("[bool] false\n");
    } else if (val == VALUE_NIL) {
        printf("[nil]\n");
    } else if (is_pair(val)) {
        printf("[pair]\n");
        Value cur = val;
        while (!is_nil(cur)) {
            print_tree(first(cur), depth + 1);
            cur = rest(cur);
        }
    }
}

static bool is_obj_entry(Value v) {
    if (is_atom_def(v))
        return true;
    if (is_pair(v) && is_atom_def(first(v)) && is_nil(rest(v)))
        return true;
    return false;
}

static Value unwrap_entry(Value v) {
    if (is_atom_def(v))
        return v;
    return first(v);
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
        Value result = parse(&l);
        to_json(result, 0);
        printf("\n");
    }

    return 0;
}
