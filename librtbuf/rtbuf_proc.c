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
#include <stdlib.h>
#include <strings.h>
#include "data.h"
#include "rtbuf.h"
#include "symbol.h"

s_data_type g_rtbuf_proc_type = {
  sizeof(s_rtbuf_proc),
  DATA_TYPE_BYTES
};
s_data_alloc g_rtbuf_proc_alloc;
s_rtbuf_proc *g_rtbuf_proc;

void rtbuf_proc_init ()
{
  data_alloc_init(&g_rtbuf_proc_alloc, &g_rtbuf_proc_type,
                  RTBUF_PROC_MAX, 0, 0);
  g_rtbuf_proc = g_rtbuf_proc_alloc.mem;
  assert(g_rtbuf_proc);
}

void rtbuf_proc_shutdown ()
{
  if (g_rtbuf_proc) {
    rtbuf_proc_delete_all();
    data_alloc_clean(&g_rtbuf_proc_alloc);
    g_rtbuf_proc = 0;
  }
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

void rtbuf_proc_delete_all ()
{
  unsigned i = 0;
  int n = g_rtbuf_proc_alloc.n;
  while (i < g_rtbuf_proc_alloc.max && n > 0) {
    if (g_rtbuf_proc[i].name) {
      rtbuf_proc_delete(g_rtbuf_proc + i);
      n--;
    }
    i++;
  }
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
  printf("#<proc %s (", proc->name);
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
