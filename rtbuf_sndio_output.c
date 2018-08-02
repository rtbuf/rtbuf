
#include <limits.h>
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_sndio.h"

void rtbuf_sndio_output_parameters (struct sio_par *want)
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

int rtbuf_sndio_output_check_parameters (struct sio_par *have)
{
  print_sio_par(have); printf("\n");
  int ok = (have->bits == 16 &&
            have->sig == 1 &&
            have->rchan == 0 &&
            have->pchan == RTBUF_SNDIO_CHANNELS &&
            have->rate == RTBUF_SIGNAL_SAMPLERATE);
  return ok;
}

int rtbuf_sndio_output_start (s_rtbuf *rtb)
{
  s_rtbuf_sndio_output_data *data;
  int err = 0;
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  if (!data->sio_hdl) {
    data->sio_hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
    if (!data->sio_hdl)
      err = rtbuf_err("sndio_output_start: sio_open failed");
    else {
      rtbuf_sndio_output_parameters(&data->want);
      if (sio_setpar(data->sio_hdl, &data->want) != 1)
        err = rtbuf_err("sndio_output_start: sio_setpar failed");
      else if (sio_getpar(data->sio_hdl, &data->have) != 1)
        err = rtbuf_err("sndio_output_start: sio_getpar failed");
      else if (!rtbuf_sndio_output_check_parameters(&data->have))
        err = rtbuf_err("sndio_output_start: check_parameters failed");
      else if (sio_start(data->sio_hdl) != 1)
        err = rtbuf_err("sndio_output_start: sio_start failed");
    }
  }
  return err;
}

int rtbuf_sndio_output_stop (s_rtbuf *rtb)
{
  s_rtbuf_sndio_output_data *data;
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  if (data->sio_hdl) {
    sio_close(data->sio_hdl);
    data->sio_hdl = 0;
  }
  return 0;
}

int rtbuf_sndio_output (s_rtbuf *rtb)
{
  s_rtbuf_sndio_output_data *data;
  short *sample;
  unsigned int i = 0;
  unsigned int j = 0;
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  sample = data->samples;
  //printf("sndio_output");
  while (i < RTBUF_SIGNAL_SAMPLES) {
    j = 0;
    while (j < RTBUF_SNDIO_CHANNELS) {
      double in = rtbuf_signal_sample(rtb, j, i, 0.0);
      in = clamp(-1.0, in, 1.0);
      *sample = (short) (in < 0 ? in * -SHRT_MIN : in * SHRT_MAX);
      //printf(" %i", *sample);
      sample++;
      j++;
    }
    i++;
  }
  sio_write(data->sio_hdl, data->samples, sizeof(data->samples));
  //printf("\n");
  return 0;
}
