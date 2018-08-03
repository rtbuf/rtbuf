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
#include "rtbuf_signal.h"

symbol g_sym_start = 0;
symbol g_sym_stop = 0;

void rtbuf_signal_synth_delete_note (s_rtbuf_signal_synth_data *data,
                                     unsigned int i)
{
  s_rtbuf_signal_synth_note *sn = &data->notes.note[i];
  if (sn->envelope || sn->oscillator) {
    if (sn->envelope) {
      rtbuf_delete(sn->envelope);
      sn->envelope = 0;
    }
    if (sn->oscillator) {
      rtbuf_delete(sn->oscillator);
      sn->oscillator = 0;
    }
    data->notes.note_n--;
  }
}

s_rtbuf * rtbuf_signal_synth_new_envelope (s_rtbuf *rtb,
                                           s_rtbuf_music_note *kn)
{
  s_rtbuf_binding *v = &rtb->var[RTBUF_SIGNAL_SYNTH_VAR_ENVELOPE];
  int i;
  s_rtbuf *env;
  if (v->rtb < 0)
    return 0;
  if((i = rtbuf_clone(&g_rtbuf[v->rtb])) < 0)
    return 0;
  env = &g_rtbuf[i];
  rtbuf_data_set(env, g_sym_start, &kn->start, sizeof(kn->start));
  rtbuf_data_set(env, g_sym_stop, &kn->stop, sizeof(kn->stop));
  return env;
}

s_rtbuf * rtbuf_signal_synth_new_oscillator (s_rtbuf *rtb)
{
  s_rtbuf_binding *v = &rtb->var[RTBUF_SIGNAL_SYNTH_VAR_OSCILLATOR];
  int i;
  if (v->rtb < 0)
    return 0;
  if ((i = rtbuf_clone(&g_rtbuf[v->rtb])) < 0)
    return 0;
  return &g_rtbuf[i];
}

int rtbuf_signal_synth_note_p (s_rtbuf_signal_synth_note *sn)
{
  return sn && sn->envelope && sn->oscillator;
}

void rtbuf_signal_synth_update_note (s_rtbuf *rtb,
                                     s_rtbuf_signal_synth_data *data,
                                     unsigned int i,
                                     s_rtbuf_music_note *note)
{
  s_rtbuf_signal_synth_note *sn = &data->notes.note[i];
  if (rtbuf_music_note_p(note)) {
    if (!sn->envelope || !sn->oscillator) {
      if (!sn->envelope)
        sn->envelope = rtbuf_signal_synth_new_envelope(rtb, note);
      if (!sn->oscillator)
        sn->oscillator = rtbuf_signal_synth_new_oscillator(rtb);
      data->notes.note_n++;
    }
  }
  else
    rtbuf_signal_synth_delete_note(data, i);
}

int rtbuf_signal_synth (s_rtbuf *rtb)
{
  s_rtbuf_signal_synth_data *data;
  s_rtbuf_music_notes *knotes =
    rtbuf_music_notes(rtb, RTBUF_SIGNAL_SYNTH_VAR_KEYBOARD);
  unsigned int i = 0;
  unsigned int kn = knotes ? knotes->note_n : 0;
  unsigned int n;
  data = (s_rtbuf_signal_synth_data*) rtb->data;
  n = data->notes.note_n;
  while (i < RTBUF_MUSIC_NOTE_MAX && (n > 0 || kn > 0)) {
    s_rtbuf_music_note *note = knotes ? &knotes->note[i] : 0;
    if (rtbuf_music_note_p(note))
      kn--;
    if (rtbuf_signal_synth_note_p(&data->notes.note[i]))
      n--;
    rtbuf_signal_synth_update_note(rtb, data, i, note);
    i++;
  }
  return 0;
}

int rtbuf_signal_synth_start (s_rtbuf *rtb)
{
  (void) rtb;
  if (!g_sym_start)
    g_sym_start = symbol_intern("start");
  if (!g_sym_stop)
    g_sym_stop = symbol_intern("stop");
  return 0;
}
