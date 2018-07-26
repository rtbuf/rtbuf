
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "symbol.h"

s_rtbuf_fun g_rtbuf_fun[RTBUF_FUN_MAX];
unsigned int g_rtbuf_fun_n = 0;

int rtbuf_fun_p (s_rtbuf_fun *fun)
{
  return fun && fun->name;
}

s_rtbuf_fun * find_rtbuf_fun (const char *x)
{
  const char *sym = find_symbol(x);
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

void init_rtbuf_fun (s_rtbuf_fun *fun, s_rtbuf_lib_fun *x)
{
  const char **var = x->var;
  unsigned int j = 0;
  fun->name = intern(x->name);
  fun->f = x->f;
  fun->start = x->start;
  fun->stop = x->stop;
  fun->nmemb = x->nmemb;
  fun->size = x->size;
  fun->var_n = 0;
  if (var)
    while (*var) {
      fun->var_n++;
      var++;
    }
  fun->var = malloc(sizeof(const char *) * (fun->var_n + 1));
  if ((var = x->var))
    while (*var)
      fun->var[j++] = intern(*var++);
  fun->var[j] = 0;
}

s_rtbuf_fun * new_rtbuf_fun (s_rtbuf_lib_fun *x)
{
  unsigned int i = 0;
  if (g_rtbuf_fun_n == RTBUF_FUN_MAX) {
    fprintf(stderr, "RTBUF_FUN_MAX exceeded\n");
    return 0;
  }
  while (i < RTBUF_FUN_MAX) {
    if (!rtbuf_fun_p(&g_rtbuf_fun[i])) {
      init_rtbuf_fun(&g_rtbuf_fun[i], x);
      g_rtbuf_fun_n++;
      return &g_rtbuf_fun[i];
    }
    i++;
  }
  return 0;
}

void delete_rtbuf_fun (s_rtbuf_fun *fun)
{
  if (fun && fun->var) {
    free(fun->var);
    bzero(fun, sizeof(s_rtbuf_fun));
    g_rtbuf_fun_n--;
  }
  fun->name = 0;
}
