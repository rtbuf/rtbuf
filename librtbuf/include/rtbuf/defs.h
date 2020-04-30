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
#ifndef RTBUF_DEFS_H
#define RTBUF_DEFS_H

typedef struct rtbuf              s_rtbuf;
typedef struct rtbuf_binding      s_rtbuf_binding;
typedef struct rtbuf_proc         s_rtbuf_proc;
typedef struct rtbuf_proc_in      s_rtbuf_proc_in;
typedef struct rtbuf_proc_out     s_rtbuf_proc_out;
typedef struct rtbuf_lib          s_rtbuf_lib;
typedef struct rtbuf_lib_proc     s_rtbuf_lib_proc;
typedef struct rtbuf_lib_proc_in  s_rtbuf_lib_proc_in;
typedef struct rtbuf_lib_proc_out s_rtbuf_lib_proc_out;
typedef struct rtbuf_type         s_rtbuf_type;
typedef struct rtbuf_var          s_rtbuf_var;
typedef union rtbuf_var_data      u_rtbuf_var_data;

typedef enum rtbuf_var_type {
  RTBUF_VAR_NULL  = 0,
  RTBUF_VAR_RTBUF = 1
} e_rtbuf_var_type;

typedef int f_rtbuf_proc (s_rtbuf *rtbuf);
typedef int f_rtbuf_lib_init (s_rtbuf_lib *lib);

#endif
