#ifndef RTBUF_SNDIO_H
#define RTBUF_SNDIO_H

#include <sndio.h>
#include "rtbuf_signal.h"

void print_sio_par (struct sio_par *par);

int rtbuf_sndio_init ();

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

typedef struct rtbuf_sndio_output_data {
  t_rtbuf_sndio_samples samples;
  struct sio_hdl *sio_hdl;
  struct sio_par want;
  struct sio_par have;
} s_rtbuf_sndio_output_data;

#define RTBUF_SNDIO_OUTPUT_RESERVED_SIZE \
  (sizeof(s_rtbuf_sndio_output_data) - sizeof(t_rtbuf_sndio_samples))

int rtbuf_sndio_output (s_rtbuf *rtb);
int rtbuf_sndio_output_start (s_rtbuf *rtb);
int rtbuf_sndio_output_stop (s_rtbuf *rtb);

const char g_rtbuf_sndio_output_reserved_type[1024];

#endif
