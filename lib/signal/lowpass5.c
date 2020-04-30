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
/* Butterworth lowpass fifth order filter
a: (sqrt(5)-1)/2 $
b: (sqrt(5)+1)/2 $
s: 2*F*(1-z)/(1+z) $
B5:(s/w+1)*((s/w)^2+a*(s/w)+1)*((s/w)^2+b*s/w+1) $
H: 1/B5 $
ratsimp(H);
  5  5      5  4       5  3       5  2      5      5
(w  z  + 5 w  z  + 10 w  z  + 10 w  z  + 5 w  z + w )
    5                          4                     2  3
/((w  + ((- 2 sqrt(5)) - 2) F w  + (4 sqrt(5) + 12) F  w
                         3  2                      4         5   5
 + ((- 8 sqrt(5)) - 24) F  w  + (16 sqrt(5) + 16) F  w - 32 F ) z
       5                          4                     2  3
 + (5 w  + ((- 6 sqrt(5)) - 6) F w  + (4 sqrt(5) + 12) F  w
                     3  2                          4          5   4
 + (8 sqrt(5) + 24) F  w  + ((- 48 sqrt(5)) - 48) F  w + 160 F ) z
        5                          4                         2  3
 + (10 w  + ((- 4 sqrt(5)) - 4) F w  + ((- 8 sqrt(5)) - 24) F  w
                      3  2                      4          5   3
 + (16 sqrt(5) + 48) F  w  + (32 sqrt(5) + 32) F  w - 320 F ) z
        5                      4                         2  3
 + (10 w  + (4 sqrt(5) + 4) F w  + ((- 8 sqrt(5)) - 24) F  w
                          3  2                      4          5   2
 + ((- 16 sqrt(5)) - 48) F  w  + (32 sqrt(5) + 32) F  w + 320 F ) z
       5                      4                     2  3
 + (5 w  + (6 sqrt(5) + 6) F w  + (4 sqrt(5) + 12) F  w
                         3  2                          4          5
 + ((- 8 sqrt(5)) - 24) F  w  + ((- 48 sqrt(5)) - 48) F  w - 160 F ) z
    5                      4                     2  3
 + w  + (2 sqrt(5) + 2) F w  + (4 sqrt(5) + 12) F  w
                     3  2                      4         5
 + (8 sqrt(5) + 24) F  w  + (16 sqrt(5) + 16) F  w + 32 F )

a:    w5 + (r5 + 1)( 2)(f w4 + 8 f4 w) + (4 r5 + 12)    (f2 w3 + 2 f3 w2) +  32 f5; z
b:  5 w5 + (r5 + 1)( 6)(f w4 - 8 f4 w) + (4 r5 + 12)    (f2 w3 - 2 f3 w2) - 160 f5; z1
c: 10 w5 + (r5 + 1)( 4)(f w4 + 8 f4 w) + (4 r5 + 12)(-2)(f2 w3 + 2 f3 w2) + 320 f5; z2
d: 10 w5 + (r5 + 1)(-4)(f w4 - 8 f4 w) + (4 r5 + 12)(-2)(f2 w3 - 2 f3 w2) - 320 f5; z3
e:  5 w5 + (r5 + 1)(-6)(f w4 + 8 f4 w) + (4 r5 + 12)    (f2 w3 + 2 f3 w2) + 160 f5; z4
f:    w5 + (r5 + 1)(-2)(f w4 - 8 f4 w) + (4 r5 + 12)    (f2 w3 - 2 f3 w2) -  32 f5; z5

*/
#include <math.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/signal.h>

int rtbuf_signal_lowpass5_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_lowpass5_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_signal_lowpass5_data*) rtb->data;
  data->x1 = 0.0;
  data->x2 = 0.0;
  data->x3 = 0.0;
  data->x4 = 0.0;
  data->x5 = 0.0;
  data->y1 = 0.0;
  data->y2 = 0.0;
  data->y3 = 0.0;
  data->y4 = 0.0;
  data->y5 = 0.0;
  return 0;
}

int rtbuf_signal_lowpass5 (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in;
  s_rtbuf_signal_fun cutoff;
  s_rtbuf_signal_lowpass5_data *data;
  unsigned int i = 0;
  const double r5_1 = sqrt(5.0) + 1.0;
  const double _4r5_12 = 4.0 * sqrt(5.0) + 12.0;
  const double f = RTBUF_SIGNAL_SAMPLERATE;
  const double f2 = f * f;
  const double f3 = f2 * f;
  const double f4 = f2 * f2;
  const double f5 = f2 * f3;
  const double _32f5 = 32.0 * f5;
  const double _160f5 = 160.0 * f5;
  const double _320f5 = 320.0 * f5;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_lowpass5_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    const double x = in.sample_fun(in.signal, i);
    const double fc = cutoff.sample_fun(cutoff.signal, i);
    const double w = 2.0 * M_PI * fc;
    const double _8f4w = 8.0 * f4 * w;
    const double w2 = w * w;
    const double _2f3w2 = 2.0 * f3 * w2;
    const double w3 = w2 * w;
    const double f2w3 = f2 * w3;
    const double f2w3p2f3w2 = f2w3 + _2f3w2;
    const double f2w3m2f3w2 = f2w3 - _2f3w2;
    const double w4 = w2 * w2;
    const double fw4 = f * w4;
    const double fw4p8f4w = (fw4 + _8f4w);
    const double fw4m8f4w = (fw4 - _8f4w);
    const double w5 = w2 * w3;
    const double _5w5 = 5.0 * w5;
    const double _10w5 = 10.0 * w5;
    const double a =    w5 + r5_1 *  2.0 * fw4p8f4w + _4r5_12        * f2w3p2f3w2 +  _32f5;
    const double b =  _5w5 + r5_1 *  6.0 * fw4m8f4w + _4r5_12        * f2w3m2f3w2 - _160f5;
    const double c = _10w5 + r5_1 *  4.0 * fw4p8f4w + _4r5_12 * -2.0 * f2w3p2f3w2 + _320f5;
    const double d = _10w5 + r5_1 * -4.0 * fw4m8f4w + _4r5_12 * -2.0 * f2w3m2f3w2 - _320f5;
    const double e =  _5w5 + r5_1 * -6.0 * fw4p8f4w + _4r5_12        * f2w3p2f3w2 + _160f5;
    const double f =    w5 + r5_1 * -2.0 * fw4m8f4w + _4r5_12        * f2w3m2f3w2 -  _32f5;
    data->signal[i] = (w5 * (x
                             + 5.0 * (data->x1
                                      + data->x4)
                             + 10.0 * (data->x2
                                       + data->x3)
                             + data->x5)
                       - (b * data->y1
                          + c * data->y2
                          + d * data->y3
                          + e * data->y4
                          + f * data->y5)) / a;
    data->x5 = data->x4;
    data->x4 = data->x3;
    data->x3 = data->x2;
    data->x2 = data->x1;
    data->x1 = x;
    data->y5 = data->y4;
    data->y4 = data->y3;
    data->y3 = data->y2;
    data->y2 = data->y1;
    data->y1 = data->signal[i];
    i++;
  }
  return 0;
}
