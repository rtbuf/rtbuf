#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct symbol
{
  const char *name;
} s_symbol;

#define SYMBOL_MAX 32768
s_symbol g_symbols[SYMBOL_MAX];
unsigned int g_symbols_n;

int find_symbol (const char *name);
int make_symbol (const char *name);
int intern (const char *name);

#endif
