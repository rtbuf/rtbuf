/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <limits.h>
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"
#include "../sndio.h"
#include "../sndio_type.h"

s_rtbuf_lib_proc_in rtbuf_sndio_output_in[] =
  {{ "left",  RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "right", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_sndio_output_out[] =
  {{ "samples", RTBUF_SNDIO_SAMPLES_TYPE },
   { "reserved", RTBUF_SNDIO_OUTPUT_RESERVED_TYPE },
   { 0, 0 }};

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
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_sndio_output_data*) rtb->data;
  res = &data->reserved;
  if (!res->sio_hdl) {
    res->sio_hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
    if (!res->sio_hdl)
      return rtbuf_err("sndio_output_start: sio_open failed");
    else {
      rtbuf_sndio_output_parameters(&res->want);
      if (sio_setpar(res->sio_hdl,
                     &res->want) != 1)
        return rtbuf_err("sndio_output_start: sio_setpar failed");
      else if (sio_getpar(res->sio_hdl,
                          &res->have) != 1)
        return rtbuf_err("sndio_output_start: sio_getpar failed");
      else if (!rtbuf_sndio_output_check_parameters(&res->have))
        return rtbuf_err("sndio_output_start: check_parameters failed");
      else if (sio_start(res->sio_hdl) != 1)
        return rtbuf_err("sndio_output_start: sio_start failed");
    }
  }
  return 0;
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

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "output",
    rtbuf_sndio_output,
    rtbuf_sndio_output_start,
    rtbuf_sndio_output_stop,
    rtbuf_sndio_output_in,
    rtbuf_sndio_output_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}
