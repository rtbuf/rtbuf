/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 * Copyright 2020 Judy Najnudel <judy.najnudel@gmail.com>
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
#include <rtbuf/signal.h>

int rtbuf_signal_sawtooth_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_sawtooth_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_sawtooth_data*) rtb->data;
  data->phase = 0;
  return 0;
}

int rtbuf_signal_sawtooth (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun freq;
  s_rtbuf_signal_fun amp;
  s_rtbuf_signal_sawtooth_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SAWTOOTH_IN_FREQUENCY, &freq);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SAWTOOTH_IN_AMPLITUDE, &amp);
  data = (s_rtbuf_signal_sawtooth_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq.sample_fun(freq.signal, i);
    double a = amp.sample_fun(amp.signal, i);
    f = max(0.0, f);
    a = max(0.0, a);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + f, 1.0);
    data->signal[i] = a * (2.0 * data->phase - 1.0);
    i++;
  }
  return 0;
}
