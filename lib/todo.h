#pragma once

#if !defined(__clang__) && !defined(__GNUC__)
#error "todo.h requires Clang or GCC"
#endif

#include <stdio.h>
#include <stdlib.h>

static inline __attribute__((deprecated("TODO not yet implemented"))) void
_todo_warning(void) {}

#ifdef __clang__
#define _TODO_DIAG_PUSH _Pragma("clang diagnostic push")
#define _TODO_DIAG_WARN                                                        \
    _Pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")
#define _TODO_DIAG_ERROR                                                       \
    _Pragma("clang diagnostic error \"-Wdeprecated-declarations\"")
#define _TODO_DIAG_POP _Pragma("clang diagnostic pop")
#else
#define _TODO_DIAG_PUSH _Pragma("GCC diagnostic push")
#define _TODO_DIAG_WARN                                                        \
    _Pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")
#define _TODO_DIAG_ERROR                                                       \
    _Pragma("GCC diagnostic error \"-Wdeprecated-declarations\"")
#define _TODO_DIAG_POP _Pragma("GCC diagnostic pop")
#endif

/*
 * Flags (pass via -D to the compiler):
 *
 *   (none)          silently ignored           gcc your_file.c
 *   TODO_RUNTIME    crash at runtime           gcc -DTODO_RUNTIME your_file.c
 *   TODO_WARN       compiler warning           gcc -DTODO_WARN your_file.c
 *   TODO_ERROR      compiler error             gcc -DTODO_ERROR your_file.c
 *
 */

#if defined(TODO_ERROR)
#define TODO(msg, ...)                                                         \
    _TODO_DIAG_PUSH                                                            \
    _TODO_DIAG_ERROR                                                           \
    _todo_warning();                                                           \
    _TODO_DIAG_POP

#elif defined(TODO_WARN)
#define TODO(msg, ...)                                                         \
    _TODO_DIAG_PUSH                                                            \
    _TODO_DIAG_WARN                                                            \
    _todo_warning();                                                           \
    _TODO_DIAG_POP

#elif defined(TODO_RUNTIME)
#define TODO(msg, ...)                                                         \
    do {                                                                       \
        fprintf(stderr, "TODO: " msg " (%s:%d)\n", __FILE__, __LINE__);        \
        exit(1);                                                               \
    } while (0)

#else
#define TODO(msg, ...) ((void)0)
#endif
