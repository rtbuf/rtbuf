#ifndef RTBUF_VAR_H
#define RTBUF_VAR_H

#include "rtbuf_defs.h"

enum rtbuf_var_type {
  RTBUF_VAR_NULL  = 0,
  RTBUF_VAR_RTBUF = 1
};

#define RTBUF_VAR_MAX 1000

struct rtbuf_var {
  e_rtbuf_var_type type;
  const char *name;
  unsigned index;
};

void          rtbuf_var_init (void);
s_rtbuf_var * rtbuf_var_rtbuf_set (const char *name, unsigned i);
void          rtbuf_var_print (const s_rtbuf_var *v);

#endif
