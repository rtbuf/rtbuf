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

#include "rtbuf.h"
#include "rtbuf_music.h"

int rtbuf_music_init ()
{
  rtbuf_type_new(RTBUF_MUSIC_NOTES_TYPE,
                 RTBUF_MUSIC_NOTES_SIZE);
  return 0;
}

int rtbuf_music_notes_new (s_rtbuf_music_notes *notes)
{
  unsigned int i = 0;
  if (!notes)
    return -1;
  if (notes->note_n >= RTBUF_MUSIC_NOTE_MAX)
    return -1;
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    if (notes->note[i].freq == 0.0) {
      notes->note[i].freq = 0.1;
      notes->note_n++;
      return i;
    }
    i++;
  }
  return -1;
}

void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes,
                               unsigned int i)
{
  if (notes && i < RTBUF_MUSIC_NOTE_MAX &&
      notes->note[i].freq != 0.0) {
    notes->note[i].freq = 0.0;
    notes->note_n--;
  }
}

void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt)
{
  if (note) {
    note->start += dt;
    if (note->stop >= 0)
      note->stop += dt;
  }
}

void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt)
{
  unsigned int i = 0;
  if (notes) {
    unsigned int n = notes->note_n;
    while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
      if (notes->note[i].freq != 0.0) {
        rtbuf_music_note_dt(&notes->note[i], dt);
        n--;
      }
      i++;
    }
  }
}

s_rtbuf_music_notes * rtbuf_music_notes (s_rtbuf *rtb,
                                         unsigned int var)
{
  s_rtbuf_binding *v = &rtb->var[var];
  s_rtbuf *target;
  void *data;
  if (v->rtb < 0)
    return 0;
  target = &g_rtbuf[v->rtb];
  //if (v->out >= target->fun->out_n)
  data = target->data + target->fun->out[v->out].offset;
  return (s_rtbuf_music_notes*) data;
}

int rtbuf_music_note_p (s_rtbuf_music_note *note)
{
  return (note && note->freq > 0.0 && note->velocity > 0.0 &&
          note->start >= 0.0);
}
