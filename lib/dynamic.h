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
#ifndef RTBUF_DYNAMIC_H
#define RTBUF_DYNAMIC_H

#include "signal.h"

enum {
  RTBUF_DYNAMIC_LIMITER_IN_SIGNAL = 0,
  RTBUF_DYNAMIC_LIMITER_IN_GAIN,
  RTBUF_DYNAMIC_LIMITER_IN_TRESHOLD,
  RTBUF_DYNAMIC_LIMITER_IN_ATTACK,
  RTBUF_DYNAMIC_LIMITER_IN_RELEASE,
  RTBUF_DYNAMIC_LIMITER_IN_N
};

typedef struct rtbuf_dynamic_limiter_data {
  t_rtbuf_signal signal;
  t_rtbuf_signal_sample ratio;
} s_rtbuf_dynamic_limiter_data;

#endif
