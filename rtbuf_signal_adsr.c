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

int rtbuf_signal_adsr (s_rtbuf *rtb)
{
  s_rtbuf_signal_adsr_data *data;
  double *sample;
  unsigned int i = 0;
  data = (s_rtbuf_signal_adsr_data*) rtb->data;
  sample = data->samples;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    if (data->start >= 0.0) {
      double a = rtbuf_signal_sample(rtb, RTBUF_SIGNAL_ADSR_VAR_ATTACK,
                                     i, 0.05);
      double d = rtbuf_signal_sample(rtb, RTBUF_SIGNAL_ADSR_VAR_DECAY,
                                     i, 0.02);
      double s = rtbuf_signal_sample(rtb, RTBUF_SIGNAL_ADSR_VAR_SUSTAIN,
                                     i, 0.666);
      double r = rtbuf_signal_sample(rtb, RTBUF_SIGNAL_ADSR_VAR_RELEASE,
                                     i, 0.2);
      double start = data->start + (double) i / RTBUF_SIGNAL_SAMPLERATE;
      double stop = data->stop;
      if (data->stop >= 0.0)
        data->stop += (double) i / RTBUF_SIGNAL_SAMPLERATE;
      a = max(0.0, a);
      d = max(0.0, d);
      s = max(0.0, s);
      r = max(0.0, r);
      *sample = adsr(a, d, s, r, start, stop);
    }
    else
      *sample = 0.0;
    sample++;
    i++;
  }
  if (data->start >= 0.0)
    data->start += RTBUF_SIGNAL_DT;
  if (data->stop >= 0.0)
    data->stop += RTBUF_SIGNAL_DT;
  return 0;
}

int rtbuf_signal_adsr_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_adsr_data *data;
  data = (s_rtbuf_signal_adsr_data*) rtb->data;
  data->start = 0.0;
  data->stop = -1.0;
  return 0;
}
