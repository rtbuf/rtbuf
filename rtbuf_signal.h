#ifndef RTBUF_SIGNAL_H
#define RTBUF_SIGNAL_H

#include "rtbuf_type.h"
#include "rtbuf_music.h"

#define RTBUF_SIGNAL_SAMPLES 256
#define RTBUF_SIGNAL_SAMPLERATE 44100
#define RTBUF_SIGNAL_TYPE \
  RTBUF_TYPE_DOUBLE_ARRAY(RTBUF_SIGNAL_SAMPLES)

typedef double t_rtbuf_signal[RTBUF_SIGNAL_SAMPLES];

double rtbuf_signal_sample (s_rtbuf *rtb, unsigned int var,
                            unsigned int i, double default_value);

/* sinus */

typedef struct rtbuf_signal_sinus_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_sinus_data;

enum {
  RTBUF_SIGNAL_SINUS_VAR_FREQUENCY = 0,
  RTBUF_SIGNAL_SINUS_VAR_AMPLITUDE,
  RTBUF_SIGNAL_SINUS_VAR_N
};

int rtbuf_signal_sinus (s_rtbuf *rtb);
int rtbuf_signal_sinus_start (s_rtbuf *rtb);

/* square */

enum {
  RTBUF_SIGNAL_SQUARE_VAR_FREQUENCY = 0,
  RTBUF_SIGNAL_SQUARE_VAR_AMPLITUDE,
  RTBUF_SIGNAL_SQUARE_VAR_PULSE,
  RTBUF_SIGNAL_SQUARE_VAR_N
};

typedef struct rtbuf_signal_square_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_square_data;

int rtbuf_signal_square (s_rtbuf *rtb);
int rtbuf_signal_square_start (s_rtbuf *rtb);

/* synth */

typedef struct rtbuf_signal_synth_note {
  s_rtbuf *envelope;
  s_rtbuf *oscillator;
} s_rtbuf_signal_synth_note;

#define RTBUF_SIGNAL_SYNTH_NOTE_SIZE sizeof(s_rtbuf_signal_synth_note)
#define RTBUF_SIGNAL_SYNTH_NOTE_TYPE "signal_synth_note"

enum {
  RTBUF_SIGNAL_SYNTH_VAR_KEYBOARD = 0,
  RTBUF_SIGNAL_SYNTH_VAR_ENVELOPE,
  RTBUF_SIGNAL_SYNTH_VAR_OSCILLATOR,
  RTBUF_SIGNAL_SYNTH_VAR_N,
};

typedef struct rtbuf_signal_synth_notes {
  s_rtbuf_signal_synth_note note[RTBUF_MUSIC_NOTE_MAX];
  unsigned int note_n;
} s_rtbuf_signal_synth_notes;

#define RTBUF_SIGNAL_SYNTH_NOTES_SIZE sizeof(s_rtbuf_signal_synth_notes)
#define RTBUF_SIGNAL_SYNTH_NOTES_TYPE "signal_synth_notes"

typedef struct rtbuf_signal_synth_data
{
  t_rtbuf_signal signal;
  s_rtbuf_signal_synth_notes notes;
} s_rtbuf_signal_synth_data;

int rtbuf_signal_synth (s_rtbuf *rtb);
int rtbuf_signal_synth_start (s_rtbuf *rtb);

#endif /* RTBUF_SIGNAL_H */
