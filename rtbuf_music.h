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
#ifndef RTBUF_MUSIC_H
#define RTBUF_MUSIC_H

typedef struct rtbuf_music_note {
  double freq;
  double velocity;    /* between 0.0 and 1.0 */
  double start;       /* time since start in seconds */
  double stop;        /* time since stop in seconds, or -1.0 */
} s_rtbuf_music_note;

#define RTBUF_MUSIC_NOTE_MAX 1024

typedef struct rtbuf_music_notes {
  s_rtbuf_music_note note[RTBUF_MUSIC_NOTE_MAX];
  unsigned int note_n;
} s_rtbuf_music_notes;

#define RTBUF_MUSIC_NOTES_SIZE sizeof(s_rtbuf_music_notes)
#define RTBUF_MUSIC_NOTES_TYPE "music_notes"

extern int  rtbuf_music_init ();
void rtbuf_music_notes_init (s_rtbuf_music_notes *notes);
int  rtbuf_music_notes_new (s_rtbuf_music_notes *notes);
void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes,
                               unsigned int i);
void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt);
int  rtbuf_music_note_p (s_rtbuf_music_note *note);
void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt);

s_rtbuf_music_notes * rtbuf_music_notes (s_rtbuf *rtb,
                                         unsigned int var);

#endif
