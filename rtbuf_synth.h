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
#ifndef RTBUF_SYNTH_H
#define RTBUF_SYNTH_H

#include "rtbuf.h"
#include "rtbuf_music.h"
#include "rtbuf_music_type.h"
#include "rtbuf_signal.h"
#include "rtbuf_synth_type.h"

/* adsr */

enum {
  RTBUF_MUSIC_NOTE_IN_ENUM(RTBUF_SYNTH_ADSR_IN),
  RTBUF_SYNTH_ADSR_IN_ATTACK,
  RTBUF_SYNTH_ADSR_IN_DECAY,
  RTBUF_SYNTH_ADSR_IN_SUSTAIN,
  RTBUF_SYNTH_ADSR_IN_RELEASE,
  RTBUF_SYNTH_ADSR_IN_N,
};

enum {
  RTBUF_SYNTH_ENVELOPE_STATE_NOT_STARTED = 0,
  RTBUF_SYNTH_ENVELOPE_STATE_STARTED,
  RTBUF_SYNTH_ENVELOPE_STATE_ENDED
};

#pragma pack(push,1)
typedef struct rtbuf_synth_adsr_data {
  t_rtbuf_signal signal;
  int state;
} s_rtbuf_synth_adsr_data;
#pragma pack(pop)

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

#pragma pack(push,1)
typedef struct rtbuf_synth_synth_data
{
  t_rtbuf_signal signal;
  unsigned int note_n;
} s_rtbuf_synth_synth_data;
#pragma pack(pop)

int rtbuf_synth_synth (s_rtbuf *rtb);
int rtbuf_synth_synth_start (s_rtbuf *rtb);

#endif /* RTBUF_SYNTH_H */
