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
#ifndef RTBUF_SYNTH_H
#define RTBUF_SYNTH_H

#include "../librtbuf/rtbuf.h"
#include "music.h"
#include "music_type.h"
#include "signal.h"
#include "synth_type.h"

/* adsr */

enum {
  RTBUF_MUSIC_NOTE_IN_ENUM(RTBUF_SYNTH_ADSR_IN),
  RTBUF_SYNTH_ADSR_IN_ATTACK,
  RTBUF_SYNTH_ADSR_IN_DECAY,
  RTBUF_SYNTH_ADSR_IN_SUSTAIN,
  RTBUF_SYNTH_ADSR_IN_RELEASE,
  RTBUF_SYNTH_ADSR_IN_N
};

enum {
  RTBUF_SYNTH_ENVELOPE_STATE_NOT_STARTED = 0,
  RTBUF_SYNTH_ENVELOPE_STATE_STARTED,
  RTBUF_SYNTH_ENVELOPE_STATE_ENDED
};

typedef struct rtbuf_synth_adsr_data {
  t_rtbuf_signal signal;
  int state;
} s_rtbuf_synth_adsr_data;

int rtbuf_synth_adsr (s_rtbuf *rtb);
int rtbuf_synth_adsr_start (s_rtbuf *rtb);

/* synth */

enum {
  RTBUF_SYNTH_SYNTH_IN_ENVELOPE = 0,
  RTBUF_SYNTH_SYNTH_IN_OSCILLATOR,
  RTBUF_SYNTH_SYNTH_IN_NOTES,
  RTBUF_SYNTH_NOTES_IN_ENUM(RTBUF_SYNTH_SYNTH_IN_NOTE),
  RTBUF_SYNTH_SYNTH_IN_N
};

#define RTBUF_SYNTH_SYNTH_IN_NOTE_ENVELOPE(i) \
  (RTBUF_SYNTH_SYNTH_IN_NOTE_0_ENVELOPE + 2 * i)

#define RTBUF_SYNTH_SYNTH_IN_NOTE_OSCILLATOR(i) \
  (RTBUF_SYNTH_SYNTH_IN_NOTE_0_OSCILLATOR + 2 * i)

typedef struct rtbuf_synth_synth_data
{
  t_rtbuf_signal signal;
  unsigned int note_n;
} s_rtbuf_synth_synth_data;

int rtbuf_synth_synth (s_rtbuf *rtb);
int rtbuf_synth_synth_start (s_rtbuf *rtb);

#endif /* RTBUF_SYNTH_H */
