/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef RTBUF_MUSIC_H
#define RTBUF_MUSIC_H

#include "signal.h"

#define RTBUF_MUSIC_TUNE_A 440.0
#define RTBUF_MUSIC_BASE_OCTAVE 4

#define RTBUF_MUSIC_RELEASE_MAX 1.0

#define RTBUF_MUSIC_NOTE_SIZE (RTBUF_SIGNAL_SAMPLE_SIZE * 4)
#define RTBUF_MUSIC_NOTE_TYPE "music_note"

typedef struct rtbuf_music_note {
  t_rtbuf_signal_sample velocity; /* between 0.0 and 1.0 */
  t_rtbuf_signal_sample freq;     /* above 0.0 */
  t_rtbuf_signal_sample start;    /* time since start */
  t_rtbuf_signal_sample stop;     /* time since stop, or -1.0 */
} s_rtbuf_music_note;

#define RTBUF_MUSIC_NOTE_IN(note)                                           \
  { note "velocity",  RTBUF_SIGNAL_SAMPLE_TYPE, 1.0, 0.0, 1.0, 1.0 },       \
  { note "frequency", RTBUF_SIGNAL_SAMPLE_TYPE, 220.0, 0.0,                 \
      RTBUF_SIGNAL_SAMPLERATE / 2.0, 1.0 },                                 \
  { note "start",     RTBUF_SIGNAL_SAMPLE_TYPE, -1.0, -1.0, DBL_MAX, 1.0 }, \
  { note "stop",      RTBUF_SIGNAL_SAMPLE_TYPE, -1.0, -1.0, DBL_MAX, 1.0 }
          
#define RTBUF_MUSIC_NOTE_IN_ENUM(note) \
  note ## _VELOCITY , \
  note ## _FREQUENCY, \
  note ## _START    , \
  note ## _STOP

enum {
  RTBUF_MUSIC_NOTE_IN_VELOCITY = 0,
  RTBUF_MUSIC_NOTE_IN_FREQUENCY,
  RTBUF_MUSIC_NOTE_IN_START,
  RTBUF_MUSIC_NOTE_IN_STOP,
  RTBUF_MUSIC_NOTE_INS
};

#define RTBUF_MUSIC_NOTE_IN_VELOCITY(notes, i) \
  (notes + 1 + RTBUF_MUSIC_NOTE_INS * i + \
   RTBUF_MUSIC_NOTE_IN_VELOCITY)
#define RTBUF_MUSIC_NOTE_IN_FREQUENCY(notes, i) \
  (notes + 1 + RTBUF_MUSIC_NOTE_INS * i + \
   RTBUF_MUSIC_NOTE_IN_FREQUENCY)
#define RTBUF_MUSIC_NOTE_IN_START(notes, i) \
  (notes + 1 + RTBUF_MUSIC_NOTE_INS * i + RTBUF_MUSIC_NOTE_IN_START)
#define RTBUF_MUSIC_NOTE_IN_STOP(notes, i) \
  (notes + 1 + RTBUF_MUSIC_NOTE_INS * i + RTBUF_MUSIC_NOTE_IN_STOP)

#define RTBUF_MUSIC_NOTE_MAX 32

typedef struct rtbuf_music_notes {
  unsigned int note_n;
  s_rtbuf_music_note note[RTBUF_MUSIC_NOTE_MAX];
} s_rtbuf_music_notes;

#define RTBUF_MUSIC_NOTES_SIZE sizeof(unsigned int)
#define RTBUF_MUSIC_NOTES_TYPE "music_notes"

int  rtbuf_music_init (void);
void rtbuf_music_notes_init (s_rtbuf_music_notes *notes);
int  rtbuf_music_notes_new (s_rtbuf_music_notes *notes, double velocity);
void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes,
                               unsigned int i);
void rtbuf_music_notes_delete_all (s_rtbuf_music_notes *notes);
void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt);
int  rtbuf_music_note_p (s_rtbuf_music_note *note);
void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt);

s_rtbuf_music_notes * rtbuf_music_notes (s_rtbuf *rtb,
                                         unsigned int in);

t_rtbuf_signal_sample rtbuf_music_note_frequency (unsigned int octave,
                                                  unsigned int note);
double rtbuf_music_frequency_note (double f);

#endif
