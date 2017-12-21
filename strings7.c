
#include "strings7.h"

unsigned strings7_len (strings7 *r, unsigned l)
{
  unsigned i = 0;
  while (1) {
    if (l == 0)
      return i;
    if (*r >= 0)
      return i;
    if (l <= (unsigned) -*r - 1)
      return i;
    i = i + 1;
    l = l + *r;
    r = r - *r;
  }
}

strings7 * strings7_string7 (string7 *s, strings7 *r, unsigned l)
{
  if (*s < 0 && r <= s && (unsigned) (s - r - *s) < l)
    return s;
  while (1) {
    if (l == 0 || *r >=0 || l < (unsigned) -*r)
      return 0;
    if (string7_eq(s, r))
      return r;
    l = l + *r;
    r = r - *r;
  }
}

strings7 * strings7_end (strings7 *r, unsigned l)
{
  while (1) {
    if (l == 0)
      return 0;
    if (*r >= 0)
      return r;
    if (l < (unsigned) -*r)
      return 0;
    l = l + *r;
    r = r - *r;
  }
}

strings7 * strings7_in (strings7 *r, unsigned l, string7 *s)
{
  if (*s >= 0)
    return 0;
  unsigned char sl = string7_len(s);
  strings7 *t = strings7_string7(s, r, l);
  if (t)
    return t;
  t = strings7_end(r, l);
  if (!t)
    return 0;
  *t = -sl - 1;
  strings7 *tt = t;
  while (1) {
    if (sl == 0)
      return tt;
    *++t = *++s;
    sl = sl - 1;
  }
}

strings7 * strings7_last (strings7 *r, unsigned l)
{
  strings7 *s = 0;
  while (1) {
    if (l == 0 || *r >= 0 || l < (unsigned) -*r)
      return s;
    s = r;
    l = l + *r;
    r = r - *r;
  }
}

char strings7_out (strings7 *r, unsigned l)
{
  string7 *s = strings7_last(r, l);
  if (!s)
    return 1;
  *s = 0;
  return 0;
}

char strings7_remove (string7 *s, strings7 *r, unsigned l)
{
  strings7 *t = strings7_string7(s, r, l);
  if (!t)
    return 1;
  unsigned d = string7_len((string7*) t);
  strings7 *u = strings7_last(t, l - (t - r));
  unsigned len = u - t - *u - d;
  while (len-- > 0) {
    *t = t[d];
    t++;
  }
  return 0;
}

strings7 * strings7_nth (strings7 *r, unsigned l, unsigned n)
{
  while (1) {
    if (l == 0 || *r >= 0 || l < (unsigned) -*r)
      return 0;
    if (n == 0)
      return r;
    l += *r;
    n--;
    r -= *r;
  }
}
