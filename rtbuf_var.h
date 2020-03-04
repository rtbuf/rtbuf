#ifndef RTBUF_VAR_H
#define RTBUF_VAR_H

#include "rtbuf_defs.h"

#define RTBUF_VAR_MAX 1000

struct rtbuf_var {
  e_rtbuf_var_type type;
  const char *name;
  unsigned index;
};

void          rtbuf_var_init (void);
s_rtbuf_var * rtbuf_var_find (const char *name);
s_rtbuf_var * rtbuf_var_rtbuf_set (const char *name, unsigned i);
void          rtbuf_var_print (const s_rtbuf_var *v);

#endif
