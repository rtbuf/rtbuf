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
/* Butterworth lowpass second order filter

B2: s^2+sqrt(2)*s+1 $
H: 1/B2 $

*/
#include <math.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"

int rtbuf_signal_lowpass2_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_lowpass2_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_lowpass2_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->y1 = 0.0;
  data->y2 = 0.0;
  return 0;
}

int rtbuf_signal_lowpass2 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_lowpass2_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_lowpass2_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double x = in.sample_fun(in.signal, i);
    double fc = cutoff.sample_fun(cutoff.signal, i);
    double k = RTBUF_SIGNAL_SAMPLERATE / (M_PI * fc);
    double k2 = k * k;
    double k_sqrt2 = M_SQRT2 * k;
    double z = 1.0 / (k2 + k_sqrt2 + 1);
    data->signal[i] = z * (x
                           + data->x1 * 2.0
                           + data->x2
                           + data->y1 * 2.0 * (k2 - 1.0)
                           + data->y2 * (k_sqrt2 - (k2 + 1.0)));
    data->x2 = data->x1;
    data->x1 = x;
    data->y2 = data->y1;
    data->y1 = data->signal[i];
    i++;
  }
  return 0;
}
