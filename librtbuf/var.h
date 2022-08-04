/*
 * Copyright 2018-2021 Thomas de Grivel <thoxdg@gmail.com>
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
#ifndef RTBUF_VAR_H
#define RTBUF_VAR_H

#include "defs.h"

#define RTBUF_VAR_MAX 1000

struct rtbuf_var {
  e_rtbuf_var_type type;
  const char *name;
  unsigned index;
};

void          rtbuf_var_init (void);
void          rtbuf_var_shutdown (void);
void          rtbuf_var_delete (s_rtbuf_var *var);
void          rtbuf_var_delete_all ();
s_rtbuf_var * rtbuf_var_find (const char *name);
s_rtbuf_var * rtbuf_var_rtbuf_set (const char *name, unsigned i);
void          rtbuf_var_print (const s_rtbuf_var *v);

#endif
