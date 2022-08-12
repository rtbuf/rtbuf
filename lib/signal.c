/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com>
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
#include "../librtbuf/rtbuf.h"
#include "../librtbuf/lib.h"
#include "signal.h"
#include "signal_type.h"

const double g_rtbuf_signal_sample_zero       = 0.0;
const double g_rtbuf_signal_sample_half       = 0.5;
const double g_rtbuf_signal_sample_one        = 1.0;
const double g_rtbuf_signal_default_frequency = 220.0;

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
    if (out->type->t.bytes >= sizeof(t_rtbuf_signal_sample)) {
      t_rtbuf_signal_sample *sample = (t_rtbuf_signal_sample*)
        ((char*) src->data + out->offset);
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
  default_value = rtbuf_in_unbound_value(rtb, in);
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
    if (out->type->t.bytes >= sizeof(t_rtbuf_signal_sample))
      rsf->signal = (double*)((char*) dest->data + out->offset);
    if (out->type->t.bytes >= sizeof(t_rtbuf_signal))
      rsf->sample_fun = rtbuf_signal_sample_from_signal;
  }
}

const double sqrt2_2 = M_SQRT2 * M_SQRT2;
