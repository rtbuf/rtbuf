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
#ifndef RTBUF_GTK_RTBUF_INFO_H
#define RTBUF_GTK_RTBUF_INFO_H

#include "rtbuf_gtk3_widget.h"
#include "rtbuf_gtk3_input_widget.h"
#include "rtbuf_gtk3_output_widget.h"

typedef struct rtbuf_gtk_rtbuf_info {
  RtbufWidget *widget;
  short x;
  short y;
  RtbufInputWidget *in[RTBUF_PROC_IN_MAX];
  RtbufOutputWidget *out[RTBUF_PROC_OUT_MAX];
} s_rtbuf_gtk_rtbuf_info;

void rtbuf_gtk_rtbuf_info_init ();
s_rtbuf_gtk_rtbuf_info *rtbuf_gtk_rtbuf_info_new ();
void rtbuf_gtk_rtbuf_info_delete (s_rtbuf_gtk_rtbuf_info *info);

#endif /* RTBUF_GTK_RTBUF_INFO_H */
