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
/**
 * @file
 * @brief Rtbuf library header.
 *
 * Include <rtbuf/lib.h> to manage rtbuf libraries.
 *
 * Use rtbuf_lib_load to load a library, and rtbuf_lib_delete to
 * free it.
 *
 * @see rtbuf_lib_load
 * @see rtbuf_lib_delete
 */
#ifndef RTBUF_LIB_H
#define RTBUF_LIB_H

#include "defs.h"

/**
 * @brief Maximum number of loaded libraries.
 */
#define RTBUF_LIB_MAX 1000

/**
 * @brief Version for library compatibility.
 */
#define RTBUF_LIB_VER 0x00020001

/**
 * @brief Description of a library's proc input.
 */
struct rtbuf_lib_proc_in {
  const char *name; /**< Name of the input. */
  const char *type; /**< Type of the input (as a string). */
  double def;       /**< Default value for the input. */
  double min;       /**< Minimum value for the input. */
  double max;       /**< Maximum value for the input. */
  double log_base;  /**< Base for ogarithmic scale or 0 for linear. */
};

/**
 * @brief Description of a library's proc output.
 */
struct rtbuf_lib_proc_out {
  const char *name;
  const char *type;
};

/**
 * @brief Description of a library's proc.
 *
 * A proc is like a class for real time buffers. It has a name, a
 * description for the buffers inputs and outputs, and pointers to
 * functions for the buffer's start, run, and stop events.
 */
struct rtbuf_lib_proc {
  const char *name;          /**< Name of the proc. E.g. "sinus" */
  f_rtbuf_proc *f;           /**< The real time buffer's run function. */
  f_rtbuf_proc *start;       /**< The real time buffer's start function. */
  f_rtbuf_proc *stop;        /**< The real time buffer's stop function. */
  s_rtbuf_lib_proc_in *in;   /**< Description for the buffer's inputs. Must end with NULL. */
  s_rtbuf_lib_proc_out *out; /**< Description for the buffer's outputs. Must end with NULL. */
};

struct rtbuf_lib {
  const char *name;
  s_rtbuf_proc *proc;
  void *lib;
  const char *path;
  f_rtbuf_lib_unload *unload;
};

extern char *g_rtbuf_lib_path[];

extern s_data_alloc g_rtbuf_lib_alloc;
extern s_rtbuf_lib *g_rtbuf_lib;

void          rtbuf_lib_delete (s_rtbuf_lib *rl);
void          rtbuf_lib_delete_all ();
int           rtbuf_lib_find (const char *str);
void          rtbuf_lib_init_ ();
void          rtbuf_lib_shutdown ();
s_rtbuf_lib * rtbuf_lib_load (const char *path);
s_rtbuf_lib * rtbuf_lib_new ();
void          rtbuf_lib_print (const s_rtbuf_lib *lib);
void          rtbuf_lib_print_long (unsigned int i);

void rtbuf_lib_proc_clean (s_rtbuf_proc *proc);
void rtbuf_lib_proc_init_in (s_rtbuf_proc *proc,
                             s_rtbuf_lib_proc_in *in);
void rtbuf_lib_proc_init_out (s_rtbuf_proc *proc,
                              s_rtbuf_lib_proc_out *out);
void rtbuf_lib_proc_init (s_rtbuf_proc *proc, s_rtbuf_lib_proc *x);


#endif
