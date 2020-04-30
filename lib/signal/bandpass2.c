/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com> +33614550127
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
/* Butterworth bandpass second order filter
s: 2*f*(1-z)/(1+z) $
B2: (q*(s/w+w/s))^2+sqrt(2)*(q*(s/w+w/s))+1 $
H: 1/B2 $
ratsimp(H);
           2  2  4      2  2  2      2  2
(%o25) (4 f  w  z  - 8 f  w  z  + 4 f  w )
    2  4    3/2      3       2  2      2   2    7/2  3           4  2   4
/((q  w  - 2    f q w  + (8 f  q  + 4 f ) w  - 2    f  q w + 16 f  q ) z
       2  4    5/2      3    9/2  3           4  2   3
 + (4 q  w  - 2    f q w  + 2    f  q w - 64 f  q ) z
       2  4           2  2       2   2       4  2   2
 + (6 q  w  + ((- 16 f  q ) - 8 f ) w  + 96 f  q ) z
       2  4    5/2      3    9/2  3           4  2       2  4    3/2      3
 + (4 q  w  + 2    f q w  - 2    f  q w - 64 f  q ) z + q  w  + 2    f q w
       2  2      2   2    7/2  3           4  2
 + (8 f  q  + 4 f ) w  + 2    f  q w + 16 f  q )

a:   q2 w4 + sqrt(2)^3 f q w3 +   (8 f2 q2 + 4 f2) w2 + sqrt(2)^7 f3 q w + 16 f4 q2;
b: 4 q2 w4 + sqrt(2)^5 f q w3                         - sqrt(2)^9 f3 q w - 64 f4 q2;
c: 6 q2 w4                    - 2 (8 f2 q2 + 4 f2) w2                    + 96 f4 q2;
d: 4 q2 w4 - sqrt(2)^5 f q w3                         + sqrt(2)^9 f3 q w - 64 f4 q2;
e:   q2 w4 - sqrt(2)^3 f q w3 +   (8 f2 q2 + 4 f2) w2 - sqrt(2)^7 f3 q w + 16 f4 q2;
-----------------------------------------------------------------------------------
a:   q2 w4 + sqrt(2)^3 f q w3 +   (2 q2 + 1) 4 f2 w2 + sqrt(2)^7 f3 q w + 16 f4 q2;
b: 4 q2 w4 + sqrt(2)^5 f q w3                        - sqrt(2)^9 f3 q w - 64 f4 q2;
c: 6 q2 w4                    - 2 (2 q2 + 1) 4 f2 w2                    + 96 f4 q2;
d: 4 q2 w4 - sqrt(2)^5 f q w3                        + sqrt(2)^9 f3 q w - 64 f4 q2;
e:   q2 w4 - sqrt(2)^3 f q w3 +   (2 q2 + 1) 4 f2 w2 - sqrt(2)^7 f3 q w + 16 f4 q2;
-----------------------------------------------------------------------------------
y: (4 f2 w2 (x4 - 2 x2 + x) - (b y1 + c y2 + d y3 + e y4)) / a;
*/
#include <math.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>

s_rtbuf_lib_proc_in rtbuf_signal_bandpass2_in[] = {
  { "signal",   RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { "cutoff",   RTBUF_SIGNAL_TYPE, 400.0, 0.0, RTBUF_SIGNAL_SAMPLERATE / 2.0 },
  { "qfactor",  RTBUF_SIGNAL_TYPE, 1.0, 0.0, 1000.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out rtbuf_signal_bandpass2_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "x1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { 0, 0 } };

int rtbuf_signal_bandpass2_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_bandpass2_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_bandpass2_data*) rtb->data;
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

int rtbuf_signal_bandpass2 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_fun qfactor;
  s_rtbuf_signal_bandpass2_data *data;
  unsigned int i = 0;
  const double sqrt2_3 = sqrt2_2 * M_SQRT2;
  const double sqrt2_5 = sqrt2_3 * sqrt2_2;
  const double sqrt2_7 = sqrt2_5 * sqrt2_2;
  const double sqrt2_9 = sqrt2_7 * sqrt2_2;
  const double f = RTBUF_SIGNAL_SAMPLERATE;
  const double f2 = f * f;
  const double f3 = f2 * f;
  const double f4 = f2 * f2;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_BANDPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_BANDPASS_IN_CUTOFF, &cutoff);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_BANDPASS_IN_QFACTOR, &qfactor);
  data = (s_rtbuf_signal_bandpass2_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double q = qfactor.sample_fun(qfactor.signal, i);
    const double q2 = q * q;
    const double f4q2 = f4 * q2;
    const double _16f4q2 = 16.0 * f4q2;
    const double _64f4q2 = 64.0 * f4q2;
    const double w = 2.0 * M_PI * fc;
    const double f3qw = f3 * q * w;
    const double sqrt2_7f3qw = sqrt2_7 * f3qw;
    const double sqrt2_9f3qw = sqrt2_9 * f3qw;
    const double w2 = w * w;
    const double _4f2w2 = 4.0 * f2 * w2;
    const double _2q2_1_4f2w2 = (2.0 * q2 + 1.0) * _4f2w2;
    const double w3 = w2 * w;
    const double fqw3 = f * q * w3;
    const double sqrt2_3fqw3 = sqrt2_3 * fqw3;
    const double sqrt2_5fqw3 = sqrt2_5 * fqw3;
    const double w4 = w2 * w2;
    const double q2w4 = q2 * w4;
    const double _4q2w4 = 4.0 * q2w4;
    const double a =       q2w4 + sqrt2_3fqw3 + _2q2_1_4f2w2 + sqrt2_7f3qw + _16f4q2;
    const double b =     _4q2w4 + sqrt2_5fqw3                - sqrt2_9f3qw - _64f4q2;
    const double c = 6.0 * q2w4         - 2.0 * _2q2_1_4f2w2           + 96.0 * f4q2;
    const double d =     _4q2w4 - sqrt2_5fqw3                + sqrt2_9f3qw - _64f4q2;
    const double e =       q2w4 - sqrt2_3fqw3 + _2q2_1_4f2w2 - sqrt2_7f3qw + _16f4q2;
    data->signal[i] = (_4f2w2 * (x
                                 - 2.0 * data->x2
                                 + data->x4)
                       - (b * data->y1
                          + c * data->y2
                          + d * data->y3
                          + e * data->y4)) / a;
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
  { "bandpass2",
    rtbuf_signal_bandpass2,
    rtbuf_signal_bandpass2_start,
    0,
    rtbuf_signal_bandpass2_in,
    rtbuf_signal_bandpass2_out };

unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
