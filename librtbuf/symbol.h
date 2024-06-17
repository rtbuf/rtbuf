/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

#include <sys/types.h>

typedef const char *symbol;

#define STRING_MAX 65536
extern char         g_str_start[STRING_MAX];
extern char        *g_str;
extern unsigned int g_str_n;
extern unsigned int g_str_last;

char * g_str_append (const char *str, size_t len);
void   g_str_reset (char *head);

#define SYMBOL_MAX 32768
extern symbol       g_symbols[SYMBOL_MAX];
extern unsigned int g_symbols_n;

void   symbols_init (void);
symbol symbol_new (symbol name);
void   symbol_delete (symbol sym);
symbol symbol_find (symbol name);
symbol symbol_intern (const char *name);

#endif
