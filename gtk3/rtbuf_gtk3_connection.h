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
#ifndef RTBUF_GTK_CONNECTION_H
#define RTBUF_GTK_CONNECTION_H

#include "rtbuf_gtk3_input_widget.h"
#include "rtbuf_gtk3_output_widget.h"

typedef struct rtbuf_gtk_connection s_rtbuf_gtk_connection;

struct rtbuf_gtk_connection {
  RtbufOutputWidget *output_widget;
  RtbufInputWidget *input_widget;
  s_rtbuf_gtk_connection *next;
};

void rtbuf_gtk_connection_init ();
s_rtbuf_gtk_connection * rtbuf_gtk_connection_new ();
void rtbuf_gtk_connection_delete (s_rtbuf_gtk_connection *connection);

void rtbuf_gtk_connection_push (s_rtbuf_gtk_connection **head,
                                s_rtbuf_gtk_connection *item);
int rtbuf_gtk_connection_remove_one (s_rtbuf_gtk_connection **head,
                                     s_rtbuf_gtk_connection *item);
s_rtbuf_gtk_connection * rtbuf_gtk_connection_find
(s_rtbuf_gtk_connection *c,
 RtbufOutputWidget *output_widget,
 RtbufInputWidget *input_widget);

void rtbuf_gtk_connection_remove_rtbuf (s_rtbuf_gtk_connection **head,
                                        s_rtbuf *rtbuf);

#endif
