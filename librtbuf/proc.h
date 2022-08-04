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
#ifndef RTBUF_PROC_H
#define RTBUF_PROC_H

#include "data.h"
#include "defs.h"
#include "symbol.h"

struct rtbuf_proc_in {
  symbol name;
  const char *name_type;
  s_rtbuf_type *type;
  double def;
  double min;
  double max;
  double log_base;
  unsigned int offset;
};

struct rtbuf_proc_out {
  symbol name;
  const char *name_type;
  s_rtbuf_type *type;
  unsigned int offset;
};

#define RTBUF_PROC_IN_MAX 1024
#define RTBUF_PROC_OUT_MAX 1024

struct rtbuf_proc {
  symbol           name;
  f_rtbuf_proc    *f;
  f_rtbuf_proc    *start;
  f_rtbuf_proc    *stop;
  s_rtbuf_proc_in  in[RTBUF_PROC_IN_MAX];    /* inputs */
  unsigned int     in_n;                     /* number of inputs */
  unsigned int     in_bytes;   /* size of input data in bytes */
  s_rtbuf_proc_out out[RTBUF_PROC_OUT_MAX];  /* outputs */
  unsigned int     out_n;                    /* number of outputs */
  unsigned int     out_bytes;  /* size of output data in bytes */
  s_rtbuf_lib     *lib;        /* the library this procedure comes from */
  unsigned int     bytes;      /* size of buffer data in bytes */
  s_data_type      type;
  s_data_alloc     alloc;
};

#define RTBUF_PROC_MAX 1024
extern s_data_alloc  g_rtbuf_proc_alloc;
extern s_rtbuf_proc *g_rtbuf_proc;

void           rtbuf_proc_delete (s_rtbuf_proc *proc);
void           rtbuf_proc_delete_all ();
s_rtbuf_proc * rtbuf_proc_find (const char *x);
void           rtbuf_proc_init ();
s_rtbuf_proc * rtbuf_proc_new ();
int            rtbuf_proc_out_find (s_rtbuf_proc *proc,
                                    const char *name);
int            rtbuf_proc_p (s_rtbuf_proc *proc);
void           rtbuf_proc_print (s_rtbuf_proc *proc);
void           rtbuf_proc_shutdown ();

#endif
