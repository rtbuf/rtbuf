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
#ifndef RTBUF_LIB_H
#define RTBUF_LIB_H

#include <rtbuf/defs.h>

#define RTBUF_LIB_MAX 1000
#define RTBUF_LIB_VER 0x00020001

struct rtbuf_lib_proc_in {
  const char *name;
  const char *type;
  double def;
  double min;
  double max;
};

struct rtbuf_lib_proc_out {
  const char *name;
  const char *type;
};

struct rtbuf_lib_proc {
  const char *name;
  f_rtbuf_proc *f;
  f_rtbuf_proc *start;
  f_rtbuf_proc *stop;
  s_rtbuf_lib_proc_in *in;    /* inputs, end with NULL */
  s_rtbuf_lib_proc_out *out;  /* outputs, end with NULL */
};

struct rtbuf_lib {
  const char *name;
  s_rtbuf_proc *proc;
  void *lib;
  const char *path;
};

extern s_data_alloc g_rtbuf_lib_alloc;
extern s_rtbuf_lib *g_rtbuf_lib;

void          rtbuf_lib_delete (s_rtbuf_lib *rl);
int           rtbuf_lib_find (const char *str);
void          rtbuf_lib_init_ ();
s_rtbuf_lib * rtbuf_lib_load (const char *path);
s_rtbuf_lib * rtbuf_lib_new ();
void          rtbuf_lib_print (const s_rtbuf_lib *lib);
void          rtbuf_lib_print_long (unsigned int i);

void rtbuf_lib_proc_var_init_proc (s_rtbuf_proc *proc,
                                   s_rtbuf_lib_proc_in *in);
void rtbuf_lib_proc_out_init_proc (s_rtbuf_proc *proc,
                                   s_rtbuf_lib_proc_out *out);
void rtbuf_lib_proc_init_proc (s_rtbuf_proc *proc, s_rtbuf_lib_proc *x);

#endif
