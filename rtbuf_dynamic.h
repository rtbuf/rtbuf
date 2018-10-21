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
#ifndef RTBUF_DYNAMIC_H
#define RTBUF_DYNAMIC_H

#include "rtbuf_signal.h"

enum {
  RTBUF_DYNAMIC_LIMITER_IN_SIGNAL = 0,
  RTBUF_DYNAMIC_LIMITER_IN_GAIN,
  RTBUF_DYNAMIC_LIMITER_IN_TRESHOLD,
  RTBUF_DYNAMIC_LIMITER_IN_LIMIT,
  RTBUF_DYNAMIC_LIMITER_IN_ATTACK,
  RTBUF_DYNAMIC_LIMITER_IN_RELEASE,
  RTBUF_DYNAMIC_LIMITER_IN_N
};

typedef struct rtbuf_dynamic_limiter_data {
  t_rtbuf_signal signal;
  t_rtbuf_signal_sample gain;
} s_rtbuf_dynamic_limiter_data;

int rtbuf_dynamic_limiter (s_rtbuf *rtb);
int rtbuf_dynamic_limiter_start (s_rtbuf *rtb);

#endif
