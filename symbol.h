#ifndef SYMBOL_H
#define SYMBOL_H

typedef const char *symbol;

#define STRING_MAX 65536
extern char         g_string[STRING_MAX];
extern unsigned int g_string_n;

#define SYMBOL_MAX 32768
extern symbol       g_symbols[SYMBOL_MAX];
extern unsigned int g_symbols_n;

void init_symbols ();
symbol find_symbol (symbol name);
symbol make_symbol (symbol name);
symbol intern (symbol name);

#endif
