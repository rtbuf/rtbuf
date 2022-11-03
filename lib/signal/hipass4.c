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
/* Butterworth hipass fourth order filter
a: cos(5*%pi/8) $
b: cos(7*%pi/8) $
s: 2*F*(1-z)/(1+z) $
B4:((w/s)^2-2*(w/s)*a+1)*((w/s)^2-2*w/s*b+1) $
H: 1/B4 $
ratsimp(H);
     4  4       4  3       4  2       4         4
(16 F  z  - 64 F  z  + 96 F  z  - 64 F  z + 16 F )
    4                    3        2          2   2        3         3
/((w  + (4 F b + 4 F a) w  + (16 F  a b + 8 F ) w  + (16 F  b + 16 F  a) w
       4   4       4                    3           3          3
 + 16 F ) z  + (4 w  + (8 F b + 8 F a) w  + ((- 32 F  b) - 32 F  a) w
       4   3       4           2            2   2       4   2
 - 64 F ) z  + (6 w  + ((- 32 F  a b) - 16 F ) w  + 96 F ) z
       4                        3        3         3            4       4
 + (4 w  + ((- 8 F b) - 8 F a) w  + (32 F  b + 32 F  a) w - 64 F ) z + w
                        3        2          2   2
 + ((- 4 F b) - 4 F a) w  + (16 F  a b + 8 F ) w
           3          3            4
 + ((- 16 F  b) - 16 F  a) w + 16 F )

a:   w4 - 4 (a + b) F w3 + (16 a b +  8) f2 w2 - 16 (a + b) f3 w + 16 f4 $
b: 4 w4 - 8 (a + b) F w3                       + 32 (a + b) f3 w - 64 f4 $
c: 6 w4                  - (32 a b + 16) f2 w2                   + 96 f4 $
d: 4 w4 + 8 (a + b) F w3                       - 32 (a + b) f3 w - 64 f4 $
e:   w4 + 4 (a + b) F w3 + (16 a b +  8) f2 w2 + 16 (a + b) f3 w + 16 f4 $

y: f4 (16 x4 - 64 x3 + 96 x2 - 64 x1 + 16 x - (e y4 + d y3 + c y2 + b y1)) / a $
y: f4 (16 (x4 + x) - 64 (x3 + x1) + 96 x2 - (e y4 + d y3 + c y2 + b y1)) / a $

*/
#include <math.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_hipass4_in[] =
  {{ "signal", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "cutoff", RTBUF_SIGNAL_TYPE, 8000.0, 1.0, RTBUF_SIGNAL_SAMPLERATE / 2.0, 2.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_hipass4_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "x1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "x4", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y1", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y2", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y3", RTBUF_SIGNAL_SAMPLE_TYPE },
   { "y4", RTBUF_SIGNAL_SAMPLE_TYPE },
   { 0, 0 }};

int rtbuf_signal_hipass4_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_hipass4_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_hipass4_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->x3 = 0.0;
  data->x4 = 0.0;
  data->y1 = 0.0;
  data->y2 = 0.0;
  data->y3 = 0.0;
  data->y4 = 0.0;
  return 0;
}

int rtbuf_signal_hipass4 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_hipass4_data *data;
  unsigned int i = 0;
  const double a = cos(5.0 * M_PI / 8.0);
  const double b = cos(7.0 * M_PI / 8.0);
  const double f = RTBUF_SIGNAL_SAMPLERATE;
  const double _4a_bf = 4.0 * (a + b) * f;
  const double f2 = f * f;
  const double _16ab_8f2 = (16.0 * a * b + 8.0) * f2;
  const double f3 = f2 * f;
  const double _16a_bf3 = 16.0 * (a + b) * f3;
  const double f4 = f2 * f2;
  const double _16f4 = 16.0 * f4;
  const double _64f4 = 64.0 * f4;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_HIPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_hipass4_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double w = 2.0 * M_PI * fc;
    const double _16a_bf3w = _16a_bf3 * w;
    const double _32a_bf3w = 2.0 * _16a_bf3w;
    const double w2 = w * w;
    const double _16ab_8f2w2 = _16ab_8f2 * w2;
    const double _32ab_16f2w2 = 2 * _16ab_8f2w2;
    const double w3 = w2 * w;
    const double _4a_bfw3 = _4a_bf * w3;
    const double _8a_bfw3 = 2.0 * _4a_bfw3;
    const double w4 = w2 * w2;
    const double _4w4 = 4.0 * w4;
    const double a =       w4 - _4a_bfw3 + _16ab_8f2w2 - _16a_bf3w + _16f4;
    const double b =     _4w4 - _8a_bfw3               + _32a_bf3w - _64f4;
    const double c = 6.0 * w4            - _32ab_16f2w2        + 96.0 * f4;
    const double d =     _4w4 + _8a_bfw3               - _32a_bf3w - _64f4;
    const double e =       w4 + _4a_bfw3 + _16ab_8f2w2 + _16a_bf3w + _16f4;
    data->signal[i] = (f4 * (16.0 * (x + data->x4)
                             - 64.0 * (data->x1 +
                                       data->x3)
                             + 96.0 * data->x2)
                       - (e * data->y4
                          + d * data->y3
                          + c * data->y2
                          + b * data->y1)) / a;
    data->x4 = data->x3;
    data->x3 = data->x2;
    data->x2 = data->x1;
    data->x1 = x;
    data->y4 = data->y3;
    data->y3 = data->y2;
    data->y2 = data->y1;
    data->y1 = data->signal[i];
    i++;
  }
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "hipass4",
    rtbuf_signal_hipass4,
    rtbuf_signal_hipass4_start,
    0,
    rtbuf_signal_hipass4_in,
    rtbuf_signal_hipass4_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
