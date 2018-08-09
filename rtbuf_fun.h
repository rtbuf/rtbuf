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
#ifndef RTBUF_FUN_H
#define RTBUF_FUN_H

#include "rtbuf_defs.h"
#include "symbol.h"

struct rtbuf_fun_var {
  symbol name;
  s_rtbuf_type *type;
};

struct rtbuf_fun_out {
  symbol name;
  s_rtbuf_type *type;
  unsigned int offset;
};

#define RTBUF_FUN_VAR_MAX 128
#define RTBUF_FUN_OUT_MAX 128

struct rtbuf_fun {
  symbol          name;
  f_rtbuf_fun    *f;
  f_rtbuf_fun    *start;
  f_rtbuf_fun    *stop;
  s_rtbuf_fun_var var[RTBUF_FUN_VAR_MAX];      /* variables */
  unsigned int    var_n;                       /* number of variables */
  s_rtbuf_fun_out out[RTBUF_FUN_OUT_MAX];      /* outputs */
  unsigned int    out_n;                       /* number of outputs */
  unsigned int    out_bytes;  /* size of output data in bytes */
  s_rtbuf_lib    *lib;        /* the library this function comes from */
  unsigned int    lib_fun;    /* the function index in lib functions */
};

#define RTBUF_FUN_MAX 1024
extern s_rtbuf_fun g_rtbuf_fun[RTBUF_FUN_MAX];
extern unsigned int g_rtbuf_fun_n;

void              rtbuf_fun_init ();
int               rtbuf_fun_p (s_rtbuf_fun *fun);
s_rtbuf_fun *     rtbuf_fun_next ();
void              rtbuf_fun_delete (s_rtbuf_fun *fun);
s_rtbuf_fun *     rtbuf_fun_find (const char *x);
s_rtbuf_fun_out * rtbuf_fun_out_find (s_rtbuf_fun *fun,
                                             symbol name);
void              rtbuf_fun_print (s_rtbuf_fun *fun);

#endif
