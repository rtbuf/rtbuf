#include "rtbuf.h"
#include "rtbuf_signal.h"
#include "rtbuf_music.h"

static inline
double adsr (double attack, double decay, double sustain,
             double release, double start, double stop)
{
  double x;
  if (start < attack)
    x = start / attack;
  else if (start < attack + decay)
    x = 1.0 + (sustain - 1.0) * (start - attack);
  else
    x = sustain;
  if (stop < 0.0)
    return x;
  if (stop < release)
    return x * (1.0 - stop / release);
  return 0.0;
}

enum {
  RTBUF_SIGNAL_ADSR_VAR_NOTE = 0,
  RTBUF_SIGNAL_ADSR_VAR_ATTACK,
  RTBUF_SIGNAL_ADSR_VAR_DECAY,
  RTBUF_SIGNAL_ADSR_VAR_SUSTAIN,
  RTBUF_SIGNAL_ADSR_VAR_RELEASE,
  RTBUF_SIGNAL_ADSR_VAR_N,
};

typedef struct rtbuf_signal_adsr_data {
  double samples[RTBUF_SIGNAL_SAMPLES];
} s_rtbuf_signal_adsr_data;

int rtbuf_signal_adsr (s_rtbuf *rtb)
{
  s_rtbuf_signal_adsr_data *data;
  int attack = rtb->var[RTBUF_SIGNAL_ADSR_VAR_ATTACK];
  int decay = rtb->var[RTBUF_SIGNAL_ADSR_VAR_DECAY];
  int sustain = rtb->var[RTBUF_SIGNAL_ADSR_VAR_SUSTAIN];
  int release = rtb->var[RTBUF_SIGNAL_ADSR_VAR_RELEASE];
  double *attack_samples = attack < 0 ? 0 : g_rtbuf[attack].data;
  double *decay_samples = decay < 0 ? 0 : g_rtbuf[decay].data;
  double *sustain_samples = sustain < 0 ? 0 : g_rtbuf[sustain].data;
  double *release_samples = release < 0 ? 0 : g_rtbuf[release].data;
  double *sample;
  unsigned int i = 0;
  data = (s_rtbuf_signal_adsr_data*) rtb->data;
  sample = data->samples;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    *sample = adsr(
    sample++;
    i++;
  }
}
