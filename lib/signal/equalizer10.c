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

12db/octave
fc: -3db
3/12 = 1/4 octave = 3/4 et 5/4
q: 2
*/
#include <math.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>

s_rtbuf_lib_proc_in rtbuf_signal_equalizer10_in[] =
  {{ "signal", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "amp32",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp64",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp128", RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp256", RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp512", RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp1k",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp2k",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp4k",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp8k",  RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { "amp16k", RTBUF_SIGNAL_TYPE, 1.0, 0.01, 2.0, 10.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_signal_equalizer10_out[] = {
  { "signal",    RTBUF_SIGNAL_TYPE },
  { "signal32",  RTBUF_SIGNAL_TYPE },
  { "signal64",  RTBUF_SIGNAL_TYPE },
  { "signal128", RTBUF_SIGNAL_TYPE },
  { "signal256", RTBUF_SIGNAL_TYPE },
  { "signal512", RTBUF_SIGNAL_TYPE },
  { "signal1k",  RTBUF_SIGNAL_TYPE },
  { "signal2k",  RTBUF_SIGNAL_TYPE },
  { "signal4k",  RTBUF_SIGNAL_TYPE },
  { "signal8k",  RTBUF_SIGNAL_TYPE },
  { "signal16k", RTBUF_SIGNAL_TYPE },
  { "x1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "x4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y0_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y0_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y0_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y0_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y1_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y1_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y1_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y1_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y2_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y2_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y2_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y2_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y3_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y3_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y3_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y3_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y4_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y4_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y4_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y4_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y5_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y5_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y5_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y5_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y6_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y6_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y6_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y6_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y7_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y7_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y7_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y7_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y8_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y8_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y8_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y8_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y9_1", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y9_2", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y9_3", RTBUF_SIGNAL_SAMPLE_TYPE },
  { "y9_4", RTBUF_SIGNAL_SAMPLE_TYPE },
  { 0, 0 } };

double sqrt2_3 = 0.0;
double sqrt2_5;
double sqrt2_7;
double sqrt2_9;
double f;
double f2;
double f3;
double f4;

int rtbuf_signal_equalizer10_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_equalizer10_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_equalizer10_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->x3 = 0.0;
  data->x4 = 0.0;
  data->y0_1 = 0.0;
  data->y0_2 = 0.0;
  data->y0_3 = 0.0;
  data->y0_4 = 0.0;
  data->y1_1 = 0.0;
  data->y1_2 = 0.0;
  data->y1_3 = 0.0;
  data->y1_4 = 0.0;
  data->y2_1 = 0.0;
  data->y2_2 = 0.0;
  data->y2_3 = 0.0;
  data->y2_4 = 0.0;
  data->y3_1 = 0.0;
  data->y3_2 = 0.0;
  data->y3_3 = 0.0;
  data->y3_4 = 0.0;
  data->y4_1 = 0.0;
  data->y4_2 = 0.0;
  data->y4_3 = 0.0;
  data->y4_4 = 0.0;
  data->y5_1 = 0.0;
  data->y5_2 = 0.0;
  data->y5_3 = 0.0;
  data->y5_4 = 0.0;
  data->y6_1 = 0.0;
  data->y6_2 = 0.0;
  data->y6_3 = 0.0;
  data->y6_4 = 0.0;
  data->y7_1 = 0.0;
  data->y7_2 = 0.0;
  data->y7_3 = 0.0;
  data->y7_4 = 0.0;
  data->y8_1 = 0.0;
  data->y8_2 = 0.0;
  data->y8_3 = 0.0;
  data->y8_4 = 0.0;
  data->y9_1 = 0.0;
  data->y9_2 = 0.0;
  data->y9_3 = 0.0;
  data->y9_4 = 0.0;
  sqrt2_3 = sqrt2_2 * M_SQRT2;
  sqrt2_5 = sqrt2_3 * sqrt2_2;
  sqrt2_7 = sqrt2_5 * sqrt2_2;
  sqrt2_9 = sqrt2_7 * sqrt2_2;
  f = RTBUF_SIGNAL_SAMPLERATE;
  f2 = f * f;
  f3 = f2 * f;
  f4 = f2 * f2;
  return 0;
}

double butterworth_bandpass2 (const double x, const double fc,
                              const double q, const double x2,
                              const double x4, const double y1,
                              const double y2, const double y3,
                              const double y4)
{
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
  const double y = (_4f2w2 * (x
                              - 2.0 * x2
                              + x4)
                    - (b * y1
                       + c * y2
                       + d * y3
                       + e * y4)) / a;
  return y;
}

int rtbuf_signal_equalizer10 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun amp32;
  s_rtbuf_signal_fun amp64;
  s_rtbuf_signal_fun amp128;
  s_rtbuf_signal_fun amp256;
  s_rtbuf_signal_fun amp512;
  s_rtbuf_signal_fun amp1k;
  s_rtbuf_signal_fun amp2k;
  s_rtbuf_signal_fun amp4k;
  s_rtbuf_signal_fun amp8k;
  s_rtbuf_signal_fun amp16k;
  s_rtbuf_signal_equalizer10_data *data;
  unsigned int i = 0;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP32, &amp32);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP64, &amp64);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP128, &amp128);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP256, &amp256);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP512, &amp512);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP1K, &amp1k);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP2K, &amp2k);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP4K, &amp4k);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP8K, &amp8k);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_EQUALIZER10_IN_AMP16K, &amp16k);
  data = (s_rtbuf_signal_equalizer10_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double a0 = amp32.sample_fun(amp32.signal, i);
    const double a1 = amp32.sample_fun(amp64.signal, i);
    const double a2 = amp32.sample_fun(amp128.signal, i);
    const double a3 = amp32.sample_fun(amp256.signal, i);
    const double a4 = amp32.sample_fun(amp512.signal, i);
    const double a5 = amp32.sample_fun(amp1k.signal, i);
    const double a6 = amp32.sample_fun(amp2k.signal, i);
    const double a7 = amp32.sample_fun(amp4k.signal, i);
    const double a8 = amp32.sample_fun(amp8k.signal, i);
    const double a9 = amp32.sample_fun(amp16k.signal, i);
    const double y0 = butterworth_bandpass2(x, 32.0, 2.0,
                                            data->x2, data->x4,
                                            data->y0_1, data->y0_2,
                                            data->y0_3, data->y0_4);
    const double y1 = butterworth_bandpass2(x, 64.0, 2.0,
                                            data->x2, data->x4,
                                            data->y1_1, data->y1_2,
                                            data->y1_3, data->y1_4);
    const double y2 = butterworth_bandpass2(x, 128.0, 2.0,
                                            data->x2, data->x4,
                                            data->y2_1, data->y2_2,
                                            data->y2_3, data->y2_4);
    const double y3 = butterworth_bandpass2(x, 256.0, 2.0,
                                            data->x2, data->x4,
                                            data->y3_1, data->y3_2,
                                            data->y3_3, data->y3_4);
    const double y4 = butterworth_bandpass2(x, 512.0, 2.0,
                                            data->x2, data->x4,
                                            data->y4_1, data->y4_2,
                                            data->y4_3, data->y4_4);
    const double y5 = butterworth_bandpass2(x, 1024.0, 2.0,
                                            data->x2, data->x4,
                                            data->y5_1, data->y5_2,
                                            data->y5_3, data->y5_4);
    const double y6 = butterworth_bandpass2(x, 2048.0, 2.0,
                                            data->x2, data->x4,
                                            data->y6_1, data->y6_2,
                                            data->y6_3, data->y6_4);
    const double y7 = butterworth_bandpass2(x, 4096.0, 2.0,
                                            data->x2, data->x4,
                                            data->y7_1, data->y7_2,
                                            data->y7_3, data->y7_4);
    const double y8 = butterworth_bandpass2(x, 8192.0, 2.0,
                                            data->x2, data->x4,
                                            data->y8_1, data->y8_2,
                                            data->y8_3, data->y8_4);
    const double y9 = butterworth_bandpass2(x, 16384.0, 2.0,
                                            data->x2, data->x4,
                                            data->y9_1, data->y9_2,
                                            data->y9_3, data->y9_4);
    data->signal[i] = (a0 * y0 + a1 * y1 + a2 * y2 + a3 * y3 + a4 * y4 +
                       a5 * y5 + a6 * y6 + a7 * y7 + a8 * y8 + a9 * y9);
    data->signal32[i]  = y0;
    data->signal64[i]  = y1;
    data->signal128[i] = y2;
    data->signal256[i] = y3;
    data->signal512[i] = y4;
    data->signal1k[i]  = y5;
    data->signal2k[i]  = y6;
    data->signal4k[i]  = y7;
    data->signal8k[i]  = y8;
    data->signal16k[i] = y9;
    data->x4 = data->x3;
    data->x3 = data->x2;
    data->x2 = data->x1;
    data->x1 = x;
    data->y0_4 = data->y0_3;
    data->y0_3 = data->y0_2;
    data->y0_2 = data->y0_1;
    data->y0_1 = y0;
    data->y1_4 = data->y1_3;
    data->y1_3 = data->y1_2;
    data->y1_2 = data->y1_1;
    data->y1_1 = y1;
    data->y2_4 = data->y2_3;
    data->y2_3 = data->y2_2;
    data->y2_2 = data->y2_1;
    data->y2_1 = y2;
    data->y3_4 = data->y3_3;
    data->y3_3 = data->y3_2;
    data->y3_2 = data->y3_1;
    data->y3_1 = y3;
    data->y4_4 = data->y4_3;
    data->y4_3 = data->y4_2;
    data->y4_2 = data->y4_1;
    data->y4_1 = y4;
    data->y5_4 = data->y5_3;
    data->y5_3 = data->y5_2;
    data->y5_2 = data->y5_1;
    data->y5_1 = y5;
    data->y6_4 = data->y6_3;
    data->y6_3 = data->y6_2;
    data->y6_2 = data->y6_1;
    data->y6_1 = y6;
    data->y7_4 = data->y7_3;
    data->y7_3 = data->y7_2;
    data->y7_2 = data->y7_1;
    data->y7_1 = y7;
    data->y8_4 = data->y8_3;
    data->y8_3 = data->y8_2;
    data->y8_2 = data->y8_1;
    data->y8_1 = y8;
    data->y9_4 = data->y9_3;
    data->y9_3 = data->y9_2;
    data->y9_2 = data->y9_1;
    data->y9_1 = y9;
    i++;
  }
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "equalizer10",
    rtbuf_signal_equalizer10,
    rtbuf_signal_equalizer10_start,
    0,
    rtbuf_signal_equalizer10_in,
    rtbuf_signal_equalizer10_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
