
#include <math.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"

enum {
  SINUS_FREQUENCY = 0,
  SINUS_AMPLITUDE,
  SINUS_ARITY
};

typedef struct sinus_data {
  double samples[RTBUF_SIGNAL_SAMPLES];
  double phase;
} s_sinus_data;

static int sinus (s_rtbuf *rtb);
static int sinus_start (s_rtbuf *rtb);

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_fun rtbuf_lib_fun[] = {
  { sinus, sinus_start, 0,
    { sizeof(s_sinus_data) / sizeof(double),
      sizeof(double),
      SINUS_ARITY } },
  { 0, 0, 0, { 0, 0, 0 } }
};

int sinus (s_rtbuf *rtb)
{
  double phase;
  unsigned int i = 0;
  s_sinus_data *data = (s_sinus_data*) rtb->data;
  int freq = rtb->var[SINUS_FREQUENCY];
  double *freq_samples = freq < 0 ? 0 : (double*) g_rtbuf[freq].data;
  phase = data->phase;
  while (i < RTBUF_SIGNAL_SAMPLES) {
    double f = freq ? freq_samples[i] : 220;
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

int sinus_start (s_rtbuf *rtb)
{
  s_sinus_data *data = (s_sinus_data*) rtb->data;
  data->phase = 0;
  return 0;
}
