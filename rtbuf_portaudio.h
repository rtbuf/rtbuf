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
#ifndef RTBUF_PORTAUDIO_H
#define RTBUF_PORTAUDIO_H

#include <portaudio.h>
#include "rtbuf_signal.h"

enum {
  RTBUF_PORTAUDIO_LEFT = 0,
  RTBUF_PORTAUDIO_RIGHT,
  RTBUF_PORTAUDIO_CHANNELS
};

#define RTBUF_PORTAUDIO_SAMPLE_TYPE "short"
#define RTBUF_PORTAUDIO_SAMPLES \
  (RTBUF_PORTAUDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES)

typedef short                    t_rtbuf_portaudio_sample;
typedef t_rtbuf_portaudio_sample t_rtbuf_portaudio_samples[RTBUF_PORTAUDIO_SAMPLES];

typedef struct rtbuf_portaudio_input_data {
  t_rtbuf_signal signal[RTBUF_PORTAUDIO_CHANNELS];
  t_rtbuf_portaudio_samples samples;
} s_rtbuf_portaudio_input_data;

int rtbuf_portaudio_input (s_rtbuf *rtb);
int rtbuf_portaudio_input_start (s_rtbuf *rtb);
int rtbuf_portaudio_input_stop (s_rtbuf *rtb);

typedef struct rtbuf_portaudio_output_reserved {
  PaStream *stream;
} s_rtbuf_portaudio_output_reserved;

typedef struct rtbuf_portaudio_output_data {
  t_rtbuf_portaudio_samples samples;
  s_rtbuf_portaudio_output_reserved reserved;
} s_rtbuf_portaudio_output_data;

#define RTBUF_PORTAUDIO_OUTPUT_RESERVED_SIZE \
  sizeof(s_rtbuf_portaudio_output_reserved)

int rtbuf_portaudio_output (s_rtbuf *rtb);
int rtbuf_portaudio_output_start (s_rtbuf *rtb);
int rtbuf_portaudio_output_stop (s_rtbuf *rtb);

#endif
