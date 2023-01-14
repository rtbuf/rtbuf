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
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "../../librtbuf/rtbuf.h"
#include "../../librtbuf/lib.h"
#include "../signal.h"
#include "../signal_type.h"
#include "../sndio.h"
#include "../sndio_type.h"

s_rtbuf_lib_proc_out rtbuf_sndio_input_out[] = {
  { "left", RTBUF_SIGNAL_TYPE },
  { "right", RTBUF_SIGNAL_TYPE },
  { "samples", RTBUF_SNDIO_SAMPLES_TYPE },
  { 0, 0 } };

int rtbuf_sndio_input (s_rtbuf *rtb)
{
  /*
    s_sndio_input_data *data = (s_sndio_input_data*) rtb->data;
  */
  (void) rtb;
  return 0;
}

int rtbuf_sndio_input_start (s_rtbuf *rtb)
{
  s_rtbuf_sndio_input_data *data;
  if (rtb->proc->out_bytes != sizeof(*data))
    return 1;
  data = (s_rtbuf_sndio_input_data*) rtb->data;
  (void) data;
  return 0;
}

int rtbuf_sndio_input_stop (s_rtbuf *rtb)
{
  (void) rtb;
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "input",
    rtbuf_sndio_input,
    rtbuf_sndio_input_start,
    rtbuf_sndio_input_stop,
    0,
    rtbuf_sndio_input_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}
