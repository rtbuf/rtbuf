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
/**
 * @file
 * @brief rtbuf main header.
 *
 * Main include file for librtbuf. Include this file in your project
 * to use librtbuf.
 *
 * Use librtbuf_init to initialize librtbuf.
 * You can then load libraries using rtbuf_lib_*
 */
#ifndef RTBUF_H
#define RTBUF_H

#include "data.h"
#include "defs.h"
#include "type.h"
#include "proc.h"

/**
 * @brief Rtbuf sort flag. Set if buffer order was changed.
 */
#define RTBUF_SORT     0x0001

/**
 * @brief Rtbuf delete flag. Set if buffer should be deleted.
 */
#define RTBUF_DELETE   0x0002

/**
 * @brief Rtbuf new flag. Set when buffer is new and should be started.
 */
#define RTBUF_NEW      0x0004

/**
 * @brief Rtbuf ready flag. Set when buffer is ready to use.
 */
#define RTBUF_READY    0x0008

/**
 * @brief Binding to an rtbuf output.
 *
 * s_rtbuf_binding is used to bind to an arbitrary output of an
 * arbitrary rtbuf.
 */
struct rtbuf_binding
{
  int rtb;          /**< The source rtbuf index in g_rtbuf. */
  unsigned int out; /**< The source rtbuf output index. */
};

/**
 * @brief Real-time buffer.
 *
 * s_rtbuf is a real-time buffer with a procedure (start, run, stop).
 * Use rtbuf_new or rtbuf_new_ptr to allocate and initialize.
 */
struct rtbuf
{
  void           *data;                  /**< Pointer to buffer memory. */
  unsigned int    flags;                 /**< Flags for buffer state. */
  s_rtbuf_proc   *proc;                  /**< The procedure for the buffer (start, run, stop). */
  unsigned int    refc;                  /**< Reference count. */
  s_rtbuf_binding in[RTBUF_PROC_IN_MAX]; /**< Inputs are bindings to other buffers outputs. */
  unsigned int    in_n;                  /**< Number of inputs. */
  void           *user_ptr;              /**< User pointer. */
};

/**
 * Maximum number of s_rtbuf allocated at the same time globally.
 */
#define RTBUF_MAX         10000

/**
 * Maximum number of s_rtbuf allocated at the same time for each
 * procedure.
 */
#define RTBUF_INSTANCE_MAX  100

/**
 * @brief Real time allocator for g_rtbuf.
 */
extern s_data_alloc g_rtbuf_alloc;

/**
 * @brief Global index of s_rtbuf.
 */
extern s_rtbuf     *g_rtbuf;

/**
 * @brief Run loop should exit when g_rtbuf_run is zero.
 */
extern int          g_rtbuf_run;

/**
 * @brief Is the main loop still active ?
 */
extern int          g_rtbuf_running;

/**
 * @brief Callback function for rtbuf_new.
 */
typedef void (*f_rtbuf_new_cb) (s_rtbuf *rtb);

/**
 * @brief Callback function for rtbuf_delete.
 */
typedef void (*f_rtbuf_delete_cb) (s_rtbuf *rtb);

/**
 * @brief Callback function for rtbuf_bind.
 */
typedef void (*f_rtbuf_bind_cb) (s_rtbuf *src, unsigned int out,
                                 s_rtbuf *dest, unsigned int in);

/**
 * @brief Callback function for rtbuf_unbind.
 */
typedef void (*f_rtbuf_unbind_cb) (s_rtbuf *src, unsigned int out,
                                   s_rtbuf *dest, unsigned int in);

/**
 * @brief Callback function for rtbuf_new.
 */
extern f_rtbuf_new_cb    g_rtbuf_new_cb;

/**
 * @brief Callback function for rtbuf_delete.
 */
extern f_rtbuf_delete_cb g_rtbuf_delete_cb;

/**
 * @brief Callback function for rtbuf_bind.
 */
extern f_rtbuf_bind_cb   g_rtbuf_bind_cb;

/**
 * @brief Callback function for rtbuf_unbind.
 */
extern f_rtbuf_unbind_cb g_rtbuf_unbind_cb;

/**
 * @brief Initialize librtbuf.
 *
 * Should be called before any other rtbuf function.
 *
 * @sa librtbuf_shutdown
 */
int librtbuf_init ();

/**
 * @brief Shutdown librtbuf.
 *
 * Should be called only after any other rtbuf function, when the
 * program shuts down.
 *
 * @sa librtbuf_init
 */
void librtbuf_shutdown ();

int rtbuf_err (const char *msg);

/**
 * @brief Create a new real time buffer.
 *
 * The buffer is allocated in real time and is ready to be bound.
 * This is equivalent to calling rtbuf_new_ptr with user_ptr
 * set to NULL.
 *
 * @param rp The procedure for the new rtbuf.
 * @return The index of the new rtbuf in g_rtbuf.
 */
int rtbuf_new (s_rtbuf_proc *rp);

/**
 * @brief Create a new real time buffer.
 *
 * The buffer is allocated in real time and is ready to be bound.
 * Its user pointer is set to user_ptr.
 *
 * @param rp The procedure for the new rtbuf.
 * @param ptr The user pointer for the new rtbuf.
 * @return The index of the new rtbuf in g_rtbuf.
 */
int   rtbuf_new_ptr (s_rtbuf_proc *rp, void *user_ptr);

void  rtbuf_in_unbind (s_rtbuf *rtb, unsigned int in);
void  rtbuf_out_unbind (s_rtbuf *rtb, unsigned int out);
void  rtbuf_unbind_all (s_rtbuf *rtb);
void  rtbuf_delete (s_rtbuf *rtb);
int   rtbuf_clone (s_rtbuf *rtb);
int   rtbuf_find (symbol sym);
int   rtbuf_in_find (s_rtbuf *rtb, const char *x);
void  rtbuf_bind (unsigned int src, unsigned int out,
                  s_rtbuf *dest, unsigned int in);
int   rtbuf_out_find (s_rtbuf *rtb, symbol sym);
int   rtbuf_data_set (s_rtbuf *rtb, symbol name, void *value,
                      unsigned int size);
void  rtbuf_sort ();
int   rtbuf_start ();
int   rtbuf_run ();
void  rtbuf_stop ();
void  rtbuf_print (unsigned int i);
void  rtbuf_print_long (unsigned int i);
void  rtbuf_print_sorted ();

int  rtbuf_out_int (s_rtbuf *rtb, unsigned int out, int default_value);

double * rtbuf_in_unbound_value (s_rtbuf *rtb, unsigned int in);

double min (double a, double b);
double max (double a, double b);
double clamp (double inf, double x, double sup);

#endif /* RTBUF_H */
