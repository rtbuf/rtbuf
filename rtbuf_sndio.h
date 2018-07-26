#ifndef RTBUF_SNDIO_H
#define RTBUF_SNDIO_H

enum {
  RTBUF_SNDIO_LEFT = 0,
  RTBUF_SNDIO_RIGHT,
  RTBUF_SNDIO_CHANNELS
};

typedef struct rtbuf_sndio_input_data {
  double samples[RTBUF_SNDIO_CHANNELS][RTBUF_SIGNAL_SAMPLES];
  short short_samples[RTBUF_SNDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES];
} s_rtbuf_sndio_input_data;

int rtbuf_sndio_input (s_rtbuf *rtb);
int rtbuf_sndio_input_start (s_rtbuf *rtb);
int rtbuf_sndio_input_stop (s_rtbuf *rtb);

typedef struct rtbuf_sndio_output_data {
  short samples[RTBUF_SNDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES];
  struct sio_hdl *sio_hdl;
  struct sio_par want;
  struct sio_par have;
} s_rtbuf_sndio_output_data;

int rtbuf_sndio_output (s_rtbuf *rtb);
int rtbuf_sndio_output_start (s_rtbuf *rtb);
int rtbuf_sndio_output_stop (s_rtbuf *rtb);

#endif
