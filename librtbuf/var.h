/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef RTBUF_VAR_H
#define RTBUF_VAR_H

#include "defs.h"

#define RTBUF_VAR_MAX 1000

struct rtbuf_var {
  e_rtbuf_var_type type;
  const char *name;
  unsigned index;
};

void          rtbuf_var_init (void);
void          rtbuf_var_shutdown (void);
void          rtbuf_var_delete (s_rtbuf_var *var);
void          rtbuf_var_delete_all ();
s_rtbuf_var * rtbuf_var_find (const char *name);
s_rtbuf_var * rtbuf_var_rtbuf_set (const char *name, unsigned i);
void          rtbuf_var_print (const s_rtbuf_var *v);

#endif
