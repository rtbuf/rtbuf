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

#include <math.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_lowpass3_in[] =
  {{ "signal", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "cutoff", RTBUF_SIGNAL_TYPE, 400.0, 1.0, RTBUF_SIGNAL_SAMPLERATE / 2.0, 2.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_lowpass3_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "x1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { 0, 0 }};

int rtbuf_signal_lowpass3_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_lowpass3_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_lowpass3_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->x3 = 0.0;
  data->y1 = 0.0;
  data->y2 = 0.0;
  data->y3 = 0.0;
  return 0;
}

int rtbuf_signal_lowpass3 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_lowpass3_data *data;
  unsigned int i = 0;
  const double fs = RTBUF_SIGNAL_SAMPLERATE;
  const double fs2 = fs * fs;
  const double fs3 = fs2 * fs;
  const double _8fs3 = 8.0 * fs3;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_lowpass3_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double wc = 2.0 * M_PI * fc;
    const double wc2 = wc * wc;
    const double wc3 = wc2 * wc;
    const double _4fs2wc = 4.0 * fs2 * wc;
    const double _8fs2wc = 2.0 * _4fs2wc;
    const double _4fswc2 = 4.0 * fs * wc2;
    const double _3wc3 = 3.0 * wc3;
    const double a = wc3 + _4fswc2 + _8fs2wc + _8fs3;
    const double b = _8fs2wc + 24.0 * fs3 - (_3wc3 + _4fswc2);
    const double c = _4fswc2 + _8fs2wc - (24.0 * fs3 + _3wc3);
    const double d = _8fs3 + _4fswc2 - (wc3 + _8fs2wc);
    data->signal[i] = (wc3 * (x
                              + 3.0 * (data->x1 +
                                       data->x2)
                              + data->x3)
                       + b * data->y1
                       + c * data->y2
                       + d * data->y3) / a;
    data->x3 = data->x2;
    data->x2 = data->x1;
    data->x1 = x;
    data->y3 = data->y2;
    data->y2 = data->y1;
    data->y1 = data->signal[i];
    i++;
  }
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "lowpass3",
    rtbuf_signal_lowpass3,
    rtbuf_signal_lowpass3_start,
    0,
    rtbuf_signal_lowpass3_in,
    rtbuf_signal_lowpass3_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
