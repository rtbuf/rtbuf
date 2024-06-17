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
#include <assert.h>
#include <stdio.h>
#include "data.h"
#include "var.h"
#include "rtbuf.h"

s_data_type  g_rtbuf_var_type = {
  sizeof(s_rtbuf_var),
  DATA_TYPE_BYTES
};
s_data_alloc g_rtbuf_var_alloc;
s_rtbuf_var *g_rtbuf_var;

void rtbuf_var_init (void)
{
  data_alloc_init(&g_rtbuf_var_alloc, &g_rtbuf_var_type, RTBUF_VAR_MAX, NULL, NULL);
  g_rtbuf_var = g_rtbuf_var_alloc.mem;
  assert(g_rtbuf_var);
}

void rtbuf_var_shutdown (void)
{
  if (g_rtbuf_var) {
    rtbuf_var_delete_all();
    g_rtbuf_var = 0;
    data_alloc_clean(&g_rtbuf_var_alloc);
  }
}

s_rtbuf_var * rtbuf_var_new (const char *name)
{
  s_rtbuf_var *var = data_new(&g_rtbuf_var_alloc);
  if (var)
    var->name = symbol_intern(name);
  return var;
}

void rtbuf_var_delete (s_rtbuf_var *var)
{
  data_delete(&g_rtbuf_var_alloc, var);
}

void rtbuf_var_delete_all (void)
{
  unsigned i = 0;
  int n = g_rtbuf_var_alloc.n;
  while (i < g_rtbuf_var_alloc.max && n > 0) {
    if (g_rtbuf_var[i].name) {
      rtbuf_var_delete(g_rtbuf_var + i);
      n--;
    }
    i++;
  }
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
      rtbuf_print_long(g_rtbuf + v->index);
      break;
    default:
      assert(0);
    }
  }
}
