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
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "symbol.h"

char         g_str_start[STRING_MAX];
char        *g_str = g_str_start;
unsigned int g_str_n = 0;
unsigned int g_str_last = 0;

char * g_str_append (const char *str, size_t len)
{
  char *s = 0;
  if (len <= STRING_MAX - g_str_n) {
    s = g_str;
    memcpy(s, str, len);
    g_str_n += len;
    g_str += len;
  }
  return s;
}

void g_str_reset (char *head)
{
  assert(head > g_str_start);
  assert(head <= g_str_start + STRING_MAX);
  g_str = head;
  g_str_n = head - g_str_start;
}

const char  *g_symbols[SYMBOL_MAX];
unsigned int g_symbols_n = 0;

void symbols_init () {
  bzero(g_str_start, sizeof(g_str_start));
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
      g_symbols[i] = g_str_append(name, strlen(name) + 1);
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
