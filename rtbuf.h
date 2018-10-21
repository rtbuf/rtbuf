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
#ifndef RTBUF_H
#define RTBUF_H

#include <data.h>
#include "rtbuf_defs.h"
#include "rtbuf_type.h"
#include "rtbuf_proc.h"

#define RTBUF_SORT   0x0001
#define RTBUF_DELETE 0x0002

struct rtbuf_binding
{
  int rtb;
  unsigned int out;
};

struct rtbuf
{
  void           *data;
  unsigned int    flags;
  s_rtbuf_proc   *proc;
  unsigned int    refc;
  s_rtbuf_binding var[RTBUF_PROC_IN_MAX];
  unsigned int    var_n;
};

#define RTBUF_MAX 1000

s_data_alloc g_rtbuf_alloc;
s_rtbuf     *g_rtbuf;
int          g_rtbuf_run;

int   librtbuf_init ();

int   rtbuf_err (const char *msg);
int   rtbuf_new (s_rtbuf_proc *rp);
void  rtbuf_var_unbind (s_rtbuf *rtb, unsigned int var);
void  rtbuf_unbind_all (s_rtbuf *rtb);
void  rtbuf_delete (s_rtbuf *rtb);
int   rtbuf_clone (s_rtbuf *rtb);
int   rtbuf_find (symbol sym);
int   rtbuf_var_find (s_rtbuf *rtb, const char *x);
void  rtbuf_bind (unsigned int src, unsigned int out,
                  s_rtbuf *dest, unsigned int var);
int   rtbuf_out_find (s_rtbuf *rtb, symbol sym);
int   rtbuf_data_set (s_rtbuf *rtb, symbol name, void *value,
                      unsigned int size);
void  rtbuf_sort ();
void  rtbuf_start ();
void  rtbuf_run ();
void  rtbuf_stop ();
void  rtbuf_print (unsigned int i);
void  rtbuf_print_long (unsigned int i);
void  rtbuf_print_sorted ();

int  rtbuf_out_int (s_rtbuf *rtb, unsigned int out, int default_value);

static inline
double min (double a, double b)
{
  return a < b ? a : b;
}

static inline
double max (double a, double b)
{
  return a < b ? b : a;
}

static inline
double clamp (double inf, double x, double sup)
{
  return max(inf, min(x, sup));
}

#endif /* RTBUF_H */
