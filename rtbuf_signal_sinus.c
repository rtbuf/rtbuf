
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
  double *freq_samples = freq < 0 ? 0 : (double*) g_rtbuf[freq].data;
  data = (s_rtbuf_signal_sinus_data*) rtb->data;
  phase = data->phase;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq_samples ? freq_samples[i] : 220;
    //printf(" i=%u freq=%f", i, f);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    phase = phase + 2.0 * M_PI * f;
    data->samples[i] = sin(phase);
    //printf(" sin=%f", data->samples[i]);
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
