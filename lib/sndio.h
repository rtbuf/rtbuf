/* rtbuf
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef RTBUF_SNDIO_H
#define RTBUF_SNDIO_H

#include <sndio.h>
#include "signal.h"

void print_sio_par (struct sio_par *par);

enum {
  RTBUF_SNDIO_LEFT = 0,
  RTBUF_SNDIO_RIGHT,
  RTBUF_SNDIO_CHANNELS
};

#define RTBUF_SNDIO_SAMPLE_TYPE "short"
#define RTBUF_SNDIO_SAMPLES \
  (RTBUF_SNDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES)

typedef short                t_rtbuf_sndio_sample;
typedef t_rtbuf_sndio_sample t_rtbuf_sndio_samples[RTBUF_SNDIO_SAMPLES];

#pragma pack(push,1)
typedef struct rtbuf_sndio_input_data {
  t_rtbuf_signal signal[RTBUF_SNDIO_CHANNELS];
  t_rtbuf_sndio_samples samples;
} s_rtbuf_sndio_input_data;
#pragma pack(pop)

int rtbuf_sndio_input (s_rtbuf *rtb);
int rtbuf_sndio_input_start (s_rtbuf *rtb);
int rtbuf_sndio_input_stop (s_rtbuf *rtb);

typedef struct rtbuf_sndio_output_reserved {
  struct sio_hdl *sio_hdl;
  struct sio_par want;
  struct sio_par have;
} s_rtbuf_sndio_output_reserved;

#pragma pack(push,1)
typedef struct rtbuf_sndio_output_data {
  t_rtbuf_sndio_samples samples;
  s_rtbuf_sndio_output_reserved reserved;
} s_rtbuf_sndio_output_data;
#pragma pack(pop)

#define RTBUF_SNDIO_OUTPUT_RESERVED_SIZE \
  sizeof(s_rtbuf_sndio_output_reserved)

int rtbuf_sndio_output (s_rtbuf *rtb);
int rtbuf_sndio_output_start (s_rtbuf *rtb);
int rtbuf_sndio_output_stop (s_rtbuf *rtb);

#endif
