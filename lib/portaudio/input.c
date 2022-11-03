/* rtbuf
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
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
  data = (s_rtbuf_portaudio_input_data*) rtb->data;
  (void) data;
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
