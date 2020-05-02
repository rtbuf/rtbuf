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
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>

s_rtbuf_lib_proc_in rtbuf_signal_square_in[] = {
  { "frequency", RTBUF_SIGNAL_TYPE, 220.0, 0.0, RTBUF_SIGNAL_SAMPLERATE / 2.0 },
  { "amplitude", RTBUF_SIGNAL_TYPE, 1.0, 0.0, 1.0 },
  { "pulse",     RTBUF_SIGNAL_TYPE, 0.5, 0.0, 1.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out rtbuf_signal_square_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

static
double square (double amp, double phase, double pulse)
{
  return phase < pulse ? amp : -amp;
}

int rtbuf_signal_square (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun freq;
  s_rtbuf_signal_fun amp;
  s_rtbuf_signal_fun pulse;
  s_rtbuf_signal_square_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SQUARE_IN_FREQUENCY, &freq);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SQUARE_IN_AMPLITUDE, &amp);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SQUARE_IN_PULSE, &pulse);
  data = (s_rtbuf_signal_square_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq.sample_fun(freq.signal, i);
    double a = amp.sample_fun(amp.signal, i);
    double p = pulse.sample_fun(pulse.signal, i);
    f = max(0.0, f);
    a = max(0.0, a);
    p = clamp(0.0, p, 1.0);
    /* printf(" i=%u freq=%f amp=%f pulse=%f", i, f, a, p); */
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + f, 1.0);
    data->signal[i] = square(a, data->phase, p);
    /* printf(" f=%f a=%f p=%f square=%f", f, a, p, data->samples[i]); */
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

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "square",
    rtbuf_signal_square,
    rtbuf_signal_square_start,
    0,
    rtbuf_signal_square_in,
    rtbuf_signal_square_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;