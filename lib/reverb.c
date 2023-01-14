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
