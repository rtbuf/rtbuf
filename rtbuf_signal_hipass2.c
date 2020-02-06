/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com> +33614550127
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
#include "rtbuf.h"
#include "rtbuf_signal.h"

int rtbuf_signal_hipass2_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_hipass2_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_hipass2_data*) rtb->data;
  data->x1 = 0;
  data->x2 = 0;
  data->y1 = 0;
  data->y2 = 0;
  return 0;
}

int rtbuf_signal_hipass2 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_hipass2_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_hipass2_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double x = in.sample_fun(in.signal, i);
    double fc = cutoff.sample_fun(cutoff.signal, i);
    double wc = 2.0 * M_PI * fc;
    double wc2 = wc * wc;
    double _2fs = 2.0 * RTBUF_SIGNAL_SAMPLERATE;
    double k = M_SQRT2 * _2fs * wc;
    double _4fs2 = _2fs * _2fs;
    data->signal[i] = (_4fs2 * (x
                                + data->x1 * -2.0
                                + data->x2)
                       + data->y1 * (2.0 * _4fs2 - 2.0 * wc2)
                       + data->y2 * (k - (_4fs2 + wc2)))
      / (wc2 + k + _4fs2);
    data->x2 = data->x1;
    data->x1 = x;
    data->y2 = data->y1;
    data->y1 = data->signal[i];
    i++;
  }
  return 0;
}
