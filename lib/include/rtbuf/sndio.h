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
#ifndef RTBUF_SNDIO_H
#define RTBUF_SNDIO_H

#include <sndio.h>
#include <rtbuf/signal.h>

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

typedef struct rtbuf_sndio_input_data {
  t_rtbuf_signal signal[RTBUF_SNDIO_CHANNELS];
  t_rtbuf_sndio_samples samples;
} s_rtbuf_sndio_input_data;

int rtbuf_sndio_input (s_rtbuf *rtb);
int rtbuf_sndio_input_start (s_rtbuf *rtb);
int rtbuf_sndio_input_stop (s_rtbuf *rtb);

typedef struct rtbuf_sndio_output_reserved {
  struct sio_hdl *sio_hdl;
  struct sio_par want;
  struct sio_par have;
} s_rtbuf_sndio_output_reserved;

typedef struct rtbuf_sndio_output_data {
  t_rtbuf_sndio_samples samples;
  s_rtbuf_sndio_output_reserved reserved;
} s_rtbuf_sndio_output_data;

#define RTBUF_SNDIO_OUTPUT_RESERVED_SIZE \
  sizeof(s_rtbuf_sndio_output_reserved)

int rtbuf_sndio_output (s_rtbuf *rtb);
int rtbuf_sndio_output_start (s_rtbuf *rtb);
int rtbuf_sndio_output_stop (s_rtbuf *rtb);

#endif
