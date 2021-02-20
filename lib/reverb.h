/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
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
#ifndef RTBUF_REVERB_H
#define RTBUF_REVERB_H

#include <assert.h>
#include "../librtbuf/rtbuf.h"
#include "signal.h"

/* Feedback delay network with fourth order Hadamard matrix */

#define RTBUF_REVERB_FDN4_DELAY_SAMPLES_MAX \
  RTBUF_SIGNAL_SAMPLERATE

typedef struct rtbuf_reverb_fdn4_data {
        t_rtbuf_signal signal;
        t_rtbuf_signal_sample d0[RTBUF_REVERB_FDN4_DELAY_SAMPLES_MAX];
        t_rtbuf_signal_sample d1[RTBUF_REVERB_FDN4_DELAY_SAMPLES_MAX];
        t_rtbuf_signal_sample d2[RTBUF_REVERB_FDN4_DELAY_SAMPLES_MAX];
        t_rtbuf_signal_sample d3[RTBUF_REVERB_FDN4_DELAY_SAMPLES_MAX];
        unsigned int pos;
} s_rtbuf_reverb_fdn4_data;

enum {
        RTBUF_REVERB_FDN4_IN_SIGNAL = 0,
        RTBUF_REVERB_FDN4_IN_TIME,
        RTBUF_REVERB_FDN4_IN_DRY_LEVEL,
        RTBUF_REVERB_FDN4_IN_WET_LEVEL,
        RTBUF_REVERB_FDN4_INS
};

int rtbuf_reverb_fdn4 (s_rtbuf *rtb);
int rtbuf_reverb_fdn4_start (s_rtbuf *rtb);

#endif /* RTBUF_REVERB_H */
