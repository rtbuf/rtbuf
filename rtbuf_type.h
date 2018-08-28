/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef RTBUF_TYPE_H
#define RTBUF_TYPE_H

#include <data.h>
#include "rtbuf_defs.h"
#include "symbol.h"

struct rtbuf_type {
  symbol       name;
  s_data_type  t;
  s_data_alloc alloc;
};

#define RTBUF_TYPE_MAX 1024

void           rtbuf_type_init ();
s_rtbuf_type * rtbuf_type_new (const char *name,
                               unsigned int size);
void           rtbuf_type_delete (s_rtbuf_type *rt);
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
