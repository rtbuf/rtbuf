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

#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"

int rtbuf_signal_delay_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_delay_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_delay_data*) rtb->data;
  bzero(data->in, sizeof(data->in));
  data->pos = 0;
  return 0;
}

int rtbuf_signal_delay (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun delay;
  s_rtbuf_signal_fun feedback;
  s_rtbuf_signal_delay_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_DELAY_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_DELAY_IN_DELAY, &delay);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_DELAY_IN_DELAY, &feedback);
  data = (s_rtbuf_signal_delay_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double s = in.sample_fun(in.signal, i);
    double d = min(max(0.0, delay.sample_fun(delay.signal, i)),
                   RTBUF_SIGNAL_DELAY_MAX);
    double fb = min(max(0.0, feedback.sample_fun(feedback.signal, i)), 1.0);
    unsigned int ds = d * RTBUF_SIGNAL_SAMPLERATE;
    unsigned int p = (data->pos + RTBUF_SIGNAL_DELAY_SAMPLES_MAX - ds) %
      RTBUF_SIGNAL_DELAY_SAMPLES_MAX;
    data->signal[i] = data->in[p];
    data->in[data->pos] = (1.0 - fb) * s + fb * data->in[p];
    data->pos = (data->pos + 1) % RTBUF_SIGNAL_DELAY_SAMPLES_MAX;
    i++;
  }
  return 0;
}
