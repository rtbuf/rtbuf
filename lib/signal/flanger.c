/*
 * Copyright 2018,2020 Thomas de Grivel <thoxdg@gmail.com> +33614550127
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
#include <string.h>
#include <strings.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>

s_rtbuf_lib_proc_in rtbuf_signal_flanger_in[] =
  {{ "signal",    RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "frequency", RTBUF_SIGNAL_TYPE, 0.5, 0.01, 20.0, 2.0 },
   { "amplitude", RTBUF_SIGNAL_TYPE, 0.02, 0.01, 1.0, 10.0 },
   { "delay",     RTBUF_SIGNAL_TYPE, 0.01, 0.01, 1.0, 2.0 },
   { "feedback",  RTBUF_SIGNAL_TYPE, 0.01, 0.0, 1.0, 1.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_flanger_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "phase", "double" },
   { "in", RTBUF_SIGNAL_FLANGER_TYPE },
   { "pos", "unsigned int" },
   { "ds", "unsigned int" },
   { 0, 0 }};

int rtbuf_signal_flanger_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_flanger_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_flanger_data*) rtb->data;
  data->phase = 0;
  bzero(data->in, sizeof(data->in));
  data->pos = 0;
  data->ds = 0;
  return 0;
}

int rtbuf_signal_flanger (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun freq;
  s_rtbuf_signal_fun amp;
  s_rtbuf_signal_fun delay;
  s_rtbuf_signal_fun feedback;
  s_rtbuf_signal_flanger_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_FLANGER_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_FLANGER_IN_FREQUENCY, &freq);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_FLANGER_IN_AMPLITUDE, &amp);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_FLANGER_IN_DELAY, &delay);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_FLANGER_IN_FEEDBACK, &feedback);
  data = (s_rtbuf_signal_flanger_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double s = in.sample_fun(in.signal, i);
    double f = freq.sample_fun(freq.signal, i);
    double a = amp.sample_fun(amp.signal, i);
    double d = delay.sample_fun(delay.signal, i);
    double delay;
    double fb = feedback.sample_fun(feedback.signal, i);
    unsigned int ds;
    unsigned int pos;
    f = max(0.0, f);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + 2.0 * M_PI * f, 2.0 * M_PI);
    delay = a * (sin(data->phase) * 0.5 + 0.5) + d;
    ds = max(0.0, min(delay * RTBUF_SIGNAL_SAMPLERATE,
                      RTBUF_SIGNAL_FLANGER_SAMPLES_MAX));
    pos = (data->pos + RTBUF_SIGNAL_FLANGER_SAMPLES_MAX - ds) %
      RTBUF_SIGNAL_FLANGER_SAMPLES_MAX;
    data->signal[i] = (data->in[pos] + s) / 2.0;
    data->in[data->pos++] = (1.0 - fb) * s + fb * data->in[pos];
    data->pos %= RTBUF_SIGNAL_FLANGER_SAMPLES_MAX;
    i++;
  }
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "flanger",
    rtbuf_signal_flanger,
    rtbuf_signal_flanger_start,
    0,
    rtbuf_signal_flanger_in,
    rtbuf_signal_flanger_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
