
#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"

int rtbuf_signal_sinus (s_rtbuf *rtb)
{
  double phase;
  unsigned int i = 0;
  s_rtbuf_signal_sinus_data *data;
  int freq = rtb->var[RTBUF_SIGNAL_SINUS_VAR_FREQUENCY];
  int amp = rtb->var[RTBUF_SIGNAL_SINUS_VAR_AMPLITUDE];
  double *freq_samples = freq < 0 ? 0 : (double*) g_rtbuf[freq].data;
  double *amp_samples = amp < 0 ? 0 : (double*) g_rtbuf[amp].data;
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  phase = data->phase;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq_samples ? freq_samples[i] : 220.0;
    double a = amp_samples ? amp_samples[i] : 1.0;
    f = max(0.0, f);
    a = max(0.0, a);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    phase = phase + 2.0 * M_PI * f;
    while (phase >= 2.0 * M_PI)
      phase -= 2.0 * M_PI;
    data->samples[i] = a * sin(phase);
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
