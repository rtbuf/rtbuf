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
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"
#include "../dynamic.h"

s_rtbuf_lib_proc_in rtbuf_dynamic_limiter_in[] =
  {{ "signal"  , RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "gain"    , RTBUF_SIGNAL_TYPE, 1.0, DBL_EPSILON, 1000.0, 10.0 },
   { "treshold", RTBUF_SIGNAL_TYPE, 0.9, DBL_EPSILON, 1.0, 10.0 },
   { "attack"  , RTBUF_SIGNAL_TYPE, 0.1, 0, 1.0, 1.0 },
   { "release" , RTBUF_SIGNAL_TYPE, 1.0, 0.0, 1.0, 1.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_dynamic_limiter_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "ratio"  , RTBUF_SIGNAL_SAMPLE_TYPE },
   { 0, 0 }};

int rtbuf_dynamic_limiter (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in_;
  s_rtbuf_signal_fun g_;
  s_rtbuf_signal_fun t_;
  s_rtbuf_signal_fun a_;
  s_rtbuf_signal_fun r_;
  s_rtbuf_dynamic_limiter_data *data;
  unsigned int i = 0;
  assert(rtb);
  assert(rtb->proc);
  assert(rtb->data);
  rtbuf_signal_fun(rtb, RTBUF_DYNAMIC_LIMITER_IN_SIGNAL  , &in_);
  rtbuf_signal_fun(rtb, RTBUF_DYNAMIC_LIMITER_IN_GAIN    , &g_);
  rtbuf_signal_fun(rtb, RTBUF_DYNAMIC_LIMITER_IN_TRESHOLD, &t_);
  rtbuf_signal_fun(rtb, RTBUF_DYNAMIC_LIMITER_IN_ATTACK  , &a_);
  rtbuf_signal_fun(rtb, RTBUF_DYNAMIC_LIMITER_IN_RELEASE , &r_);
  data = (s_rtbuf_dynamic_limiter_data*) rtb->data;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double in = in_.sample_fun(in_.signal, i);
    double g = max(0.0, g_.sample_fun(g_.signal, i));
    double t = max(0.0, t_.sample_fun(t_.signal, i));
    double a = max(0.0, a_.sample_fun(a_.signal, i));
    double r = max(0.0, r_.sample_fun(r_.signal, i));
    double in_g = in * g;
    double s = in_g * data->ratio;
    if (s < t)
            if (data->ratio < 1.0) {
                    data->ratio += 1.0 / (r * RTBUF_SIGNAL_SAMPLERATE);
                    if (data->ratio > 1.0)
                            data->ratio = 1.0;
            }
    if (s >= t) {
            data->ratio -= 1.0 / (a * RTBUF_SIGNAL_SAMPLERATE);
            if (data->ratio < 0.0)
                    data->ratio = 0;
    }
    data->signal[i] = in_g * data->ratio;
    i++;
  }
  return 0;
}

int rtbuf_dynamic_limiter_start (s_rtbuf *rtb)
{
  s_rtbuf_dynamic_limiter_data *data;
  assert(rtb);
  assert(rtb->proc);
  assert(rtb->data);
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_dynamic_limiter_data*) rtb->data;
  data->signal[RTBUF_SIGNAL_SAMPLES - 1] = 0.0;
  data->ratio = 1.0;
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "limiter",
    rtbuf_dynamic_limiter,
    rtbuf_dynamic_limiter_start,
    0,
    rtbuf_dynamic_limiter_in,
    rtbuf_dynamic_limiter_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
