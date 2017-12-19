
#ifndef ORD_H
#define ORD_H

#define STRUCT_ORD(item, count) struct ord_ ## item ## _ ## count
#define S_ORD(item, count) s_ord_ ## item ## _ ## count
#define TYPEDEF_ORD(item, count)                \
  typedef                                       \
  STRUCT_ORD(item, count)                       \
  S_ORD(item, count);                           \
  
#define STRUCT__ORD(item, count)                \
  STRUCT_ORD(item, count)                       \
  {                                             \
    unsigned in;                                \
    unsigned out;                               \
    unsigned len;                               \
    item ord[count];                            \
    item e;                                     \
  };                                            \

#define INIT_ORD(var)                           \
  var.in = var.out = var.len = 0;

#define ORD_IN(item, count) ord_ ## item ## _ ## count ## _in
#define PROTO_ORD_IN(item, count)                                       \
  item * ORD_IN(item, count) (S_ORD(item, count) *ord, item t)
#define DEF_ORD_IN(item, count)                 \
  PROTO_ORD_IN(item, count)                     \
  {                                             \
    if (!ord || ord->len == count)              \
      return 0;                                 \
    {                                           \
      unsigned i = ord->in;                     \
      ord->in += 1;                             \
      if (ord->in == count)                     \
        ord->in = 0;                            \
      ord->ord[i] = t;                          \
      ord->len += 1;                            \
      return ord->ord + i;                      \
    }                                           \
  }                                             \


#define ORD_OUT(item, count) ord_ ## item ## _ ## count ## _out
#define PROTO_ORD_OUT(item, count)                      \
  item * ORD_OUT(item, count) (S_ORD(item, count) *ord)
#define DEF_ORD_OUT(item, count)                     \
  PROTO_ORD_OUT(item, count)                         \
  {                                                  \
    if (!ord || ord->len == 0)                       \
      return 0;                                      \
    {                                                \
      unsigned o = ord->out;                         \
      ord->out += 1;                                 \
      if (ord->out == count)                         \
        ord->out = 0;                                \
      ord->len -= 1;                                 \
      return ord->ord + o;                           \
    }                                                \
  }                                                  \


#define HEADER_ORD(item, count)                      \
  TYPEDEF_ORD(item, count)                           \
  STRUCT__ORD(item, count)                           \
  PROTO_ORD_IN(item, count);                         \
  PROTO_ORD_OUT(item, count);

#define CODE_ORD(item, count)                        \
  DEF_ORD_IN(item, count)                            \
  DEF_ORD_OUT(item, count)

HEADER_ORD(char, 8000);

typedef S_ORD(char, 8000) s_ord;
#define ord_in(ord, char) ORD_IN(char, 8000)(ord, char)
#define ord_out(ord) ORD_OUT(char, 8000)(ord)

#endif /* ORD_H */
