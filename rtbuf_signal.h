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

#define RTBUF_SIGNAL_SAMPLES     1024
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
                     unsigned int in,
                     t_rtbuf_signal_sample default_value);
void rtbuf_signal_fun (s_rtbuf *rtb,
                       unsigned int in,
                       s_rtbuf_signal_fun *data);

f_rtbuf_signal rtbuf_signal_sample_from_sample;
f_rtbuf_signal rtbuf_signal_sample_from_signal;

const double g_rtbuf_signal_sample_zero;
const double g_rtbuf_signal_sample_half;
const double g_rtbuf_signal_sample_one;
const double g_rtbuf_signal_sample_2pi;
const double g_rtbuf_signal_default_frequency;

/* delay */

#define RTBUF_SIGNAL_DELAY_MAX 10
#define RTBUF_SIGNAL_DELAY_SAMPLES_MAX (RTBUF_SIGNAL_DELAY_MAX *       \
                                         RTBUF_SIGNAL_SAMPLERATE)

#pragma pack(push,1)
typedef struct rtbuf_signal_delay_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample in[RTBUF_SIGNAL_DELAY_SAMPLES_MAX];
        unsigned int pos;
} s_rtbuf_signal_delay_data;
#pragma pack(pop)

enum {
  RTBUF_SIGNAL_DELAY_IN_SIGNAL = 0,
  RTBUF_SIGNAL_DELAY_IN_DELAY,
  RTBUF_SIGNAL_DELAY_IN_FEEDBACK,
  RTBUF_SIGNAL_DELAY_INS
};

int rtbuf_signal_delay (s_rtbuf *rtb);
int rtbuf_signal_delay_start (s_rtbuf *rtb);

/* flanger */

#define RTBUF_SIGNAL_FLANGER_MAX 1
#define RTBUF_SIGNAL_FLANGER_SAMPLES_MAX (RTBUF_SIGNAL_FLANGER_MAX * \
                                          RTBUF_SIGNAL_SAMPLERATE)

#pragma pack(push,1)
typedef struct rtbuf_signal_flanger_data {
  t_rtbuf_signal signal;
  double phase;
  t_rtbuf_signal_sample in[RTBUF_SIGNAL_FLANGER_SAMPLES_MAX];
  unsigned int pos;
  unsigned int ds;
} s_rtbuf_signal_flanger_data;
#pragma pack(pop)

enum {
  RTBUF_SIGNAL_FLANGER_IN_SIGNAL = 0,
  RTBUF_SIGNAL_FLANGER_IN_FREQUENCY,
  RTBUF_SIGNAL_FLANGER_IN_AMPLITUDE,
  RTBUF_SIGNAL_FLANGER_IN_DELAY,
  RTBUF_SIGNAL_FLANGER_IN_FEEDBACK,
  RTBUF_SIGNAL_FLANGER_IN_N
};

int rtbuf_signal_flanger (s_rtbuf *rtb);
int rtbuf_signal_flanger_start (s_rtbuf *rtb);

/* butterworth hipass filter, first order */

#pragma pack(push,1)
typedef struct rtbuf_signal_hipass_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample y1;
} s_rtbuf_signal_hipass_data;
#pragma pack(pop)

enum {
  RTBUF_SIGNAL_HIPASS_IN_SIGNAL = 0,
  RTBUF_SIGNAL_HIPASS_IN_CUTOFF,
  RTBUF_SIGNAL_HIPASS_INS
};

int rtbuf_signal_hipass (s_rtbuf *rtb);
int rtbuf_signal_hipass_start (s_rtbuf *rtb);

/* butterworth hipass filter, second order */

#pragma pack(push,1)
typedef struct rtbuf_signal_hipass2_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
} s_rtbuf_signal_hipass2_data;
#pragma pack(pop)

int rtbuf_signal_hipass2 (s_rtbuf *rtb);
int rtbuf_signal_hipass2_start (s_rtbuf *rtb);

/* butterworth lowpass filter, first order */

#pragma pack(push,1)
typedef struct rtbuf_signal_lowpass_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample y1;
} s_rtbuf_signal_lowpass_data;
#pragma pack(pop)

enum {
  RTBUF_SIGNAL_LOWPASS_IN_SIGNAL = 0,
  RTBUF_SIGNAL_LOWPASS_IN_CUTOFF,
  RTBUF_SIGNAL_LOWPASS_INS
};

int rtbuf_signal_lowpass (s_rtbuf *rtb);
int rtbuf_signal_lowpass_start (s_rtbuf *rtb);

/* butterworth lowpass filter, second order */

#pragma pack(push,1)
typedef struct rtbuf_signal_lowpass2_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
} s_rtbuf_signal_lowpass2_data;
#pragma pack(pop)

int rtbuf_signal_lowpass2 (s_rtbuf *rtb);
int rtbuf_signal_lowpass2_start (s_rtbuf *rtb);

/* sinus */

#pragma pack(push,1)
typedef struct rtbuf_signal_sinus_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_sinus_data;
#pragma pack(pop)

enum {
  RTBUF_SIGNAL_SINUS_IN_FREQUENCY = 0,
  RTBUF_SIGNAL_SINUS_IN_AMPLITUDE,
  RTBUF_SIGNAL_SINUS_IN_N
};

int rtbuf_signal_sinus (s_rtbuf *rtb);
int rtbuf_signal_sinus_start (s_rtbuf *rtb);

/* square */

enum {
  RTBUF_SIGNAL_SQUARE_IN_FREQUENCY = 0,
  RTBUF_SIGNAL_SQUARE_IN_AMPLITUDE,
  RTBUF_SIGNAL_SQUARE_IN_PULSE,
  RTBUF_SIGNAL_SQUARE_IN_N
};

#pragma pack(push,1)
typedef struct rtbuf_signal_square_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_square_data;
#pragma pack(pop)

int rtbuf_signal_square (s_rtbuf *rtb);
int rtbuf_signal_square_start (s_rtbuf *rtb);

#endif /* RTBUF_SIGNAL_H */
