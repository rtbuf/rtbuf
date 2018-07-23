
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"

enum {
  RTBUF_SNDIO_LEFT = 0,
  RTBUF_SNDIO_RIGHT,
  RTBUF_SNDIO_CHANNELS
};

typedef struct sndio_input_data {
  double samples[RTBUF_SNDIO_CHANNELS][RTBUF_SIGNAL_SAMPLES];
  short short_samples[RTBUF_SNDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES];
} s_sndio_input_data;

static int sndio_input (s_rtbuf *rtb);
static int sndio_input_start (s_rtbuf *rtb);
static int sndio_input_stop (s_rtbuf *rtb);

typedef struct sndio_output_data {
  short samples[RTBUF_SNDIO_CHANNELS * RTBUF_SIGNAL_SAMPLES];
  struct sio_hdl *sio_hdl;
  struct sio_par want;
  struct sio_par have;
} s_sndio_output_data;

static int sndio_output (s_rtbuf *rtb);
static int sndio_output_start (s_rtbuf *rtb);
static int sndio_output_stop (s_rtbuf *rtb);

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_fun rtbuf_lib_fun[] = {
  { sndio_input, sndio_input_start, sndio_input_stop,
    { sizeof(s_sndio_input_data) / sizeof(double), sizeof(double),
      0 } },
  { sndio_output, sndio_output_start, sndio_output_stop,
    { 1, sizeof(s_sndio_output_data), RTBUF_SNDIO_CHANNELS } },
  { 0, 0, 0, { 0, 0, 0 } }
};

int sndio_input (s_rtbuf *rtb)
{
  /*
    s_sndio_input_data *data = (s_sndio_input_data*) rtb->data;
  */
  (void) rtb;
  return 0;
}

int sndio_input_start (s_rtbuf *rtb)
{
  (void) rtb;
  return 0;
}

int sndio_input_stop (s_rtbuf *rtb)
{
  (void) rtb;
  return 0;
}

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}

void sndio_output_parameters (struct sio_par *want)
{
  bzero(want, sizeof(struct sio_par));
  sio_initpar(want);
  want->bits = 16;
  want->sig = 1;
  want->rchan = 0;
  want->pchan = RTBUF_SNDIO_CHANNELS;
  want->rate = RTBUF_SIGNAL_SAMPLERATE;
  want->appbufsz = RTBUF_SIGNAL_SAMPLES;
  want->xrun = SIO_SYNC;
  print_sio_par(want); printf("\n");
}

int sndio_output_check_parameters (struct sio_par *have)
{
  print_sio_par(have); printf("\n");
  int ok = (have->bits == 16 &&
            have->sig == 1 &&
            have->rchan == 0 &&
            have->pchan == RTBUF_SNDIO_CHANNELS &&
            have->rate == RTBUF_SIGNAL_SAMPLERATE);
  return ok;
}

int sndio_output_start (s_rtbuf *rtb)
{
  s_sndio_output_data *data = (s_sndio_output_data*) rtb->data;
  int err = 0;
  if (!data->sio_hdl) {
    data->sio_hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
    if (!data->sio_hdl)
      err = rtbuf_err("sndio_output_start: sio_open failed");
    else {
      sndio_output_parameters(&data->want);
      if (sio_setpar(data->sio_hdl, &data->want) != 1)
        err = rtbuf_err("sndio_output_start: sio_setpar failed");
      else if (sio_getpar(data->sio_hdl, &data->have) != 1)
        err = rtbuf_err("sndio_output_start: sio_getpar failed");
      else if (!sndio_output_check_parameters(&data->have))
        err = rtbuf_err("sndio_output_start: check_parameters failed");
      else if (sio_start(data->sio_hdl) != 1)
        err = rtbuf_err("sndio_output_start: sio_start failed");
    }
  }
  return err;
}

int sndio_output_stop (s_rtbuf *rtb)
{
  s_sndio_output_data *data = (s_sndio_output_data*) rtb->data;
  if (data->sio_hdl) {
    sio_close(data->sio_hdl);
    data->sio_hdl = 0;
  }
  return 0;
}

double min (double a, double b)
{
  return a < b ? a : b;
}

double max (double a, double b)
{
  return a > b ? a : b;
}

int sndio_output (s_rtbuf *rtb)
{
  s_sndio_output_data *data = (s_sndio_output_data*) rtb->data;
  double *in[RTBUF_SNDIO_CHANNELS];
  short *sample = data->samples;
  unsigned int i = 0;
  unsigned int j = 0;
  //printf("sndio_output");
  while (j < RTBUF_SNDIO_CHANNELS) {
    in[j] = rtb->var[j] < 0 ? 0 : (double*) g_rtbuf[rtb->var[j]].data;
    j++;
  }
  while (i < RTBUF_SIGNAL_SAMPLES) {
    j = 0;
    while (j < RTBUF_SNDIO_CHANNELS) {
      if (in[j]) {
        double limit = max(-1.0, min(*in[j], 1.0));
        in[j]++;
        *sample = (short) (((1 << 15) - 1) * limit);
        //printf(" %i", *sample);
      }
      else
        *sample = 0;
      sample++;
      j++;
    }
    i++;
  }
  sio_write(data->sio_hdl, data->samples,
            sizeof(data->samples));
  //printf("\n");
  return 0;
}

int rtbuf_err (const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  return -1;
}
