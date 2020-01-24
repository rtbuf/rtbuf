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

s_rtbuf_lib_proc_in g_rtbuf_signal_delay_in[] = {
  { "signal",   RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { "delay",    RTBUF_SIGNAL_TYPE, 0.4, 0.0, RTBUF_SIGNAL_DELAY_MAX },
  { "feedback", RTBUF_SIGNAL_TYPE, 0.1, 0.0, 1.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out g_rtbuf_signal_delay_out[] = {
  { "wet", RTBUF_SIGNAL_TYPE },
  { "in", RTBUF_SIGNAL_DELAY_TYPE },
  { "pos", "unsigned int" },
  { 0, 0 } };

s_rtbuf_lib_proc_in g_rtbuf_signal_sinus_in[] = {
  { "frequency", RTBUF_SIGNAL_TYPE, 220.0, 0.0, RTBUF_SIGNAL_SAMPLERATE / 2.0 },
  { "amplitude", RTBUF_SIGNAL_TYPE, 1.0, 0.0, 1.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out g_rtbuf_signal_sinus_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

s_rtbuf_lib_proc_in g_rtbuf_signal_square_in[] = {
  { "frequency", RTBUF_SIGNAL_TYPE, 220.0, 0.0, RTBUF_SIGNAL_SAMPLERATE / 2.0 },
  { "amplitude", RTBUF_SIGNAL_TYPE, 1.0, 0.0, 1.0 },
  { "pulse",     RTBUF_SIGNAL_TYPE, 0.5, 0.0, 1.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out g_rtbuf_signal_square_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

const char     *rtbuf_lib_name = "signal";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_proc rtbuf_lib_proc[] = {
  { "delay", rtbuf_signal_delay, rtbuf_signal_delay_start, 0,
    g_rtbuf_signal_delay_in, g_rtbuf_signal_delay_out },
  { "sinus", rtbuf_signal_sinus, rtbuf_signal_sinus_start, 0,
    g_rtbuf_signal_sinus_in, g_rtbuf_signal_sinus_out },
  { "square", rtbuf_signal_square, rtbuf_signal_square_start, 0,
    g_rtbuf_signal_square_in, g_rtbuf_signal_square_out },
  { 0, 0, 0, 0, 0, 0 } };

void rtbuf_signal_zero (t_rtbuf_signal_sample *signal)
{
  assert(signal);
  bzero(signal, sizeof(t_rtbuf_signal));
}

t_rtbuf_signal_sample
rtbuf_signal_sample (s_rtbuf *rtb,
                     unsigned int in,
                     t_rtbuf_signal_sample default_value)
{
  s_rtbuf_binding *v;
  assert(rtb);
  assert(rtb->proc);
  assert(in < rtb->proc->in_n);
  v = &rtb->in[in];
  if (v->rtb >= 0) {
    s_rtbuf *src;
    s_rtbuf_proc_out *out;
    assert(v->rtb < RTBUF_MAX);
    src = &g_rtbuf[v->rtb];
    assert(src->data);
    assert(src->proc);
    assert(v->out < src->proc->out_n);
    out = &src->proc->out[v->out];
    assert(out->type);
    if (out->type->t.bits >= sizeof(t_rtbuf_signal_sample) * 8) {
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
                       unsigned int in,
                       s_rtbuf_signal_fun *rsf)
{
  s_rtbuf_binding *v;
  const double *default_value;
  assert(rtb);
  assert(rtb->proc);
  assert(in < rtb->proc->in_n);
  assert(rsf);
  default_value = &rtb->proc->in[in].def;
  rsf->signal = default_value;
  rsf->sample_fun = rtbuf_signal_sample_from_sample;
  v = &rtb->in[in];
  if (v->rtb >= 0) {
    s_rtbuf *dest;
    s_rtbuf_proc_out *out;
    assert(v->rtb < RTBUF_MAX);
    dest = &g_rtbuf[v->rtb];
    assert(dest->data);
    assert(dest->proc);
    assert(v->out < dest->proc->out_n);
    out = &dest->proc->out[v->out];
    assert(out->type);
    if (out->type->t.bits >= sizeof(t_rtbuf_signal_sample) * 8)
      rsf->signal = (double*)(dest->data + out->offset);
    if (out->type->t.bits >= sizeof(t_rtbuf_signal) * 8)
      rsf->sample_fun = rtbuf_signal_sample_from_signal;
  }
}
