
#ifndef STRINGS7_H
#define STRINGS7_H

#include "string7.h"

typedef string7 strings7;

unsigned strings7_len (strings7 *r, unsigned l);
strings7 * strings7_string7 (string7 *s, strings7 *r, unsigned l);
strings7 * strings7_end (strings7 *r, unsigned l);
strings7 * strings7_in (strings7 *r, unsigned l, string7 *s);
strings7 * strings7_last (strings7 *r, unsigned l);

#endif /* STRINGS7_H */
