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
