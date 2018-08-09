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

#include <assert.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_synth.h"
#include "rtbuf_synth_type.h"

s_rtbuf * rtbuf_var_rtbuf (s_rtbuf *rtb, unsigned int var)
{
  int i;
  assert(var < rtb->fun->var_n);
  if ((i = rtb->var[var].rtb) < 0)
    return 0;
  return &g_rtbuf[i];
}

s_rtbuf * rtbuf_synth_synth_note_envelope (s_rtbuf *rtb, unsigned int i)
{
  unsigned int j;
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  j = RTBUF_SYNTH_SYNTH_VAR_NOTE_ENVELOPE(i);
  return rtbuf_var_rtbuf(rtb, j);
}

s_rtbuf * rtbuf_synth_synth_note_oscillator (s_rtbuf *rtb,
                                             unsigned int i)
{
  unsigned int j;
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  j = RTBUF_SYNTH_SYNTH_VAR_NOTE_OSCILLATOR(i);
  return rtbuf_var_rtbuf(rtb, j);
}

void rtbuf_synth_synth_delete_note (s_rtbuf *rtb,
                                    unsigned int i)
{
  s_rtbuf *env = rtbuf_synth_synth_note_envelope(rtb, i);
  s_rtbuf *osc = rtbuf_synth_synth_note_oscillator(rtb, i);
  if (env) 
    env->flags |= RTBUF_DELETE;
  if (osc)
    rtbuf_delete(osc);
}

void rtbuf_synth_synth_delete_notes (s_rtbuf *rtb)
{
  unsigned int i = 0;
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    rtbuf_synth_synth_delete_note(rtb, i);
    i++;
  }
}

s_rtbuf * rtbuf_synth_synth_new_envelope (s_rtbuf *rtb,
                                          unsigned int i)
{
  s_rtbuf *ref = rtbuf_var_rtbuf(rtb, RTBUF_SYNTH_SYNTH_VAR_ENVELOPE);
  int env_i;
  s_rtbuf *env;
  unsigned int env_vel;
  unsigned int env_freq;
  unsigned int env_start;
  unsigned int env_stop;
  s_rtbuf_binding *v = &rtb->var[RTBUF_SYNTH_SYNTH_VAR_NOTES];
  unsigned int kbd_i = v->rtb;
  unsigned int kbd_notes = v->out;
  unsigned int note_vel = RTBUF_MUSIC_NOTE_VAR_VELOCITY(kbd_notes, i);
  unsigned int note_freq = RTBUF_MUSIC_NOTE_VAR_FREQUENCY(kbd_notes, i);
  unsigned int note_start = RTBUF_MUSIC_NOTE_VAR_START(kbd_notes, i);
  unsigned int note_stop = RTBUF_MUSIC_NOTE_VAR_STOP(kbd_notes, i);
  assert(rtb > g_rtbuf);
  if (!ref)
    return 0;
  if((env_i = rtbuf_clone(ref)) < 0)
    return 0;
  env = &g_rtbuf[env_i];
  env_vel = rtbuf_var_find(env, "velocity");
  env_freq = rtbuf_var_find(env, "frequency");
  env_start = rtbuf_var_find(env, "start");
  env_stop = rtbuf_var_find(env, "stop");
  rtbuf_bind(kbd_i, note_vel  , env, env_vel);
  rtbuf_bind(kbd_i, note_freq , env, env_stop);
  rtbuf_bind(kbd_i, note_start, env, env_start);
  rtbuf_bind(kbd_i, note_stop , env, env_stop);
  rtbuf_bind(env_i, 0, rtb, RTBUF_SYNTH_SYNTH_VAR_NOTE_ENVELOPE(i));
  if (env->fun->start)
    env->fun->start(env);
  if (env->fun->f)
    env->fun->f(env);
  rtbuf_print_long(env_i);
  return env;
}

s_rtbuf * rtbuf_synth_synth_new_oscillator (s_rtbuf *rtb,
                                            unsigned int i)
{
  s_rtbuf *ref =
    rtbuf_var_rtbuf(rtb, RTBUF_SYNTH_SYNTH_VAR_OSCILLATOR);
  int osc_i;
  s_rtbuf *osc;
  unsigned int osc_freq;
  s_rtbuf_binding *v = &rtb->var[RTBUF_SYNTH_SYNTH_VAR_NOTES];
  unsigned int kbd_i = v->rtb;
  unsigned int kbd_notes = v->out;
  unsigned int note_freq = RTBUF_MUSIC_NOTE_VAR_FREQUENCY(kbd_notes, i);
  assert(rtb > g_rtbuf);
  if (!ref)
    return 0;
  if ((osc_i = rtbuf_clone(ref)) < 0)
    return 0;
  osc = &g_rtbuf[osc_i];
  osc_freq = rtbuf_var_find(osc, "frequency");
  rtbuf_bind(kbd_i, note_freq, osc, osc_freq);
  rtbuf_bind(osc_i, 0, rtb, RTBUF_SYNTH_SYNTH_VAR_NOTE_OSCILLATOR(i));
  if (osc->fun->start)
    osc->fun->start(osc);
  if (osc->fun->f)
    osc->fun->f(osc);
  rtbuf_print_long(osc_i);
  return osc;
}

void rtbuf_synth_synth_update_note_signal (s_rtbuf *rtb,
                                           unsigned int i,
                                           double *signal)
{
  s_rtbuf_signal_fun env;
  s_rtbuf_signal_fun osc;
  unsigned int j = 0;
  rtbuf_signal_fun(rtb, RTBUF_SYNTH_SYNTH_VAR_NOTE_ENVELOPE(i), &env,
                   &g_rtbuf_signal_sample_zero);
  rtbuf_signal_fun(rtb, RTBUF_SYNTH_SYNTH_VAR_NOTE_ENVELOPE(i), &osc,
                   &g_rtbuf_signal_sample_zero);
  while (j < RTBUF_SIGNAL_SAMPLES) {
    double e = env.sample_fun(env.signal, j);
    double o = osc.sample_fun(osc.signal, j);
    e = max(0.0, e);
    *signal += e * o;
    //printf(" e=%f o=%f s=%f", e, o, *signal);
    signal++;
    j++;
  }
}

void rtbuf_synth_synth_update_note (s_rtbuf *rtb,
                                    unsigned int i)
{
  s_rtbuf_synth_synth_data *data;
  s_rtbuf_music_notes *notes =
    rtbuf_music_notes(rtb, RTBUF_SYNTH_SYNTH_VAR_NOTES);
  double velocity;
  s_rtbuf *env = rtbuf_synth_synth_note_envelope(rtb, i);
  s_rtbuf *osc = rtbuf_synth_synth_note_oscillator(rtb, i);
  assert(notes);
  velocity = notes->note[i].velocity;
  data = (s_rtbuf_synth_synth_data *) rtb->data;
  if (velocity > 0.0) {
    if (!env)
      env = rtbuf_synth_synth_new_envelope(rtb, i);
    if (!osc)
      osc = rtbuf_synth_synth_new_oscillator(rtb, i);
    else if (env && osc)
      rtbuf_synth_synth_update_note_signal(rtb, i, data->signal);
    else
      assert(0);
  }
  else
    rtbuf_synth_synth_delete_note(rtb, i);
}

int rtbuf_synth_synth (s_rtbuf *rtb)
{
  s_rtbuf_music_notes *notes =
    rtbuf_music_notes(rtb, RTBUF_SYNTH_SYNTH_VAR_NOTES);
  s_rtbuf_synth_synth_data *data;
  unsigned int i = 0;
  unsigned int notes_n = notes ? notes->note_n : 0;
  unsigned int n;
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  n = data->note_n;
  bzero(data->signal, sizeof(t_rtbuf_signal));
  while (i < RTBUF_MUSIC_NOTE_MAX && (n > 0 || notes_n > 0)) {
    s_rtbuf_music_note *note = notes ? &notes->note[i] : 0;
    if (rtbuf_music_note_p(note))
      notes_n--;
    if (rtbuf_synth_synth_note_envelope(rtb, i))
      n--;
    rtbuf_synth_synth_update_note(rtb, i);
    i++;
  }
  return 0;
}

int rtbuf_synth_synth_start (s_rtbuf *rtb)
{
  s_rtbuf_synth_synth_data *data;
  assert(rtb->fun->out_bytes == sizeof(*data));
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  return 0;
}

int rtbuf_synth_synth_stop (s_rtbuf *rtb)
{
  rtbuf_synth_synth_delete_notes(rtb);
  return 0;
}
