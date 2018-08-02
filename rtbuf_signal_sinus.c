
#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"

int rtbuf_signal_sinus (s_rtbuf *rtb)
{
  unsigned int i = 0;
  s_rtbuf_signal_sinus_data *data;
  double phase;
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  phase = data->phase;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = rtbuf_signal_sample(rtb,
                                   RTBUF_SIGNAL_SINUS_VAR_FREQUENCY,
                                   i, 220.0);
    double a = rtbuf_signal_sample(rtb,
                                   RTBUF_SIGNAL_SINUS_VAR_AMPLITUDE,
                                   i, 1.0);
    f = max(0.0, f);
    a = max(0.0, a);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    phase = phase + 2.0 * M_PI * f;
    while (phase >= 2.0 * M_PI)
      phase -= 2.0 * M_PI;
    data->signal[i] = a * sin(phase);
    //printf(" i=%u f=%f a=%f %f", i, f, a, data->samples[i]);
    i++;
  }
  data->phase = phase;
  return 0;
}

int rtbuf_signal_sinus_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_sinus_data *data;
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  data->phase = 0;
  return 0;
}
