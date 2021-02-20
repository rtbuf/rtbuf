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

#include <assert.h>
#include <stdio.h>
#include "data.h"
#include "var.h"
#include "rtbuf.h"

s_data_type  g_rtbuf_var_type = {
  sizeof(s_rtbuf_var) * 8,
  DATA_TYPE_BITS
};
s_data_alloc g_rtbuf_var_alloc;
s_rtbuf_var *g_rtbuf_var;

void rtbuf_var_init (void)
{
  data_alloc_init(&g_rtbuf_var_alloc, &g_rtbuf_var_type, RTBUF_VAR_MAX, NULL, NULL);
  g_rtbuf_var = g_rtbuf_var_alloc.mem;
  assert(g_rtbuf_var);
}

s_rtbuf_var * rtbuf_var_new (const char *name)
{
  s_rtbuf_var *v = data_new(&g_rtbuf_var_alloc);
  if (v)
    v->name = symbol_intern(name);
  return v;
}

s_rtbuf_var * rtbuf_var_find (const char *name)
{
  s_rtbuf_var *v = g_rtbuf_var + g_rtbuf_var_alloc.n;
  unsigned i = g_rtbuf_var_alloc.n;
  const char *sym = symbol_find(name);
  if (sym)
    while (v--, i--) {
      assert(v >= g_rtbuf_var);
      if (v->name == sym)
        return v;
    }
  return NULL;
}

s_rtbuf_var * rtbuf_var_rtbuf_set (const char *name, unsigned i)
{
  s_rtbuf_var *v = rtbuf_var_find(name);
  if (!v)
    v = rtbuf_var_new(name);
  if (v) {
    v->type = RTBUF_VAR_RTBUF;
    v->index = i;
  }
  return v;
}

void rtbuf_var_print (const s_rtbuf_var *v)
{
  if (v) {
    printf("%s = ", v->name);
    switch (v->type) {
    case RTBUF_VAR_NULL:
      printf("NULL");
      fflush(stdout);
      break;
    case RTBUF_VAR_RTBUF:
      rtbuf_print_long(v->index);
      break;
    default:
      assert(0);
    }
  }
}
