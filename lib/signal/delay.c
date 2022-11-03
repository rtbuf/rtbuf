/* c3
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */

#include <stdio.h>
#include <strings.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_delay_in[] =
  {{ "signal",   RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "delay",    RTBUF_SIGNAL_TYPE, 0.4, 0.0, RTBUF_SIGNAL_DELAY_MAX, 1.0 },
   { "feedback", RTBUF_SIGNAL_TYPE, 0.5, 0.0, 1.0, 1.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_delay_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "in", RTBUF_SIGNAL_DELAY_TYPE },
   { "pos", "unsigned int" },
   { 0, 0 }};

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

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "delay",
    rtbuf_signal_delay,
    rtbuf_signal_delay_start,
    0,
    rtbuf_signal_delay_in,
    rtbuf_signal_delay_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
