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
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_signal_type.h"
#include "rtbuf_sndio.h"
#include "rtbuf_sndio_type.h"

s_rtbuf_lib_proc_out g_rtbuf_sndio_input_out[] = {
  { "left", RTBUF_SIGNAL_TYPE },
  { "right", RTBUF_SIGNAL_TYPE },
  { "samples", RTBUF_SNDIO_SAMPLES_TYPE },
  { 0, 0 } };

s_rtbuf_lib_proc_in g_rtbuf_sndio_output_in[] = {
  { "left", RTBUF_SIGNAL_TYPE, -1.0, 1.0 },
  { "right", RTBUF_SIGNAL_TYPE, -1.0, 1.0 },
  { 0, 0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out g_rtbuf_sndio_output_out[] = {
  { "samples", RTBUF_SNDIO_SAMPLES_TYPE },
  { "reserved", RTBUF_SNDIO_OUTPUT_RESERVED_TYPE },
  { 0, 0 } };

const char     *rtbuf_lib_name = "sndio";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_proc rtbuf_lib_proc[] = {
  { "input", rtbuf_sndio_input, rtbuf_sndio_input_start,
    rtbuf_sndio_input_stop, 0, g_rtbuf_sndio_input_out },
  { "output", rtbuf_sndio_output, rtbuf_sndio_output_start,
    rtbuf_sndio_output_stop, g_rtbuf_sndio_output_in,
    g_rtbuf_sndio_output_out },
  { 0, 0, 0, 0, 0, 0 } };

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}
