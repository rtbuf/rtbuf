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
#include <data.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf.h"
#include "symbol.h"

s_data_type g_rtbuf_proc_type = {
  sizeof(s_rtbuf_proc) * 8,
  DATA_TYPE_BITS
};
s_data_alloc g_rtbuf_proc_alloc;
s_rtbuf_proc *g_rtbuf_proc;

void rtbuf_proc_init ()
{
  data_alloc_init(&g_rtbuf_proc_alloc, &g_rtbuf_proc_type,
                  RTBUF_PROC_MAX, 0, 0);
}

int rtbuf_proc_p (s_rtbuf_proc *proc)
{
  return proc && proc->name;
}

s_rtbuf_proc * rtbuf_proc_new ()
{
  s_rtbuf_proc *rf = data_new(&g_rtbuf_proc_alloc);
  return rf;
}

void rtbuf_proc_delete (s_rtbuf_proc *proc)
{
  assert(proc);
  data_delete(&g_rtbuf_proc_alloc, proc);
}

s_rtbuf_proc * rtbuf_proc_find (const char *x)
{
  const char *sym = symbol_find(x);
  if (sym) {
    unsigned int i = 0;
    unsigned int n = g_rtbuf_alloc.n - g_rtbuf_alloc.free_n;
    while (i < g_rtbuf_proc_alloc.n && n > 0) {
      if (rtbuf_proc_p(&g_rtbuf_proc[i])) {
        if (sym == g_rtbuf_proc[i].name)
          return &g_rtbuf_proc[i];
        n--;
      }
      i++;
    }
  }
  return 0;
}

int rtbuf_proc_out_find (s_rtbuf_proc *proc, const char *x)
{
  symbol sym;
  if (x && '0' <= x[0] && x[0] <= '9') {
    int i = atoi(x);
    if (0 <= i && (unsigned int) i < proc->out_n)
      return i;
  }
  if ((sym = symbol_find(x))) {
    unsigned int i = 0;
    while (i < proc->out_n) {
      if (sym == proc->out[i].name)
        return i;
      i++;
    }
  }
  return -1;
}

void rtbuf_proc_print (s_rtbuf_proc *proc)
{
  unsigned int i = 0;
  printf("#<proc %i %s (", proc->lib_proc, proc->name);
  while (i < proc->in_n) {
    if (i > 0)
      fputs(" ", stdout);
    fputs(proc->in[i].name, stdout);
    fputs(":", stdout);
    fputs(proc->in[i].type->name, stdout);
    i++;
  }
  printf(") -> (");
  i = 0;
  while (i < proc->out_n) {
    if (i > 0)
      fputs(" ", stdout);
    fputs(proc->out[i].name, stdout);
    fputs(":", stdout);
    fputs(proc->out[i].type->name, stdout);
    i++;
  }
  fputs(")>", stdout);
}
