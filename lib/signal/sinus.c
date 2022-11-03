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

#include <float.h>
#include <math.h>
#include <stdio.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_sinus_in[] =
  {{ "frequency", RTBUF_SIGNAL_TYPE, 220.0, 20.0, RTBUF_SIGNAL_SAMPLERATE / 2.0, 2.0 },
   { "amplitude", RTBUF_SIGNAL_TYPE, 1.0, 0.001, 1.0, 10.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_sinus_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "phase", "double" },
   { 0, 0 }};

int rtbuf_signal_sinus_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_sinus_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  data->phase = 0;
  return 0;
}

int rtbuf_signal_sinus (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun freq;
  s_rtbuf_signal_fun amp;
  s_rtbuf_signal_sinus_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SINUS_IN_FREQUENCY, &freq);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_SINUS_IN_AMPLITUDE, &amp);
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq.sample_fun(freq.signal, i);
    double a = amp.sample_fun(amp.signal, i);
    f = max(0.0, f);
    a = max(0.0, a);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    data->phase = fmod(data->phase + 2.0 * M_PI * f, 2.0 * M_PI);
    data->signal[i] = a * sin(data->phase);
    /* printf(" i=%u f=%f a=%f %f", i, f, a, data->samples[i]); */
    i++;
  }
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "sinus", rtbuf_signal_sinus, rtbuf_signal_sinus_start, 0,
    rtbuf_signal_sinus_in, rtbuf_signal_sinus_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
