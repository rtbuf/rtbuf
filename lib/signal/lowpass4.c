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
/* Butterworth lowpass fourth order filter
a: cos(5*%pi/8) $
b: cos(7*%pi/8) $
s: 2*F*(1-z)/(1+z) $
B4:((s/w)^2-2*(s/w)*a+1)*((s/w)^2-2*s/w*b+1) $
H: 1/B4 $
ratsimp(H);
        4  4      4  3      4  2      4      4
(%o8) (w  z  + 4 w  z  + 6 w  z  + 4 w  z + w )
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

c:  16*F^4+( 16*b+16*a)*F^3*w+(16*a*b  +8)*F^2*w^2+( 4*b+4*a)*F*w^3+  w^4 $
d: -64*F^4+(-32*b-32*a)*F^3*w                     +( 8*b+8*a)*F*w^3+4*w^4 $
e:  96*F^4                   +(-32*a*b-16)*F^2*w^2                  +6*w^4 $
f: -64*F^4+( 32*b+32*a)*F^3*w                     +(-8*b-8*a)*F*w^3+4*w^4 $
g:  16*F^4+(-16*b-16*a)*F^3*w+(16*a*b  +8)*F^2*w^2+(-4*b-4*a)*F*w^3+  w^4 $

c:   16*F^4 + 16*(a+b)*F^3*w +  (16*a*b+8)*F^2*w^2 + 4*(a+b)*F*w^3 +  w^4 $
d: -(64*F^4 + 32*(a+b)*F^3*w)                      + 8*(a+b)*F*w^3 + 4*w^4 $
e:   96*F^4                  -2*(16*a*b+8)*F^2*w^2                + 6*w^4 $
f:  -64*F^4 + 32*(a+b)*F^3*w                       - 8*(a+b)*F*w^3 + 4*w^4 $
g:   16*F^4 - 16*(a+b)*F^3*w +  (16*a*b+8)*F^2*w^2 - 4*(a+b)*F*w^3 +  w^4 $

y: (w4*(x4+4*x3+6*x2+4*x1+x)-(c*y4+d*y3+e*y2+f*y1))/g $
*/
#include <math.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"

s_rtbuf_lib_proc_in rtbuf_signal_lowpass4_in[] =
  {{ "signal", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "cutoff", RTBUF_SIGNAL_TYPE, 400.0, 1.0, RTBUF_SIGNAL_SAMPLERATE / 2.0, 2.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_lowpass4_out[] =
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

int rtbuf_signal_lowpass4_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_lowpass4_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_lowpass4_data*) rtb->data;
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

int rtbuf_signal_lowpass4 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_lowpass4_data *data;
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
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_lowpass4_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double w = 2.0 * M_PI * fc;
    const double _16a_bf3w = _16a_bf3 * w;
    const double _32a_bf3w = 2.0 * _16a_bf3w;
    const double w2 = w * w;
    const double _16ab_8f2w2 = _16ab_8f2 * w2;
    const double w3 = w2 * w;
    const double _4a_bfw3 = _4a_bf * w3;
    const double _8a_bfw3 = 2.0 * _4a_bfw3;
    const double w4 = w2 * w2;
    const double _4w4 = 4.0 * w4;
    const double c =       _16f4 + _16a_bf3w + _16ab_8f2w2 + _4a_bfw3 +   w4;
    const double d =     -(_64f4 + _32a_bf3w)              + _8a_bfw3 + _4w4;
    const double e =   96.0 * f4       - 2.0 * _16ab_8f2w2        + 6.0 * w4;
    const double f =     - _64f4 + _32a_bf3w               - _8a_bfw3 + _4w4;
    const double g =       _16f4 - _16a_bf3w + _16ab_8f2w2 - _4a_bfw3 +   w4;
    data->signal[i] = (w4 * (x
                             + 4.0 * (data->x1 +
                                      data->x3)
                             + 6.0 * data->x2
                             + data->x4)
                       - (c * data->y4
                          + d * data->y3
                          + e * data->y2
                          + f * data->y1)) / g;
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
  { "lowpass4",
    rtbuf_signal_lowpass4,
    rtbuf_signal_lowpass4_start,
    0,
    rtbuf_signal_lowpass4_in,
    rtbuf_signal_lowpass4_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
