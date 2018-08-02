
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "symbol.h"

s_rtbuf_fun g_rtbuf_fun[RTBUF_FUN_MAX];
unsigned int g_rtbuf_fun_n = 0;

void rtbuf_fun_init ()
{
  bzero(g_rtbuf_fun, sizeof(g_rtbuf_fun));
}

int rtbuf_fun_p (s_rtbuf_fun *fun)
{
  return fun && fun->name;
}

void rtbuf_fun_new_var (s_rtbuf_fun *fun, s_rtbuf_lib_fun_var *var)
{
  unsigned int i = 0;
  bzero(fun->var, sizeof(fun->var));
  if (var)
    while (var->name) {
      s_rtbuf_fun_var *v = &fun->var[i];
      v->name = symbol_intern(var->name);
      v->type = rtbuf_type(var->type);
      i++;
    }
  fun->var_n = i;
}

void rtbuf_fun_new_out (s_rtbuf_fun *fun, s_rtbuf_lib_fun_out *out)
{
  unsigned int i = 0;
  bzero(fun->out, sizeof(fun->out));
  if (out) {
    unsigned int offset = 0;
    while (out->name) {
      s_rtbuf_fun_out *o = &fun->out[i];
      o->name = symbol_intern(out->name);
      o->type = rtbuf_type(out->type);
      o->offset = offset;
      offset += o->type->size;
      i++;
    }
  }
  fun->out_n = i;
}

s_rtbuf_fun * rtbuf_fun_new (s_rtbuf_lib_fun *x)
{
  unsigned int i = 0;
  if (g_rtbuf_fun_n == RTBUF_FUN_MAX) {
    fprintf(stderr, "RTBUF_FUN_MAX exceeded\n");
    return 0;
  }
  while (i < RTBUF_FUN_MAX) {
    s_rtbuf_fun *fun = &g_rtbuf_fun[i];
    if (!rtbuf_fun_p(fun)) {
      fun->name = symbol_intern(x->name);
      fun->f = x->f;
      fun->start = x->start;
      fun->stop = x->stop;
      rtbuf_fun_new_var(&g_rtbuf_fun[i], x->var);
      rtbuf_fun_new_out(&g_rtbuf_fun[i], x->out);
      g_rtbuf_fun_n++;
      return &g_rtbuf_fun[i];
    }
    i++;
  }
  return 0;
}

void rtbuf_fun_delete (s_rtbuf_fun *fun)
{
  if (fun && fun->name) {
    bzero(fun, sizeof(s_rtbuf_fun));
    g_rtbuf_fun_n--;
  }
}

s_rtbuf_fun * rtbuf_fun_find (const char *x)
{
  const char *sym = symbol_find(x);
  if (sym) {
    unsigned int i = 0;
    unsigned int n = g_rtbuf_fun_n;
    while (i < RTBUF_FUN_MAX && n > 0) {
      if (rtbuf_fun_p(&g_rtbuf_fun[i]) &&
          sym == g_rtbuf_fun[i].name)
        return &g_rtbuf_fun[i];
      i++;
    }
  }
  return 0;
}
