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

#ifndef RTBUF_OUTPUT_WIDGET_H
#define RTBUF_OUTPUT_WIDGET_H

#include "rtbuf.h"

#define RTBUF_OUTPUT_WIDGET_TYPE \
  (rtbuf_output_widget_get_type())

#define RTBUF_OUTPUT_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), RTBUF_OUTPUT_WIDGET_TYPE, \
                              RtbufOutputWidget))

#define RTBUF_OUTPUT_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), RTBUF_OUTPUT_WIDGET_TYPE, \
                           RtbufOutputWidgetClass))

#define IS_RTBUF_OUTPUT_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), RTBUF_OUTPUT_WIDGET_TYPE))

#define IS_RTBUF_OUTPUT_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), RTBUF_OUTPUT_WIDGET_TYPE))

#define RTBUF_OUTPUT_WIDGET_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), RTBUF_OUTPUT_WIDGET_TYPE, \
                             RtbufOutputWidgetClass))

typedef struct _RtbufOutputWidget RtbufOutputWidget;
typedef struct _RtbufOutputWidgetClass RtbufOutputWidgetClass;
typedef struct _RtbufOutputWidgetPrivate RtbufOutputWidgetPrivate;

struct _RtbufOutputWidget {
  GtkBox box;
};

struct _RtbufOutputWidgetClass {
  GtkBoxClass parent_class;
};

GType         rtbuf_output_widget_get_type (void) G_GNUC_CONST;
RtbufOutputWidget * rtbuf_output_widget_new (s_rtbuf *rtbuf,
                                             unsigned int out);
s_rtbuf *     rtbuf_output_widget_get_rtbuf (RtbufOutputWidget *widget);
unsigned int  rtbuf_output_widget_get_out   (RtbufOutputWidget *widget);
GtkWidget *   rtbuf_output_widget_get_check (RtbufOutputWidget *widget);
void          rtbuf_output_widget_update    (RtbufOutputWidget *widget);

#endif
