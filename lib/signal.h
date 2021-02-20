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
#include "../librtbuf/rtbuf.h"

typedef double t_rtbuf_signal_sample;
#define RTBUF_SIGNAL_SAMPLE_SIZE sizeof(t_rtbuf_signal_sample)
#define RTBUF_SIGNAL_SAMPLE_TYPE "double"

#define RTBUF_SIGNAL_SAMPLES      256
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

/* butterworth bandpass filter, second order */

typedef struct rtbuf_signal_bandpass2_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
        t_rtbuf_signal_sample y4;
} s_rtbuf_signal_bandpass2_data;

enum {
  RTBUF_SIGNAL_BANDPASS_IN_SIGNAL = 0,
  RTBUF_SIGNAL_BANDPASS_IN_CUTOFF,
  RTBUF_SIGNAL_BANDPASS_IN_QFACTOR,
  RTBUF_SIGNAL_BANDPASS_INS
};

int rtbuf_signal_bandpass2 (s_rtbuf *rtb);
int rtbuf_signal_bandpass2_start (s_rtbuf *rtb);

/* delay */

#define RTBUF_SIGNAL_DELAY_MAX 10
#define RTBUF_SIGNAL_DELAY_SAMPLES_MAX (RTBUF_SIGNAL_DELAY_MAX *       \
                                         RTBUF_SIGNAL_SAMPLERATE)

typedef struct rtbuf_signal_delay_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample in[RTBUF_SIGNAL_DELAY_SAMPLES_MAX];
        unsigned int pos;
} s_rtbuf_signal_delay_data;

enum {
        RTBUF_SIGNAL_DELAY_IN_SIGNAL = 0,
        RTBUF_SIGNAL_DELAY_IN_DELAY,
        RTBUF_SIGNAL_DELAY_IN_FEEDBACK,
        RTBUF_SIGNAL_DELAY_INS
};

int rtbuf_signal_delay (s_rtbuf *rtb);
int rtbuf_signal_delay_start (s_rtbuf *rtb);

/* butterworth second order 10 band equalizer */

typedef struct rtbuf_signal_equalizer10_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal signal32;
        t_rtbuf_signal signal64;
        t_rtbuf_signal signal128;
        t_rtbuf_signal signal256;
        t_rtbuf_signal signal512;
        t_rtbuf_signal signal1k;
        t_rtbuf_signal signal2k;
        t_rtbuf_signal signal4k;
        t_rtbuf_signal signal8k;
        t_rtbuf_signal signal16k;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample y0_1;
        t_rtbuf_signal_sample y0_2;
        t_rtbuf_signal_sample y0_3;
        t_rtbuf_signal_sample y0_4;
        t_rtbuf_signal_sample y1_1;
        t_rtbuf_signal_sample y1_2;
        t_rtbuf_signal_sample y1_3;
        t_rtbuf_signal_sample y1_4;
        t_rtbuf_signal_sample y2_1;
        t_rtbuf_signal_sample y2_2;
        t_rtbuf_signal_sample y2_3;
        t_rtbuf_signal_sample y2_4;
        t_rtbuf_signal_sample y3_1;
        t_rtbuf_signal_sample y3_2;
        t_rtbuf_signal_sample y3_3;
        t_rtbuf_signal_sample y3_4;
        t_rtbuf_signal_sample y4_1;
        t_rtbuf_signal_sample y4_2;
        t_rtbuf_signal_sample y4_3;
        t_rtbuf_signal_sample y4_4;
        t_rtbuf_signal_sample y5_1;
        t_rtbuf_signal_sample y5_2;
        t_rtbuf_signal_sample y5_3;
        t_rtbuf_signal_sample y5_4;
        t_rtbuf_signal_sample y6_1;
        t_rtbuf_signal_sample y6_2;
        t_rtbuf_signal_sample y6_3;
        t_rtbuf_signal_sample y6_4;
        t_rtbuf_signal_sample y7_1;
        t_rtbuf_signal_sample y7_2;
        t_rtbuf_signal_sample y7_3;
        t_rtbuf_signal_sample y7_4;
        t_rtbuf_signal_sample y8_1;
        t_rtbuf_signal_sample y8_2;
        t_rtbuf_signal_sample y8_3;
        t_rtbuf_signal_sample y8_4;
        t_rtbuf_signal_sample y9_1;
        t_rtbuf_signal_sample y9_2;
        t_rtbuf_signal_sample y9_3;
        t_rtbuf_signal_sample y9_4;
} s_rtbuf_signal_equalizer10_data;

enum {
  RTBUF_SIGNAL_EQUALIZER10_IN_SIGNAL = 0,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP32,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP64,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP128,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP256,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP512,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP1K,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP2K,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP4K,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP8K,
  RTBUF_SIGNAL_EQUALIZER10_IN_AMP16K,
  RTBUF_SIGNAL_EQUALIZER10_INS
};

int rtbuf_signal_equalizer10 (s_rtbuf *rtb);
int rtbuf_signal_equalizer10_start (s_rtbuf *rtb);

/* flanger */

#define RTBUF_SIGNAL_FLANGER_MAX 1
#define RTBUF_SIGNAL_FLANGER_SAMPLES_MAX (RTBUF_SIGNAL_FLANGER_MAX * \
                                          RTBUF_SIGNAL_SAMPLERATE)

typedef struct rtbuf_signal_flanger_data {
  t_rtbuf_signal signal;
  double phase;
  t_rtbuf_signal_sample in[RTBUF_SIGNAL_FLANGER_SAMPLES_MAX];
  unsigned int pos;
  unsigned int ds;
} s_rtbuf_signal_flanger_data;

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

typedef struct rtbuf_signal_hipass_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample y1;
} s_rtbuf_signal_hipass_data;

enum {
  RTBUF_SIGNAL_HIPASS_IN_SIGNAL = 0,
  RTBUF_SIGNAL_HIPASS_IN_CUTOFF,
  RTBUF_SIGNAL_HIPASS_INS
};

int rtbuf_signal_hipass (s_rtbuf *rtb);
int rtbuf_signal_hipass_start (s_rtbuf *rtb);

/* butterworth hipass filter, second order */

typedef struct rtbuf_signal_hipass2_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
} s_rtbuf_signal_hipass2_data;

int rtbuf_signal_hipass2 (s_rtbuf *rtb);
int rtbuf_signal_hipass2_start (s_rtbuf *rtb);

/* butterworth hipass filter, third order */

typedef struct rtbuf_signal_hipass3_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
} s_rtbuf_signal_hipass3_data;

int rtbuf_signal_hipass3 (s_rtbuf *rtb);
int rtbuf_signal_hipass3_start (s_rtbuf *rtb);

/* butterworth hipass filter, fourth order */

typedef struct rtbuf_signal_hipass4_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
        t_rtbuf_signal_sample y4;
} s_rtbuf_signal_hipass4_data;

int rtbuf_signal_hipass4 (s_rtbuf *rtb);
int rtbuf_signal_hipass4_start (s_rtbuf *rtb);

/* butterworth hipass filter, fifth order */

typedef struct rtbuf_signal_hipass5_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample x5;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
        t_rtbuf_signal_sample y4;
        t_rtbuf_signal_sample y5;
} s_rtbuf_signal_hipass5_data;

int rtbuf_signal_hipass5 (s_rtbuf *rtb);
int rtbuf_signal_hipass5_start (s_rtbuf *rtb);

/* butterworth lowpass filter, first order */

typedef struct rtbuf_signal_lowpass_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample y1;
} s_rtbuf_signal_lowpass_data;

enum {
  RTBUF_SIGNAL_LOWPASS_IN_SIGNAL = 0,
  RTBUF_SIGNAL_LOWPASS_IN_CUTOFF,
  RTBUF_SIGNAL_LOWPASS_INS
};

int rtbuf_signal_lowpass (s_rtbuf *rtb);
int rtbuf_signal_lowpass_start (s_rtbuf *rtb);

/* butterworth lowpass filter, second order */

typedef struct rtbuf_signal_lowpass2_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
} s_rtbuf_signal_lowpass2_data;

int rtbuf_signal_lowpass2 (s_rtbuf *rtb);
int rtbuf_signal_lowpass2_start (s_rtbuf *rtb);

/* butterworth lowpass filter, third order */

typedef struct rtbuf_signal_lowpass3_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
} s_rtbuf_signal_lowpass3_data;

int rtbuf_signal_lowpass3 (s_rtbuf *rtb);
int rtbuf_signal_lowpass3_start (s_rtbuf *rtb);

/* butterworth lowpass filter, fourth order */

typedef struct rtbuf_signal_lowpass4_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
        t_rtbuf_signal_sample y4;
} s_rtbuf_signal_lowpass4_data;

int rtbuf_signal_lowpass4 (s_rtbuf *rtb);
int rtbuf_signal_lowpass4_start (s_rtbuf *rtb);

/* butterworth lowpass filter, fifth order */

typedef struct rtbuf_signal_lowpass5_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample x1;
        t_rtbuf_signal_sample x2;
        t_rtbuf_signal_sample x3;
        t_rtbuf_signal_sample x4;
        t_rtbuf_signal_sample x5;
        t_rtbuf_signal_sample y1;
        t_rtbuf_signal_sample y2;
        t_rtbuf_signal_sample y3;
        t_rtbuf_signal_sample y4;
        t_rtbuf_signal_sample y5;
} s_rtbuf_signal_lowpass5_data;

int rtbuf_signal_lowpass5 (s_rtbuf *rtb);
int rtbuf_signal_lowpass5_start (s_rtbuf *rtb);

/* sawtooth */

typedef struct rtbuf_signal_sawtooth_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_sawtooth_data;

enum {
  RTBUF_SIGNAL_SAWTOOTH_IN_FREQUENCY = 0,
  RTBUF_SIGNAL_SAWTOOTH_IN_AMPLITUDE,
  RTBUF_SIGNAL_SAWTOOTH_IN_N
};

int rtbuf_signal_sawtooth (s_rtbuf *rtb);
int rtbuf_signal_sawtooth_start (s_rtbuf *rtb);

/* sinus */

typedef struct rtbuf_signal_sinus_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_sinus_data;

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

typedef struct rtbuf_signal_square_data {
  t_rtbuf_signal signal;
  double phase;
} s_rtbuf_signal_square_data;

int rtbuf_signal_square (s_rtbuf *rtb);
int rtbuf_signal_square_start (s_rtbuf *rtb);

const double sqrt2_2;

#endif /* RTBUF_SIGNAL_H */
