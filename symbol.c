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

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "symbol.h"

char         g_string[STRING_MAX];
unsigned int g_string_n = 0;

const char  *g_symbols[SYMBOL_MAX];
unsigned int g_symbols_n = 0;

void symbols_init () {
  bzero(g_string, sizeof(g_string));
  bzero(g_symbols, sizeof(g_symbols));
}

symbol symbol_new (const char *name)
{
  unsigned int i = 0;
  if (g_symbols_n == SYMBOL_MAX) {
    fprintf(stderr, "maximum number of symbols exceeded\n");
    return 0;
  }
  while (i < SYMBOL_MAX) {
    if (g_symbols[i] == 0) {
      const char *in = name;
      char *out = &g_string[g_string_n];
      g_symbols[i] = out;
      while (*in) {
        *out++ = *in++;
        g_string_n++;
      }
      *out = 0;
      g_string_n++;
      g_symbols_n++;
      return g_symbols[i];
    }
    i++;
  }
  return 0;
}

symbol symbol_find (const char *name)
{
  unsigned int i = 0;
  unsigned int n = g_symbols_n;
  while (i < SYMBOL_MAX && n > 0) {
    if (g_symbols[i]) {
      if (name == g_symbols[i] ||
          strcmp(name, g_symbols[i]) == 0)
        return g_symbols[i];
      n--;
    }
    i++;
  }
  return 0;
}

symbol symbol_intern (const char *name)
{
  symbol sym = symbol_find(name);
  if (sym == 0)
    sym = symbol_new(name);
  return sym;
}
