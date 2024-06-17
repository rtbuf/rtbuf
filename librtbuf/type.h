/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef RTBUF_TYPE_H
#define RTBUF_TYPE_H

#include "data.h"
#include "defs.h"
#include "symbol.h"

struct rtbuf_type {
  symbol       name;
  s_data_type  t;
};

#define RTBUF_TYPE_MAX 1024

void           rtbuf_type_init (void);
void           rtbuf_type_shutdown (void);
s_rtbuf_type * rtbuf_type_new (const char *name,
                               unsigned int size);
void           rtbuf_type_delete (s_rtbuf_type *rt);
void           rtbuf_type_delete_all (void);
s_rtbuf_type * rtbuf_type_find (symbol name);
s_rtbuf_type * rtbuf_type_define (const char *name,
                                  unsigned int size);
s_rtbuf_type * rtbuf_type (const char *name);
s_rtbuf_type * rtbuf_type_array (const char *element_type,
                                 unsigned int size);

#define RTBUF_TYPE_CHAR_ARRAY(size) ("char[" #size "]")
#define RTBUF_TYPE_SHORT_ARRAY(size) ("short[" #size "]")
#define RTBUF_TYPE_DOUBLE_ARRAY(size) ("double[" #size "]")

#endif
