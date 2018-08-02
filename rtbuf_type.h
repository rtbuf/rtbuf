#ifndef RTBUF_TYPE_H
#define RTBUF_TYPE_H

#include "rtbuf_defs.h"

struct rtbuf_type {
  const char *name;     /* symbol */
  unsigned int size;
};

#define RTBUF_TYPE_MAX 1024

s_rtbuf_type g_rtbuf_type[RTBUF_TYPE_MAX];
unsigned int g_rtbuf_type_n;

void           rtbuf_type_init ();
s_rtbuf_type * rtbuf_type_new (const char *name, unsigned int size);
void           rtbuf_type_delete (s_rtbuf_type *rt);
s_rtbuf_type * rtbuf_type_find (const char *name);
s_rtbuf_type * rtbuf_type (const char *name);
s_rtbuf_type * rtbuf_type_array (const char *element_type,
                                 unsigned int size);

#define RTBUF_TYPE_CHAR_ARRAY(size) ("char[" #size "]")
#define RTBUF_TYPE_SHORT_ARRAY(size) ("short[" #size "]")
#define RTBUF_TYPE_DOUBLE_ARRAY(size) ("double[" #size "]")

#endif
