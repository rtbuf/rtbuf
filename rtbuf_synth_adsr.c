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

#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_synth.h"

static inline
double adsr (double attack, double decay, double sustain,
             double release, double start, double stop)
{
  double x;
  if (start < attack)
    x = start / attack;
  else if (start < attack + decay)
    x = 1.0 + (sustain - 1.0) * (start - attack);
  else
    x = sustain;
  if (stop < 0.0)
    return x;
  if (stop < release)
    return x * (1.0 - stop / release);
  return 0.0;
}

void rtbuf_synth_adsr_signal (s_rtbuf *rtb, double *signal,
                              double velocity, double start,
                              double stop)
{
  double a =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_ATTACK,  0.05);
  double d =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_DECAY,   0.02);
  double s =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_SUSTAIN, 0.666);
  double r =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_RELEASE, 0.2);
  unsigned int i = 0;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double dt = (double) i / RTBUF_SIGNAL_SAMPLERATE;
    double start_i = start + dt;
    double stop_i = stop < 0 ? stop : stop + dt;
    a = max(0.0, a);
    d = max(0.0, d);
    s = max(0.0, s);
    r = max(0.0, r);
    *signal = velocity * adsr(a, d, s, r, start_i, stop_i);
    signal++;
    i++;
  }
}

int rtbuf_synth_adsr (s_rtbuf *rtb)
{
  double velocity =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_VELOCITY, 1.0);
  double start =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_START, -1.0);
  double stop =
    rtbuf_signal_sample(rtb, RTBUF_SYNTH_ADSR_VAR_STOP, -1.0);
  s_rtbuf_synth_adsr_data *data;
  data = (s_rtbuf_synth_adsr_data*) rtb->data;
  if (velocity == 0.0)
    printf("synth_adsr: velocity = 0.0");
  if (start < 0.0 || velocity == 0.0)
    bzero(data->signal, sizeof(t_rtbuf_signal));
  else
    rtbuf_synth_adsr_signal(rtb, data->signal, velocity, start, stop);
  return 0;
}

int rtbuf_synth_adsr_start (s_rtbuf *rtb)
{
  s_rtbuf_synth_adsr_data *data;
  assert(rtb->data);
  assert(rtb->fun);
  assert(rtb->fun->out_bytes == sizeof(*data));
  data = (s_rtbuf_synth_adsr_data*) rtb->data;
  data->state = RTBUF_SYNTH_ENVELOPE_STATE_NOT_STARTED;
  bzero(data->signal, sizeof(t_rtbuf_signal));
  return 0;
}
