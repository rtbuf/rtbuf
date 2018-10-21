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

#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"

static inline
double square (double amp, double phase, double pulse)
{
  return phase < pulse ? amp : -amp;
}

int rtbuf_signal_square (s_rtbuf *rtb)
{
  s_rtbuf_signal_proc freq;
  s_rtbuf_signal_proc amp;
  s_rtbuf_signal_proc pulse;
  s_rtbuf_signal_square_data *data;
  unsigned int i = 0;
  rtbuf_signal_proc(rtb, RTBUF_SIGNAL_SQUARE_IN_FREQUENCY, &freq,
                    &g_rtbuf_signal_default_frequency);
  rtbuf_signal_proc(rtb, RTBUF_SIGNAL_SQUARE_IN_AMPLITUDE, &amp,
                    &g_rtbuf_signal_sample_one);
  rtbuf_signal_proc(rtb, RTBUF_SIGNAL_SQUARE_IN_PULSE, &pulse,
                    &g_rtbuf_signal_sample_half);
  data = (s_rtbuf_signal_square_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq.sample_proc(freq.signal, i);
    double a = amp.sample_proc(amp.signal, i);
    double p = pulse.sample_proc(pulse.signal, i);
    f = max(0.0, f);
    a = max(0.0, a);
    p = clamp(0.0, p, 1.0);
    //printf(" i=%u freq=%f amp=%f pulse=%f", i, f, a, p);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + f, 1.0);
    data->signal[i] = square(a, data->phase, p);
    //printf(" f=%f a=%f p=%f square=%f", f, a, p, data->samples[i]);
    i++;
  }
  return 0;
}

int rtbuf_signal_square_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_square_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_square_data*) rtb->data;
  data->phase = 0;
  return 0;
}
