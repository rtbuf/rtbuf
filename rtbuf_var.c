#include <assert.h>
#include <stdio.h>
#include "data.h"
#include "rtbuf_var.h"
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
}

s_rtbuf_var * rtbuf_var_new (const char *name)
{
  s_rtbuf_var *v = data_new(&g_rtbuf_var_alloc);
  v->name = name;
  return v;
}

s_rtbuf_var * rtbuf_var_find (const char *name)
{
  s_rtbuf_var *v = g_rtbuf_var + g_rtbuf_var_alloc.n;
  unsigned i = g_rtbuf_var_alloc.n;
  assert(name);
  while (v--, i--) {
    assert(v >= g_rtbuf_var);
    if (v->name == name)
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
  assert(v);
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
