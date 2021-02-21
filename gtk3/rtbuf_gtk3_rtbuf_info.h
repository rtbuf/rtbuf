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
