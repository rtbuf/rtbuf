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

#include "rtbuf_defs.h"

#define RTBUF_LIB_MAX 1000
#define RTBUF_LIB_VER 0x00010001

struct rtbuf_lib_fun_var {
  const char *name;
  const char *type;
};

struct rtbuf_lib_fun_out {
  const char *name;
  const char *type;
};

struct rtbuf_lib_fun {
  const char *name;
  f_rtbuf_fun *f;
  f_rtbuf_fun *start;
  f_rtbuf_fun *stop;
  s_rtbuf_lib_fun_var *var;  /* variables, end with NULL */
  s_rtbuf_lib_fun_out *out;  /* outputs, end with NULL */
};

struct rtbuf_lib {
  const char *name;
  s_rtbuf_fun **fun;
  unsigned int fun_n;
  void *lib;
  const char *path;
};

extern s_rtbuf_lib  g_rtbuf_lib[];
extern unsigned int g_rtbuf_lib_n;

void          rtbuf_lib_delete (s_rtbuf_lib *rl);
int           rtbuf_lib_find (const char *str);
int           rtbuf_lib_find_fun (s_rtbuf_lib *rl, const char *str);
void          rtbuf_lib_init_ ();
s_rtbuf_lib * rtbuf_lib_load (const char *path);
s_rtbuf_lib * rtbuf_lib_new ();
void          rtbuf_lib_print (unsigned int i);
void          rtbuf_lib_print_long (unsigned int i);

void rtbuf_lib_fun_var_init_fun (s_rtbuf_fun *fun,
                                 s_rtbuf_lib_fun_var *var);
void rtbuf_lib_fun_out_init_fun (s_rtbuf_fun *fun,
                                 s_rtbuf_lib_fun_out *out);
void rtbuf_lib_fun_init_fun (s_rtbuf_fun *fun, s_rtbuf_lib_fun *x);

#endif
