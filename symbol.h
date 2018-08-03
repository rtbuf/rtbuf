/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

typedef const char *symbol;

#define STRING_MAX 65536
extern char         g_string[STRING_MAX];
extern unsigned int g_string_n;

#define SYMBOL_MAX 32768
extern symbol       g_symbols[SYMBOL_MAX];
extern unsigned int g_symbols_n;

void   symbols_init ();
symbol symbol_new (symbol name);
void   symbol_delete (symbol sym);
symbol symbol_find (symbol name);
symbol symbol_intern (const char *name);

#endif
