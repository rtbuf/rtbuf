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
