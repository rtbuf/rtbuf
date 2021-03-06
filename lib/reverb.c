/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
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

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include "../librtbuf/rtbuf.h"
#include "../librtbuf/lib.h"
#include "signal.h"
#include "signal_type.h"
#include "reverb.h"
#include "reverb_type.h"

s_rtbuf_lib_proc_in g_rtbuf_reverb_fdn4_in[] = {
  { "signal",    RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0, 2.0 },
  { "time",      RTBUF_SIGNAL_TYPE, 1.0, 0.0, 100.0, 10.0 },
  { "dry_level", RTBUF_SIGNAL_TYPE, 0.8, 0.0, 1.0, 2.0 },
  { "wet_level", RTBUF_SIGNAL_TYPE, 0.2, 0.0, 1.0, 2.0 },
  { 0, 0, 0.0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out g_rtbuf_reverb_fdn4_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "d0", RTBUF_REVERB_FDN4_DELAY_TYPE },
  { "d1", RTBUF_REVERB_FDN4_DELAY_TYPE },
  { "d2", RTBUF_REVERB_FDN4_DELAY_TYPE },
  { "d3", RTBUF_REVERB_FDN4_DELAY_TYPE },
  { "pos", "unsigned int" },
  { 0, 0 } };

const char     *rtbuf_lib_name = "reverb";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_proc rtbuf_lib_proc[] = {
  { "fdn4", rtbuf_reverb_fdn4, rtbuf_reverb_fdn4_start, 0,
    g_rtbuf_reverb_fdn4_in, g_rtbuf_reverb_fdn4_out },
  { 0, 0, 0, 0, 0, 0 } };
