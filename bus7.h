
#ifndef BUS7_H
#define BUS7_H

#include "string7.h"
#include "strings7.h"

#define STRUCT_BUS7(count) struct bus7_ ## count
#define S_BUS7(count) s_bus7_ ## count
#define TYPEDEF_BUS7(count)                     \
  typedef                                       \
  STRUCT_BUS7(count)                            \
       S_BUS7(count);                           \
       
#define STRUCT__BUS7(count)                     \
  STRUCT_BUS7(count)                            \
  {                                             \
    unsigned in;                                \
    unsigned out;                               \
    unsigned len;                               \
    strings7 r[count];                          \
    strings7 e;                                 \
  };                                            \

#define INIT_BUS7(var)                          \
  var.in = var.out = var.len = 0;

#define BUS7_FIRST(count) bus7_ ## count ## _first
#define PROTO_BUS7_FIRST(count)                         \
  strings7 * BUS7_FIRST(count) (S_BUS7(count) *b)
#define DEF_BUS7_FIRST(count)                   \
  PROTO_BUS7_FIRST(count)                       \
  {                                             \
    if (!b || !b->len)                          \
      return 0;                                 \
    return b->r + b->out;                       \
  }                                             \

#define BUS7_LAST(count) bus7_ ## count ## _last
#define PROTO_BUS7_LAST(count)                          \
  strings7 * BUS7_LAST(count) (S_BUS7(count) *b)
#define DEF_BUS7_LAST(count)                    \
  PROTO_BUS7_LAST(count)                        \
  {                                             \
    if (!b || !b->len)                          \
      return 0;                                 \
    return b->r + b->in;                        \
  }                                             \

#define BUS7_LEN(count) bus7_ ## count ## _len
#define PROTO_BUS7_LEN(count)                   \
  unsigned BUS7_LEN(count) (S_BUS7(count) *b)
#define DEF_BUS7_LEN(count)                     \
  PROTO_BUS7_LEN(count)                         \
  {                                             \
    if (!b)                                     \
      return 0;                                 \
    return b->len;                              \
  }                                             \

#define BUS7_NTH(count) bus7_ ## count ## _nth
#define PROTO_BUS7_NTH(count)                                   \
  strings7 * BUS7_NTH(count) (S_BUS7(count) *b, unsigned n)
#define DEF_BUS7_NTH(count)                             \
  PROTO_BUS7_NTH(count)                                 \
  {                                                     \
    strings7 *r = BUS7_FIRST(count)(b);                 \
    if (!r)                                             \
      return 0;                                         \
    unsigned l = count - (r - b->r);                    \
    while (1) {                                         \
      if (l == 0 || *r >= 0 || l < (unsigned) -*r) {    \
        if (b->out < b->in)                             \
          return 0;                                     \
        r = b->r;                                       \
        l = b->in;                                      \
        if (l == 0 || *r >= 0 || l < (unsigned) -*r)    \
          return 0;                                     \
      }                                                 \
      if (n == 0)                                       \
        return r;                                       \
      l += *r;                                          \
      n--;                                              \
      r -= *r;                                          \
    }                                                   \
  }                                                     \

#define BUS7_STRING7(count) bus7_ ## count ## _string7
#define PROTO_BUS7_STRING7(count)                   \
  string7 * BUS7_STRING7(count) (string7 *s, S_BUS7(count) *b)
#define DEF_BUS7_STRING7(count)                                         \
  PROTO_BUS7_STRING7(count)                                             \
  {                                                                     \
    if (!b || !s || !b->len)                                            \
      return 0;                                                         \
    if ((b->out < b->in && b->out + b->r < s && s < b->in + b->r) ||    \
        (b->out >= b->in && ((b->out + b->r < s && s < count + b->r) || \
                             (b->r < s && s < b->in + b->r))))          \
      return s;                                                         \
    strings7 *r = BUS7_FIRST(count)(b);                                 \
    if (!r)                                                             \
      return 0;                                                         \
    unsigned l = count - (r - b->r);                                    \
    while (1) {                                                         \
      if (l == 0 || *r >= 0 || l < (unsigned) -*r) {                    \
        if (b->out < b->in)                                             \
          return 0;                                                     \
        r = b->r;                                                       \
        l = b->in;                                                      \
        if (l == 0 || *r >= 0 || l < (unsigned) -*r)                    \
          return 0;                                                     \
      }                                                                 \
      if (string7_eq(r, s))                                             \
        return r;                                                       \
      l += *r;                                                          \
      r -= *r;                                                          \
    }                                                                   \
  }                                                                     \

#define HEADER_BUS7(count)                      \
  TYPEDEF_BUS7(count)                           \
  STRUCT__BUS7(count)                           \
  PROTO_BUS7_FIRST(count);                      \
  PROTO_BUS7_LAST(count);                       \
  PROTO_BUS7_LEN(count);                        \
  PROTO_BUS7_NTH(count);                        \
  PROTO_BUS7_STRING7(count);

#define CODE_BUS7(count)                        \
  DEF_BUS7_FIRST(count)                         \
  DEF_BUS7_LAST(count)                          \
  DEF_BUS7_LEN(count)                           \
  DEF_BUS7_NTH(count)                           \
  DEF_BUS7_STRING7(count)

HEADER_BUS7(8000)

#endif /* BUS7_H */
