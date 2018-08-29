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
#ifndef RTBUF_SIGNAL_H
#define RTBUF_SIGNAL_H

#include <assert.h>
#include "rtbuf.h"

typedef double t_rtbuf_signal_sample;
#define RTBUF_SIGNAL_SAMPLE_SIZE sizeof(t_rtbuf_signal_sample)
#define RTBUF_SIGNAL_SAMPLE_TYPE "double"

#define RTBUF_SIGNAL_SAMPLES      960
#define RTBUF_SIGNAL_SAMPLERATE 48000
#define RTBUF_SIGNAL_DT \
  ((double) RTBUF_SIGNAL_SAMPLES / RTBUF_SIGNAL_SAMPLERATE)

typedef t_rtbuf_signal_sample t_rtbuf_signal[RTBUF_SIGNAL_SAMPLES];

#define RTBUF_SIGNAL_SIZE sizeof(t_rtbuf_signal)

typedef t_rtbuf_signal_sample
f_rtbuf_signal (const t_rtbuf_signal_sample *signal,
                unsigned int sample);

typedef struct rtbuf_signal_fun {
  f_rtbuf_signal *sample_fun;
  const t_rtbuf_signal_sample *signal;
} s_rtbuf_signal_fun;

void rtbuf_signal_zero (t_rtbuf_signal_sample *signal);
t_rtbuf_signal_sample
rtbuf_signal_sample (s_rtbuf *rtb,
                     unsigned int var,
                     t_rtbuf_signal_sample default_value);
void rtbuf_signal_fun (s_rtbuf *rtb,
                       unsigned int var,
                       s_rtbuf_signal_fun *data,
                       const t_rtbuf_signal_sample *default_value);

f_rtbuf_signal rtbuf_signal_sample_from_sample;
f_rtbuf_signal rtbuf_signal_sample_from_signal;

const double g_rtbuf_signal_sample_zero;
const double g_rtbuf_signal_sample_half;
const double g_rtbuf_signal_sample_one;
const double g_rtbuf_signal_sample_2pi;
const double g_rtbuf_signal_default_frequency;

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

#endif /* RTBUF_SIGNAL_H */
