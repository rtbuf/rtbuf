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
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>
#include <rtbuf/portaudio.h>
#include <rtbuf/portaudio_type.h>

int g_initialized = 0;

s_rtbuf_lib_proc_in rtbuf_portaudio_output_in[] =
  {{ "left",  RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { "right", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 1.0 },
   { 0, 0, 0.0, 0.0, 0.0, 0.0 }};

s_rtbuf_lib_proc_out rtbuf_portaudio_output_out[] =
  {{ "samples", RTBUF_PORTAUDIO_SAMPLES_TYPE },
   { "reserved", RTBUF_PORTAUDIO_OUTPUT_RESERVED_TYPE },
   { 0, 0 }};

int rtbuf_portaudio_output_start (s_rtbuf *rtb)
{
  s_rtbuf_portaudio_output_data *data;
  PaError err;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  if (!g_initialized) {
    if (Pa_Initialize() != paNoError)
      return rtbuf_err("portaudio_output_start: Pa_Initialize() failed");
    g_initialized = 1;
  }
  err = Pa_OpenDefaultStream(&data->reserved.stream,    /* stream pointer */
                             0,                         /* input channels */
                             RTBUF_PORTAUDIO_CHANNELS,  /* output channels */
                             paInt16,                   /* sample format */
                             RTBUF_SIGNAL_SAMPLERATE,   /* sample rate */
                             RTBUF_SIGNAL_SAMPLES,      /* frames per buffer */
                             NULL,                      /* stream callback */
                             NULL);                     /* user data */
  if (err != paNoError) {
    rtbuf_err(Pa_GetErrorText(err));
    return rtbuf_err("portaudio_output_start: Pa_OpenDefaultStream() failed");
  }
  err = Pa_StartStream(data->reserved.stream);
  if (err != paNoError) {
    rtbuf_err(Pa_GetErrorText(err));
    return rtbuf_err("portaudio output start: Pa_StartStream() failed");
  }
  return 0;
}

int rtbuf_portaudio_output_stop (s_rtbuf *rtb)
{
  s_rtbuf_portaudio_output_data *data;
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  if (data->reserved.stream) {
    Pa_StopStream(data->reserved.stream);
    Pa_CloseStream(data->reserved.stream);
    data->reserved.stream = 0;
  }
  return 0;
}

int rtbuf_portaudio_output (s_rtbuf *rtb)
{
  s_rtbuf_signal_fun in[RTBUF_PORTAUDIO_CHANNELS];
  s_rtbuf_portaudio_output_data *data;
  short *sample;
  unsigned int i = 0;
  unsigned int j = 0;
  PaError err;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->proc);
  while (j < RTBUF_PORTAUDIO_CHANNELS) {
    rtbuf_signal_fun(rtb, j, &in[j]);
    j++;
  }
  data = (s_rtbuf_portaudio_output_data*) rtb->data;
  sample = data->samples;
  /* printf("portaudio_output"); */
  while (i < RTBUF_SIGNAL_SAMPLES) {
    j = 0;
    while (j < RTBUF_PORTAUDIO_CHANNELS) {
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
  err = Pa_WriteStream(data->reserved.stream, data->samples, RTBUF_SIGNAL_SAMPLES);
  if (err != paNoError) {
    rtbuf_err(Pa_GetErrorText(err));
    rtbuf_err("portaudio output: Pa_WriteStream failed");
  }
  /* printf("\n"); */
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "output",
    rtbuf_portaudio_output,
    rtbuf_portaudio_output_start,
    rtbuf_portaudio_output_stop,
    rtbuf_portaudio_output_in,
    rtbuf_portaudio_output_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
