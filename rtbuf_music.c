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
#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_music.h"

int    g_rtbuf_music_init = 0;
double g_rtbuf_music_tune = 0.0;

int rtbuf_music_init ()
{
  double tune_c;
  if (g_rtbuf_music_init)
    return 0;
  rtbuf_type_define(RTBUF_MUSIC_NOTE_TYPE, RTBUF_MUSIC_NOTE_SIZE);
  rtbuf_type_define(RTBUF_MUSIC_NOTES_TYPE, RTBUF_MUSIC_NOTES_SIZE);
  tune_c = fmod(log2(RTBUF_MUSIC_TUNE_A) - 9.0 / 12.0, 1.0);
  g_rtbuf_music_tune = RTBUF_MUSIC_BASE_OCTAVE + tune_c;
  g_rtbuf_music_init = 1;
  return 0;
}

double rtbuf_music_note_frequency (unsigned int octave,
                                   unsigned int note)
{
  return exp2(octave + note / 12.0 + g_rtbuf_music_tune);
}

double rtbuf_music_frequency_note (double f)
{
  return log2(f) * 12.0 - g_rtbuf_music_tune;
}

int rtbuf_music_notes_new (s_rtbuf_music_notes *notes, double velocity)
{
  unsigned int i = 0;
  assert(notes);
  if (velocity == 0.0)
    return rtbuf_err("music_notes_new: zero velocity");
  if (notes->note_n >= RTBUF_MUSIC_NOTE_MAX)
    return rtbuf_err("RTBUF_MUSIC_NOTE_MAX exhausted");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    if (notes->note[i].velocity == 0.0) {
      notes->note[i].velocity = velocity;
      notes->note_n++;
      printf("music_notes_new %u\n", i);
      return i;
    }
    i++;
  }
  return -1;
}

void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes,
                               unsigned int i)
{
  s_rtbuf_music_note *note;
  assert(notes);
  assert(i < RTBUF_MUSIC_NOTE_MAX);
  note = &notes->note[i];
  if (note->velocity > 0.0 && note->start >= 0.0) {
    printf("music_notes_delete %u\n", i);
    notes->note_n--;
  }
  note->velocity = 0.0;
  note->freq = 0.0;
  note->start = -1.0;
  note->stop = -1.0;
}

void rtbuf_music_notes_delete_all (s_rtbuf_music_notes *notes)
{
  unsigned int i = 0;
  while (i < RTBUF_MUSIC_NOTE_MAX && notes->note_n > 0) {
    rtbuf_music_notes_delete(notes, i);
    i++;
  }
}

void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt)
{
  assert(note);
  if (note->start >= 0.0) {
    note->start += dt;
    if (note->stop >= 0.0)
      note->stop += dt;
  }
}

void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt)
{
  unsigned int i = 0;
  if (notes) {
    unsigned int n = notes->note_n;
    while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
      if (notes->note[i].velocity > 0.0) {
        rtbuf_music_note_dt(&notes->note[i], dt);
        n--;
      }
      i++;
    }
  }
}

s_rtbuf_music_notes * rtbuf_music_notes (s_rtbuf *rtb,
                                         unsigned int in)
{
  s_rtbuf_binding *v;
  s_rtbuf *target;
  unsigned int offset;
  v = &rtb->in[in];
  if (v->rtb < 0)
    return 0;
  assert(v->rtb < RTBUF_MAX);
  target = &g_rtbuf[v->rtb];
  assert(target->proc);
  assert(v->out < target->proc->out_n);
  offset = target->proc->out[v->out].offset;
  return (s_rtbuf_music_notes*) (target->data + offset);
}

int rtbuf_music_note_p (s_rtbuf_music_note *note)
{
  return (note && note->velocity > 0.0);
}
