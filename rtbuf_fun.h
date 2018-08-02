#ifndef RTBUF_FUN_H
#define RTBUF_FUN_H

#include "rtbuf_defs.h"

struct rtbuf_fun_var {
  const char *name;     /* symbol */
  s_rtbuf_type *type;
};

struct rtbuf_fun_out {
  const char *name;     /* symbol */
  s_rtbuf_type *type;
  unsigned int offset;
};

#define RTBUF_FUN_VAR_MAX 32
#define RTBUF_FUN_OUT_MAX 32

struct rtbuf_fun {
  const char     *name;                        /* symbol */
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

extern void          rtbuf_fun_init ();
extern int           rtbuf_fun_p (s_rtbuf_fun *fun);
extern s_rtbuf_fun * rtbuf_fun_new (s_rtbuf_lib_fun *x);
extern void          rtbuf_fun_delete (s_rtbuf_fun *fun);
extern s_rtbuf_fun * rtbuf_fun_find (const char *x);

#endif
