
#include "string7.h"

unsigned char string7_len (string7 *s)
{
  if (!s)
    return 0;
  if (*s < 0)
    return -*s - 1;
  {
    unsigned char l = 0;
    while (*s > 0) {
      l = l + 1;
      if (l > 127)
        return 0;
    }
    return l;
  }
}

unsigned char string7__len (string7 *s)
{
  if (*s < 0)
    return -*s - 1;
  return 127;
}

char string7_p (string7 *s)
{
  unsigned char l;
  if (!s || *s >= 0)
    return 0;
  l = -*s;
  while (l-- > 0)
    if (*s < 0)
      return 0;
  return 1;
}

char string7_eq (string7 *a, string7 *b)
{
  if (*a < 0)
    a++;
  if (*b < 0)
    b++;
  while (1) {
    if (*a != *b)
      return 0;
    if (*a < 0)
      return 1;
    a++;
    b++;
  }
}

char string7_eq_len (string7 *a, string7 *b, unsigned char len)
{
  if (*a < 0)
    a++;
  if (*b < 0)
    b++;
  while (len-- > 0) {
    if (*a < 0 || *b < 0 || *a != *b)
      return 0;
    a++;
    b++;
  }
  return 1;
}

char string7_lt (string7 *a, string7 *b)
{
  if (*a >= 0 || *b >= 0)
    return 0;
  unsigned char l = *a > *b ? -*a - 1 : -*b - 1;
  string7 *pa = a;
  string7 *pb = b;
  while (l-- > 0) {
    pa++;
    pb++;
    if (*pa < *pb)
      return 1;
    if (*pa > *pb)
      return 0;
  }
  return *a > *b ? 1 : 0;
}

char string7_lte (string7 *a, string7 *b)
{
  if (*a >= 0 || *b >= 0)
    return 0;
  unsigned char l = *a > *b ? -*a - 1 : -*b - 1;
  string7 *pa = a;
  string7 *pb = b;
  while (l-- > 0) {
    pa++;
    pb++;
    if (*pa < *pb)
      return 1;
    if (*pa > *pb)
      return 0;
  }
  return *a == *b ? 1 : (*a > *b ? 1 : 0);
}

char string7_gt (string7 *a, string7 *b)
{
  return string7_lt(b, a);
}

char string7_gte (string7 *a, string7 *b)
{
  return string7_lte(b, a);
}

char string7_compare (string7 *a, string7 *b)
{
  if (*a >= 0 || *b >= 0)
    return 0;
  unsigned char l = *a > *b ? -*a - 1 : -*b - 1;
  string7 *pa = a;
  string7 *pb = b;
  while (l-- > 0) {
    pa++;
    pb++;
    if (*pa < *pb)
      return -1;
    if (*pa > *pb)
      return 1;
  }
  return *a == *b ? 0 : (*a > *b ? -1 : 1);
}

char string7_char (char c, string7 *s)
{
  unsigned char l = string7__len(s);
  unsigned char i = 0;
  if (c < 0)
    return -1;
  if (*s < 0)
    s++;
  while (1) {
    if (l == 0 || i == 128)
      return -1;
    if (*s < 0)
      return -1;
    if (*s == c)
      return i;
    i = i + 1;
    l = l - 1;
  }
  return -1;
}

char string7_string7 (string7 *t, string7 *s)
{
  unsigned char l = string7_len(s);
  unsigned char i = 0;
  unsigned char n = string7_len(t);
  if (*s < 0)
    s++;
  if (*t < 0)
    t++;
  if (n == 0) return 0;
  while (1) {
    if (l < n)
      return -1;
    if (string7_eq_len(s, t, n))
      return i;
    s++;
    i++;
    l--;
  }
}

char string7_string7_len (string7 *t, string7 *s, unsigned char l)
{
  unsigned char i = 0;
  unsigned char n = string7_len(t);
  if (*s < 0) {
    if (-*s - 1 < l)
      return -1;
    s++;
  }
  if (*t < 0) {
    if (-*t - 1 < l)
      return -1;
    t++;
  }
  if (n == 0) return 0;
  while (1) {
    if (l < n)
      return -1;
    if (string7_eq_len(s, t, n))
      return i;
    s++;
    i++;
    l--;
  }
}

char string7_nth (string7 *s, unsigned char n)
{
  if (*s < 0)
    return s[-*s];
  while (n-- > 0) {
    if (*s < 0)
      return -1;
    s++;
  }
  return *s;
}
