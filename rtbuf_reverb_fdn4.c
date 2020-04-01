/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
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

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include "hadamard.h"
#include "rtbuf.h"
#include "rtbuf_reverb.h"

int rtbuf_reverb_fdn4_start (s_rtbuf *rtb)
{
  s_rtbuf_reverb_fdn4_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_reverb_fdn4_data*) rtb->data;
  bzero(data, sizeof(*data));
  return 0;
}

int rtbuf_reverb_fdn4 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun signal;
  s_rtbuf_signal_fun time;
  s_rtbuf_signal_fun dry_level;
  s_rtbuf_signal_fun wet_level;
  s_rtbuf_reverb_fdn4_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_REVERB_FDN4_IN_SIGNAL, &signal);
  rtbuf_signal_fun(rtb, RTBUF_REVERB_FDN4_IN_TIME, &time);
  rtbuf_signal_fun(rtb, RTBUF_REVERB_FDN4_IN_DRY_LEVEL, &dry_level);
  rtbuf_signal_fun(rtb, RTBUF_REVERB_FDN4_IN_WET_LEVEL, &wet_level);
  data = (s_rtbuf_reverb_fdn4_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = signal.sample_fun(signal.signal, i);
    const double t = max(0.0, time.sample_fun(time.signal, i));
    const double dl = dry_level.sample_fun(dry_level.signal, i);
    const double wl = wet_level.sample_fun(wet_level.signal, i);
    const double r = 0.0 * t;
    data->signal[i] = dl * x + wl * r;
    i++;
  }
  return 0;
}
