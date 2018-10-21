/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 * Copyright 2018 Judy Najnudel <judy.najnudel@gmail.com>
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
#include "rtbuf.h"
#include "rtbuf_signal.h"

int rtbuf_signal_sinus (s_rtbuf *rtb)
{
  s_rtbuf_signal_proc freq;
  s_rtbuf_signal_proc amp;
  s_rtbuf_signal_sinus_data *data;
  unsigned int i = 0;
  rtbuf_signal_proc(rtb, RTBUF_SIGNAL_SINUS_IN_FREQUENCY, &freq,
                    &g_rtbuf_signal_default_frequency);
  rtbuf_signal_proc(rtb, RTBUF_SIGNAL_SINUS_IN_AMPLITUDE, &amp,
                    &g_rtbuf_signal_sample_one);
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq.sample_proc(freq.signal, i);
    double a = amp.sample_proc(amp.signal, i);
    f = max(0.0, f);
    a = max(0.0, a);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + 2.0 * M_PI * f, 2.0 * M_PI);
    data->signal[i] = a * sin(data->phase);
    //printf(" i=%u f=%f a=%f %f", i, f, a, data->samples[i]);
    i++;
  }
  return 0;
}

int rtbuf_signal_sinus_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_sinus_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  data->phase = 0;
  return 0;
}
