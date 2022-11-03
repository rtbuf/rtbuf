/* c3
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
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
typedef void f_rtbuf_lib_unload (s_rtbuf_lib *lib);

typedef union rtbuf_lib_init {
  void *ptr;
  f_rtbuf_lib_init *fun;
} u_rtbuf_lib_init;

typedef union rtbuf_lib_unload {
  void *ptr;
  f_rtbuf_lib_unload *fun;
} u_rtbuf_lib_unload;

#endif
