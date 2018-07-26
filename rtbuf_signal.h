#ifndef RTBUF_SIGNAL_H
#define RTBUF_SIGNAL_H

#define RTBUF_SIGNAL_SAMPLES 256
#define RTBUF_SIGNAL_SAMPLERATE 44100

/* sinus */

enum {
  RTBUF_SIGNAL_SINUS_VAR_FREQUENCY = 0,
  RTBUF_SIGNAL_SINUS_VAR_AMPLITUDE,
  RTBUF_SIGNAL_SINUS_VAR_N
};

typedef struct rtbuf_signal_sinus_data {
  double samples[RTBUF_SIGNAL_SAMPLES];
  double phase;
} s_rtbuf_signal_sinus_data;

int rtbuf_signal_sinus (s_rtbuf *rtb);
int rtbuf_signal_sinus_start (s_rtbuf *rtb);

const char **g_rtbuf_signal_sinus_vars;

#endif /* RTBUF_SIGNAL_H */
