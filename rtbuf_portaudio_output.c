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
#include <portaudio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_portaudio.h"

int g_initialized = 0;

int rtbuf_portaudio_output_start (s_rtbuf *rtb)
{
  s_rtbuf_portaudio_output_data *data;
  s_rtbuf_portaudio_output_reserved *res;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  if (!g_initialized) {
    if (Pa_Initialize() != paNoError)
      return rtbuf_err("portaudio_output_start: Pa_Initialize() failed");
    g_initialized = 1;
  }
  res = &data->reserved;
  if (!res->stream) {
    if (Pa_OpenDefaultStream(&res->stream,              /* stream pointer */
                             0,                         /* input channels */
                             RTBUF_PORTAUDIO_CHANNELS,  /* output channels */
                             paFloat32,                 /* sample format */
                             RTBUF_SIGNAL_SAMPLERATE,   /* sample rate */
                             RTBUF_SIGNAL_SAMPLES,      /* frames per buffer */
                             NULL,                      /* stream callback */
                             (void*) rtb)               /* user data */
        != paNoError)
      return rtbuf_err("portaudio_output_start: Pa_OpenDefaultStream() failed");
  }
  return 0;
}

int rtbuf_portaudio_output_stop (s_rtbuf *rtb)
{
  s_rtbuf_portaudio_output_data *data;
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  if (data->reserved.stream) {
    Pa_CloseStream(data->reserved.stream);
    data->reserved.stream = 0;
  }
  return 0;
}

int rtbuf_portaudio_output (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in[RTBUF_PORTAUDIO_CHANNELS];
  s_rtbuf_portaudio_output_data *data;
  double *sample;
  unsigned int i = 0;
  unsigned int j = 0;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->proc);
  while (j < RTBUF_PORTAUDIO_CHANNELS) {
    rtbuf_signal_fun(rtb, j, &in[j]);
    j++;
  }
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  sample = data->samples;
  //printf("portaudio_output");
  while (i < RTBUF_SIGNAL_SAMPLES) {
    j = 0;
    while (j < RTBUF_PORTAUDIO_CHANNELS) {
      double in_ = in[j].sample_fun(in[j].signal, i);
      in_ = clamp(-1.0, in_, 1.0);
      *sample = in_;
      sample++;
      j++;
    }
    i++;
  }
  Pa_WriteStream(data->reserved.stream, data->samples, RTBUF_SIGNAL_SAMPLES);
  //printf("\n");
  return 0;
}