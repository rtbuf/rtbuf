/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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
  want->xrun = SIO_IGNORE;
  print_sio_par(want); printf("\n");
}

int rtbuf_sndio_output_check_parameters (struct sio_par *have)
{
  int ok;
  print_sio_par(have);
  printf("\n");
  ok = (have->bits == 16 &&
        have->sig == 1 &&
        have->rchan == 0 &&
        have->pchan == RTBUF_SNDIO_CHANNELS &&
        have->rate == RTBUF_SIGNAL_SAMPLERATE);
  return ok;
}

int rtbuf_sndio_output_start (s_rtbuf *rtb)
{
  s_rtbuf_sndio_output_data *data;
  s_rtbuf_sndio_output_reserved *res;
  int err = 0;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  res = &data->reserved;
  if (!res->sio_hdl) {
    res->sio_hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
    if (!res->sio_hdl)
      err = rtbuf_err("sndio_output_start: sio_open failed");
    else {
      rtbuf_sndio_output_parameters(&res->want);
      if (sio_setpar(res->sio_hdl,
                     &res->want) != 1)
        err = rtbuf_err("sndio_output_start: sio_setpar failed");
      else if (sio_getpar(res->sio_hdl,
                          &res->have) != 1)
        err = rtbuf_err("sndio_output_start: sio_getpar failed");
      else if (!rtbuf_sndio_output_check_parameters(&res->have))
        err = rtbuf_err("sndio_output_start: check_parameters failed");
      else if (sio_start(res->sio_hdl) != 1)
        err = rtbuf_err("sndio_output_start: sio_start failed");
    }
  }
  return err;
}

int rtbuf_sndio_output_stop (s_rtbuf *rtb)
{
  s_rtbuf_sndio_output_data *data;
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  if (data->reserved.sio_hdl) {
    sio_close(data->reserved.sio_hdl);
    data->reserved.sio_hdl = 0;
  }
  return 0;
}

int rtbuf_sndio_output (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in[RTBUF_SNDIO_CHANNELS];
  s_rtbuf_sndio_output_data *data;
  short *sample;
  unsigned int i = 0;
  unsigned int j = 0;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->proc);
  while (j < RTBUF_SNDIO_CHANNELS) {
    rtbuf_signal_fun(rtb, j, &in[j]);
    j++;
  }
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  sample = data->samples;
  /* printf("sndio_output"); */
  while (i < RTBUF_SIGNAL_SAMPLES) {
    j = 0;
    while (j < RTBUF_SNDIO_CHANNELS) {
      double in_ = in[j].sample_fun(in[j].signal, i);
      in_ = clamp(-1.0, in_, 1.0);
      if (in_ < 0.0)
        in_ *= -SHRT_MIN;
      else
        in_ *= SHRT_MAX;
      *sample = (short) in_;
      sample++;
      j++;
    }
    i++;
  }
  sio_write(data->reserved.sio_hdl, data->samples,
            sizeof(t_rtbuf_sndio_samples));
  /* printf("\n"); */
  return 0;
}
