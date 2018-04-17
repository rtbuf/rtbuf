
#ifndef BUS8_H
#define BUS8_H

#include <u8.h>

#define STRUCT_ORDERED(count, item) struct ordered_ ## count ## _ ## item
#define S_BUS8(count) s_bus8_ ## count
#define TYPEDEF_BUS8(count)                     \
  typedef                                       \
  STRUCT_BUS8(count)                            \
  {                                             \
    unsigned in;                                \
    unsigned out;                               \
    unsigned len;                               \
    u8 u8[count];                               \
    u8 e;                                       \
  }                                             \
  S_BUS8(count);                                \

#define NEW_BUS8(var)                           \
  var.in = var.out = var.len = 0;

#define IN_BUS8(count) bus8_ ## count ## _in
#define PROTO_IN_BUS8(count)                            \
  u8 * IN_BUS8(count) (S_BUS8(count) *bus, u8 u8)
#define DEF_IN_BUS8(count)                      \
  PROTO_IN_BUS8(count)                          \
  {                                             \
    if (!bus || bus->len == count)              \
      return 0;                                 \
    {                                           \
      unsigned i = bus->in;                     \
      bus->in += 1;                             \
      if (bus->in == count)                     \
        bus->in = 0;                            \
      bus->u8[i] = u8;                          \
      bus->len += 1;                            \
      return bus->u8 + i;                       \
    }                                           \
  }                                             \


#define OUT_BUS8(count) bus8_ ## count ## _out
#define PROTO_OUT_BUS8(count)                   \
  u8 * OUT_BUS8(count) (S_BUS8(count) *bus)
#define DEF_OUT_BUS8(count)                     \
  PROTO_OUT_BUS8(count)                         \
  {                                             \
    if (!bus || bus->len == 0)                  \
      return  0;                                \
    {                                           \
      unsigned o = bus->out;                    \
      bus->out += 1;                            \
      if (bus->out == count)                    \
        bus->out = 0;                           \
      bus->len -= 1;                            \
      return bus->u8 + o;                       \
    }                                           \
  }                                             \


#define HEADER_BUS8(count)                      \
  TYPEDEF_BUS8(count)                           \
  PROTO_IN_BUS8(count);                         \
  PROTO_OUT_BUS8(count);

#define CODE_BUS8(count)                        \
  DEF_IN_BUS8(count)                            \
  DEF_OUT_BUS8(count)

HEADER_BUS8(1000);

typedef S_BUS8(1000) s_bus8;
#define bus8_in(bus, u8) IN_BUS8(1000)(bus, u8)
#define bus8_out(bus) OUT_BUS8(1000)(bus, u8)

#endif /* BUS8_H */
