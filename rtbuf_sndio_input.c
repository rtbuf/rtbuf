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

#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"
#include "rtbuf_sndio.h"

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
  assert(rtb->fun->out_bytes == sizeof(*data));
  data = (s_rtbuf_sndio_input_data*) rtb->data;
  return 0;
}

int rtbuf_sndio_input_stop (s_rtbuf *rtb)
{
  (void) rtb;
  return 0;
}
