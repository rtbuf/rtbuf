/*
 * Copyright 2018-2021 Thomas de Grivel <thoxdg@gmail.com>
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
#include <float.h>
#include <stdio.h>
#include <strings.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"
#include "../music.h"
#include "../music_type.h"
#include "../synth.h"
#include "../synth_type.h"

s_rtbuf_lib_proc_in rtbuf_synth_synth_in[] =
  {{ "envelope",   RTBUF_SIGNAL_TYPE, 1.0,  0.0, 1.0, 1.0 },
   { "oscillator", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   RTBUF_MUSIC_NOTES_IN("note"),
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_synth_synth_out[] =
  {{ "signal", RTBUF_SIGNAL_TYPE },
   { "note_n", "unsigned int" },
   { 0, 0 }};

int rtbuf_lib_init (s_rtbuf_lib *lib)
{
  (void) lib;
  rtbuf_music_init();
  return 0;
}

s_rtbuf * rtbuf_in_rtbuf (s_rtbuf *rtb, unsigned int in)
{
  int i;
  assert(in < rtb->proc->in_n);
  if ((i = rtb->in[in].rtb) < 0)
    return 0;
  return &g_rtbuf[i];
}

s_rtbuf * rtbuf_synth_synth_note_envelope (s_rtbuf *rtb, unsigned int i)
{
  unsigned int j;
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  j = RTBUF_SYNTH_SYNTH_IN_NOTE_ENVELOPE(i);
  return rtbuf_in_rtbuf(rtb, j);
}

s_rtbuf * rtbuf_synth_synth_note_oscillator (s_rtbuf *rtb,
                                             unsigned int i)
{
  unsigned int j;
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  j = RTBUF_SYNTH_SYNTH_IN_NOTE_OSCILLATOR(i);
  return rtbuf_in_rtbuf(rtb, j);
}

void rtbuf_synth_synth_delete_note (s_rtbuf *rtb,
                                    unsigned int i)
{
  s_rtbuf *env = rtbuf_synth_synth_note_envelope(rtb, i);
  s_rtbuf *osc = rtbuf_synth_synth_note_oscillator(rtb, i);
  s_rtbuf_synth_synth_data *data;
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  if (env || osc) {
    printf("synth_synth_delete_note %u\n", i);
    if (env) 
      rtbuf_delete(env);
    if (osc)
      rtbuf_delete(osc);
    data->note_n--;
  }
}

void rtbuf_synth_synth_delete_notes (s_rtbuf *rtb)
{
  s_rtbuf_synth_synth_data *data;
  unsigned int i = 0;
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  while (i < RTBUF_MUSIC_NOTE_MAX && data->note_n > 0) {
    rtbuf_synth_synth_delete_note(rtb, i);
    i++;
  }
}

s_rtbuf * rtbuf_synth_synth_new_envelope (s_rtbuf *rtb,
                                          unsigned int i)
{
  s_rtbuf *ref = rtbuf_in_rtbuf(rtb, RTBUF_SYNTH_SYNTH_IN_ENVELOPE);
  int env_i;
  s_rtbuf *env;
  unsigned int env_vel;
  unsigned int env_freq;
  unsigned int env_start;
  unsigned int env_stop;
  s_rtbuf_binding *v = &rtb->in[RTBUF_SYNTH_SYNTH_IN_NOTES];
  unsigned int kbd_i = v->rtb;
  unsigned int kbd_notes = v->out;
  unsigned int note_vel = RTBUF_MUSIC_NOTE_IN_VELOCITY(kbd_notes, i);
  unsigned int note_freq = RTBUF_MUSIC_NOTE_IN_FREQUENCY(kbd_notes, i);
  unsigned int note_start = RTBUF_MUSIC_NOTE_IN_START(kbd_notes, i);
  unsigned int note_stop = RTBUF_MUSIC_NOTE_IN_STOP(kbd_notes, i);
  assert(rtb > g_rtbuf);
  if (!ref)
    return 0;
  if((env_i = rtbuf_clone(ref)) < 0)
    return 0;
  env = &g_rtbuf[env_i];
  env_vel = rtbuf_in_find(env, "velocity");
  env_freq = rtbuf_in_find(env, "frequency");
  env_start = rtbuf_in_find(env, "start");
  env_stop = rtbuf_in_find(env, "stop");
  rtbuf_bind(kbd_i, note_vel  , env, env_vel);
  rtbuf_bind(kbd_i, note_freq , env, env_freq);
  rtbuf_bind(kbd_i, note_start, env, env_start);
  rtbuf_bind(kbd_i, note_stop , env, env_stop);
  rtbuf_bind(env_i, 0, rtb, RTBUF_SYNTH_SYNTH_IN_NOTE_ENVELOPE(i));
  if (env->proc->start)
    env->proc->start(env);
  if (env->proc->f)
    env->proc->f(env);
  /* rtbuf_print_long(env_i); */
  printf("synth_synth_new_envelope %u\n", i);
  return env;
}

s_rtbuf * rtbuf_synth_synth_new_oscillator (s_rtbuf *rtb,
                                            unsigned int i)
{
  s_rtbuf *ref =
    rtbuf_in_rtbuf(rtb, RTBUF_SYNTH_SYNTH_IN_OSCILLATOR);
  int osc_i;
  s_rtbuf *osc;
  unsigned int osc_freq;
  s_rtbuf_binding *v = &rtb->in[RTBUF_SYNTH_SYNTH_IN_NOTES];
  unsigned int kbd_i = v->rtb;
  unsigned int kbd_notes = v->out;
  unsigned int note_freq = RTBUF_MUSIC_NOTE_IN_FREQUENCY(kbd_notes, i);
  assert(rtb > g_rtbuf);
  if (!ref)
    return 0;
  if ((osc_i = rtbuf_clone(ref)) < 0)
    return 0;
  osc = &g_rtbuf[osc_i];
  osc_freq = rtbuf_in_find(osc, "frequency");
  rtbuf_bind(kbd_i, note_freq, osc, osc_freq);
  rtbuf_bind(osc_i, 0, rtb, RTBUF_SYNTH_SYNTH_IN_NOTE_OSCILLATOR(i));
  if (osc->proc->start)
    osc->proc->start(osc);
  if (osc->proc->f)
    osc->proc->f(osc);
  /* rtbuf_print_long(osc_i); */
  return osc;
}

void rtbuf_synth_synth_update_note_signal (s_rtbuf *rtb,
                                           unsigned int i)
{
  s_rtbuf_synth_synth_data *data;
  s_rtbuf_signal_fun env;
  s_rtbuf_signal_fun osc;
  unsigned int j = 0;
  double *signal;
  assert(rtb);
  assert(rtb->data);
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  rtbuf_signal_fun(rtb, RTBUF_SYNTH_SYNTH_IN_NOTE_ENVELOPE(i), &env);
  rtbuf_signal_fun(rtb, RTBUF_SYNTH_SYNTH_IN_NOTE_OSCILLATOR(i), &osc);
  data = (s_rtbuf_synth_synth_data *) rtb->data;
  signal = data->signal;
  while (j < RTBUF_SIGNAL_SAMPLES) {
    double e = env.sample_fun(env.signal, j);
    double o = osc.sample_fun(osc.signal, j);
    e = max(0.0, e);
    *signal += e * o;
    /* printf(" e=%f o=%f s=%f", e, o, *signal); */
    signal++;
    j++;
  }
}

void rtbuf_synth_synth_update_note (s_rtbuf *rtb,
                                    unsigned int i)
{
  s_rtbuf_synth_synth_data *data;
  s_rtbuf_music_notes *notes =
    rtbuf_music_notes(rtb, RTBUF_SYNTH_SYNTH_IN_NOTES);
  s_rtbuf_music_note *note;
  assert(notes);
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  assert(rtb->data);
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  note = &notes->note[i];
  if (note->velocity > 0.0 && note->start >= 0.0) {
    s_rtbuf *env = rtbuf_synth_synth_note_envelope(rtb, i);
    s_rtbuf *osc = rtbuf_synth_synth_note_oscillator(rtb, i);
    if (!env || !osc) {
      if (!env)
        env = rtbuf_synth_synth_new_envelope(rtb, i);
      if (!osc)
        osc = rtbuf_synth_synth_new_oscillator(rtb, i);
      data->note_n++;
    }
    if (env && osc) {
      unsigned int env_state_out = rtbuf_proc_out_find(env->proc,
                                                      "state");
      int env_state = rtbuf_out_int(env, env_state_out,
                                    RTBUF_SYNTH_ENVELOPE_STATE_STARTED);
      if (env_state == RTBUF_SYNTH_ENVELOPE_STATE_STARTED)
        rtbuf_synth_synth_update_note_signal(rtb, i);
    }
  }
  else
    rtbuf_synth_synth_delete_note(rtb, i);
}

int rtbuf_synth_synth (s_rtbuf *rtb)
{
  s_rtbuf_music_notes *notes =
    rtbuf_music_notes(rtb, RTBUF_SYNTH_SYNTH_IN_NOTES);
  s_rtbuf_synth_synth_data *data;
  unsigned int i = 0;
  unsigned int notes_n = notes ? notes->note_n : 0;
  unsigned int n;
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  n = data->note_n;
  rtbuf_signal_zero(data->signal);
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
  if (rtb->proc->out_bytes < sizeof(*data))
    return 1;
  data = (s_rtbuf_synth_synth_data*) rtb->data;
  (void) data;
  return 0;
}

int rtbuf_synth_synth_stop (s_rtbuf *rtb)
{
  rtbuf_synth_synth_delete_notes(rtb);
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "synth",
    rtbuf_synth_synth,
    rtbuf_synth_synth_start,
    0,
    rtbuf_synth_synth_in,
    rtbuf_synth_synth_out };

unsigned long    rtbuf_lib_ver = RTBUF_LIB_VER;
