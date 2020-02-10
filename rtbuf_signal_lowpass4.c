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
/* Butterworth lowpass fourth order filter
a: cos(5*%pi/8) $
b: cos(7*%pi/8) $
s: 2*Fs*(1-z)/(1+z) $
B4:(s^2-2*s*a+1)*(s^2-2*s*b+1) $
H: 1/B4 $
ratsimp(H);
         4      3      2                   4
(%o20) (z  + 4 z  + 6 z  + 4 z + 1)/((16 Fs
           7 %pi           5 %pi     3           5 %pi      7 %pi         2
 + (16 cos(-----) + 16 cos(-----)) Fs  + (16 cos(-----) cos(-----) + 8) Fs
             8               8                     8          8
          7 %pi          5 %pi            4
 + (4 cos(-----) + 4 cos(-----)) Fs + 1) z
            8              8
            4               7 %pi            5 %pi     3
 + ((- 64 Fs ) + ((- 32 cos(-----)) - 32 cos(-----)) Fs
                              8                8
          7 %pi          5 %pi            3
 + (8 cos(-----) + 8 cos(-----)) Fs + 4) z
            8              8
         4              5 %pi      7 %pi           2       2
 + (96 Fs  + ((- 32 cos(-----) cos(-----)) - 16) Fs  + 6) z
                          8          8
            4            7 %pi           5 %pi     3
 + ((- 64 Fs ) + (32 cos(-----) + 32 cos(-----)) Fs
                           8               8
             7 %pi           5 %pi                    4
 + ((- 8 cos(-----)) - 8 cos(-----)) Fs + 4) z + 16 Fs
               8               8
              7 %pi            5 %pi     3           5 %pi      7 %pi         2
 + ((- 16 cos(-----)) - 16 cos(-----)) Fs  + (16 cos(-----) cos(-----) + 8) Fs
                8                8                     8          8
             7 %pi           5 %pi
 + ((- 4 cos(-----)) - 4 cos(-----)) Fs + 1)
               8               8
c: 16*Fs^4+(16*b+16*a)*Fs^3+(16*a*b+8)*Fs^2+(4*b+4*a)*Fs+1 $
d: (-64*Fs^4+(-32*b-32*a)*Fs^3+(8*b+8*a)*Fs+4 $
e: 96*Fs^4
*/
#include <math.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"

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
  const double fs = RTBUF_SIGNAL_SAMPLERATE;
  const double fs2 = fs * fs;
  const double fs3 = fs2 * fs;
  const double _8fs3 = 8.0 * fs3;
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_SIGNAL, &in);
  rtbuf_signal_fun(rtb, RTBUF_SIGNAL_LOWPASS_IN_CUTOFF, &cutoff);
  data = (s_rtbuf_signal_lowpass4_data*) rtb->data;
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
