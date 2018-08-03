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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf.h"
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

s_rtbuf_fun * rtbuf_fun_next ()
{
  unsigned int i = 0;
  if (g_rtbuf_fun_n == RTBUF_FUN_MAX) {
    fprintf(stderr, "RTBUF_FUN_MAX exceeded\n");
    return 0;
  }
  while (i < RTBUF_FUN_MAX) {
    s_rtbuf_fun *fun = &g_rtbuf_fun[i];
    if (!rtbuf_fun_p(fun)) {
      g_rtbuf_fun_n++;
      return fun;
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

s_rtbuf_fun_out * rtbuf_fun_out_find (s_rtbuf_fun *fun,
                                      symbol name)
{
  unsigned int i = 0;
  while (i < fun->out_n) {
    if (name == fun->out[i].name)
      return &fun->out[i];
    i++;
  }
  return 0;
}
