/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
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

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_signal_type.h"

const double g_rtbuf_signal_sample_zero       = 0.0;
const double g_rtbuf_signal_sample_half       = 0.5;
const double g_rtbuf_signal_sample_one        = 1.0;
const double g_rtbuf_signal_default_frequency = 220.0;

s_rtbuf_lib_fun_var g_rtbuf_signal_sinus_var[] = {
  { "frequency", RTBUF_SIGNAL_TYPE },
  { "amplitude", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_sinus_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

s_rtbuf_lib_fun_var g_rtbuf_signal_square_var[] = {
  { "frequency", RTBUF_SIGNAL_TYPE },
  { "amplitude", RTBUF_SIGNAL_TYPE },
  { "pulse",     RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_square_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

const char     *rtbuf_lib_name = "signal";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "sinus", rtbuf_signal_sinus, rtbuf_signal_sinus_start, 0,
    g_rtbuf_signal_sinus_var, g_rtbuf_signal_sinus_out },
  { "square", rtbuf_signal_square, rtbuf_signal_square_start, 0,
    g_rtbuf_signal_square_var, g_rtbuf_signal_square_out },
  { 0, 0, 0, 0, 0, 0 } };

void rtbuf_signal_zero (t_rtbuf_signal_sample *signal)
{
  assert(signal);
  bzero(signal, sizeof(t_rtbuf_signal));
}

t_rtbuf_signal_sample
rtbuf_signal_sample (s_rtbuf *rtb,
                     unsigned int var,
                     t_rtbuf_signal_sample default_value)
{
  s_rtbuf_binding *v;
  assert(rtb);
  assert(rtb->fun);
  assert(var < rtb->fun->var_n);
  v = &rtb->var[var];
  if (v->rtb >= 0) {
    s_rtbuf *src;
    s_rtbuf_fun_out *out;
    assert(v->rtb < RTBUF_MAX);
    src = &g_rtbuf[v->rtb];
    assert(src->data);
    assert(src->fun);
    assert(v->out < src->fun->out_n);
    out = &src->fun->out[v->out];
    assert(out->type);
    if (out->type->size >= sizeof(t_rtbuf_signal_sample)) {
      t_rtbuf_signal_sample *sample = (t_rtbuf_signal_sample*)
        (src->data + out->offset);
      return *sample;
    }
  }
  return default_value;
}

double rtbuf_signal_sample_from_sample (const double *signal,
                                        unsigned int i)
{
  (void) i;
  assert(signal);
  return *signal;
}

double rtbuf_signal_sample_from_signal (const double *signal,
                                        unsigned int i)
{
  assert(signal);
  assert(i < RTBUF_SIGNAL_SAMPLES);
  return signal[i];
}

void rtbuf_signal_fun (s_rtbuf *rtb,
                       unsigned int var,
                       s_rtbuf_signal_fun *rsf,
                       const double *default_value)
{
  s_rtbuf_binding *v;
  assert(rtb);
  assert(rtb->fun);
  assert(var < rtb->fun->var_n);
  assert(rsf);
  assert(default_value);
  rsf->signal = default_value;
  rsf->sample_fun = rtbuf_signal_sample_from_sample;
  v = &rtb->var[var];
  if (v->rtb >= 0) {
    s_rtbuf *dest;
    s_rtbuf_fun_out *out;
    assert(v->rtb < RTBUF_MAX);
    dest = &g_rtbuf[v->rtb];
    assert(dest->data);
    assert(dest->fun);
    assert(v->out < dest->fun->out_n);
    out = &dest->fun->out[v->out];
    assert(out->type);
    if (out->type->size >= sizeof(t_rtbuf_signal_sample))
      rsf->signal = (double*)(dest->data + out->offset);
    if (out->type->size >= sizeof(t_rtbuf_signal))
      rsf->sample_fun = rtbuf_signal_sample_from_signal;
  }
}
