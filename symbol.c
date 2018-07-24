
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "symbol.h"

s_symbol g_symbols[SYMBOL_MAX];
unsigned int g_symbols_n = 0;

void init_symbols () {
  bzero(g_symbols, sizeof(g_symbols));
}

int find_symbol (const char *name)
{
  unsigned int i = 0;
  unsigned int n = g_symbols_n;
  while (i < SYMBOL_MAX && n > 0) {
    if (g_symbols[i].name) {
      if (strcmp(name, g_symbols[i].name) == 0)
        return i;
      n--;
    }
    i++;
  }
  return -1;
}

int make_symbol (const char *name)
{
  unsigned int i = 0;
  if (g_symbols_n == SYMBOL_MAX) {
    fprintf(stderr, "maximum number of symbols exceeded\n");
    return -1;
  }
  while (i < SYMBOL_MAX) {
    if (g_symbols[i].name == 0) {
      g_symbols[i].name = name;
      g_symbols_n++;
      return i;
    }
    i++;
  }
  return -1;
}

int intern (const char *name)
{
  int i = find_symbol(name);
  if (i < 0)
    i = make_symbol(name);
  return i;
}
