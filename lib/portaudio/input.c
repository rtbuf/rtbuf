/*
 * Copyright 2018-2021 Thomas de Grivel <thoxdg@gmail.com>
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

#include <portaudio.h>
#include <stdio.h>
#include <strings.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"
#include "../portaudio.h"
#include "../portaudio_type.h"

s_rtbuf_lib_proc_out rtbuf_portaudio_input_out[] = {
  { "left",    RTBUF_SIGNAL_TYPE },
  { "right",   RTBUF_SIGNAL_TYPE },
  { "samples", RTBUF_PORTAUDIO_SAMPLES_TYPE },
  { 0, 0 }};

int rtbuf_portaudio_input (s_rtbuf *rtb)
{
  /*
    s_portaudio_input_data *data = (s_portaudio_input_data*) rtb->data;
  */
  (void) rtb;
  return 0;
}

int rtbuf_portaudio_input_start (s_rtbuf *rtb)
{
  s_rtbuf_portaudio_input_data *data;
  if (rtb->proc->out_bytes < sizeof(*data))
    return 1;
  /* data = (s_rtbuf_portaudio_input_data*) rtb->data; */
  return 0;
}

int rtbuf_portaudio_input_stop (s_rtbuf *rtb)
{
  (void) rtb;
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "input",
    rtbuf_portaudio_input,
    rtbuf_portaudio_input_start,
    rtbuf_portaudio_input_stop,
    0,
    rtbuf_portaudio_input_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
