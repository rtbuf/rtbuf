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
#ifndef RTBUF_PORTAUDIO_H
#define RTBUF_PORTAUDIO_H

#include <portaudio.h>
#include "signal.h"

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
