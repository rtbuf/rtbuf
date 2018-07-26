
#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"

double square (double amp, double phase, double pulse)
{
  return phase < pulse ? amp : -amp;
}

int rtbuf_signal_square (s_rtbuf *rtb)
{
  double phase;
  unsigned int i = 0;
  s_rtbuf_signal_square_data *data;
  int freq = rtb->var[RTBUF_SIGNAL_SQUARE_VAR_FREQUENCY];
  int amp = rtb->var[RTBUF_SIGNAL_SQUARE_VAR_AMPLITUDE];
  int pulse = rtb->var[RTBUF_SIGNAL_SQUARE_VAR_PULSE];
  double *freq_samples = freq < 0 ? 0 : (double*) g_rtbuf[freq].data;
  double *amp_samples = amp < 0 ? 0 : (double*) g_rtbuf[amp].data;
  double *pulse_samples = pulse < 0 ? 0 : (double*) g_rtbuf[pulse].data;
  data = (s_rtbuf_signal_square_data*) rtb->data;
  phase = data->phase;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq_samples  ? freq_samples[i]  : 220.0;
    double a = amp_samples   ? amp_samples[i]   : 1.0;
    double p = pulse_samples ? pulse_samples[i] : 0.5;
    f = max(0.0, f);
    a = max(0.0, a);
    p = clamp(0.0, p, 1.0);
    //printf(" i=%u freq=%f amp=%f pulse=%f", i, f, a, p);
    f /= (double) RTBUF_SIGNAL_SAMPLERATE;
    phase = phase + f;
    phase = phase - floor(phase);
    data->samples[i] = square(a, phase, p);
    //printf(" f=%f a=%f p=%f square=%f", f, a, p, data->samples[i]);
    i++;
  }
  data->phase = phase;
  return 0;
}

int rtbuf_signal_square_start (s_rtbuf *rtb)
{
  s_rtbuf_signal_square_data *data;
  data = (s_rtbuf_signal_square_data*) rtb->data;
  data->phase = 0;
  return 0;
}
