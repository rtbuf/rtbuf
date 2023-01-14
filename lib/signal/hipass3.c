/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
/*

Polynomial for Butterworth third order high pass filter
b: (w/s+1)*((w/s)^2+w/s+1) $

Transfer function
h: 1/b $

Bilinear transform:
s: 2*F*(1-z)/(1+z) $

ratsimp(h);
-(8 F³ z³ - 24 F³ z² + 24 F³ z - 8 F³)
/((w³ - 4 F w² + 8 F² w - 8 F³) z³
  + (3 w³ - 4 F w² - 8 F² w + 24 F³) z²
  + (3 w³ + 4 F w² - 8 F² w - 24 F³) z
  + w³ + 4 F w² + 8 F² w + 8 F³)

a: w³ - 4 F w² + 8 F² w - 8 F³
b: 3 w³ - 4 F w² - 8 F² w + 24 F³
c: 3 w³ + 4 F w² - 8 F² w - 24 F³
d: w³ + 4 F w² + 8 F² w + 8 F³

y: (8 F³ ((x - x3) + 3 (x2 - x1)) - (a y3 + b y2 + c y1)) / d

*/
#include <math.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_hipass3_in[] =
  {{ "signal", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 0.0 },
   { "cutoff", RTBUF_SIGNAL_TYPE, 8000.0, 1.0, RTBUF_SIGNAL_SAMPLERATE / 2.0, 2.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_hipass3_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "x1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { 0, 0 }};

int rtbuf_signal_hipass3_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_hipass3_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_hipass3_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->x3 = 0.0;
  data->y1 = 0.0;
  data->y2 = 0.0;
  data->y3 = 0.0;
  return 0;
}

int rtbuf_signal_hipass3 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_hipass3_data *data;
  unsigned int i = 0;
  const double fs = RTBUF_SIGNAL_SAMPLERATE;
  const double fs2 = fs * fs;
  const double fs3 = fs2 * fs;
  const double _8fs3 = 8.0 * fs3;
  const double _24fs3 = 24.0 * fs3;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_hipass3_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double wc = 2.0 * M_PI * fc;
    const double wc2 = wc * wc;
    const double wc3 = wc2 * wc;
    const double _8fs2wc = 8.0 * fs2 * wc;
    const double _4fswc2 = 4.0 * fs * wc2;
    const double _3wc3 = 3.0 * wc3;
    const double a = wc3 - _4fswc2 + _8fs2wc - _8fs3;
    const double b = _3wc3 - _4fswc2 - _8fs2wc + _24fs3;
    const double c = _3wc3 + _4fswc2 - _8fs2wc - _24fs3;
    const double d = wc3 + _4fswc2 + _8fs2wc + _8fs3;
    data->signal[i] = (_8fs3 * (x
                                + 3.0 * (data->x2
                                         - data->x1)
                                - data->x3)
                       - (a * data->y3
                          + b * data->y2
                          + c * data-> y1)) / d;
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
  { "hipass3",
    rtbuf_signal_hipass3,
    rtbuf_signal_hipass3_start,
    0,
    rtbuf_signal_hipass3_in,
    rtbuf_signal_hipass3_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
