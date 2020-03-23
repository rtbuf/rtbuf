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

#ifndef RTBUF_WIDGET_H
#define RTBUF_WIDGET_H

#include "rtbuf.h"

#define RTBUF_WIDGET_TYPE \
  (rtbuf_widget_get_type())

#define RTBUF_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), RTBUF_WIDGET_TYPE, \
                              RtbufWidget))

#define RTBUF_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), RTBUF_WIDGET_TYPE, \
                           RtbufWidgetClass))

#define IS_RTBUF_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), RTBUF_WIDGET_TYPE))

#define IS_RTBUF_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), RTBUF_WIDGET_TYPE))

#define RTBUF_WIDGET_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), RTBUF_WIDGET_TYPE, \
                             RtbufWidgetClass))

typedef struct _RtbufWidget RtbufWidget;
typedef struct _RtbufWidgetClass RtbufWidgetClass;
typedef struct _RtbufWidgetPrivate RtbufWidgetPrivate;

struct _RtbufWidget {
  GtkBox box;
};

struct _RtbufWidgetClass {
  GtkBoxClass parent_class;
};

GType         rtbuf_widget_get_type (void) G_GNUC_CONST;
RtbufWidget * rtbuf_widget_new (s_rtbuf *rtbuf, const gchar *label);
void          rtbuf_widget_set_rtbuf (RtbufWidget *widget,
                                      s_rtbuf *rtbuf);
void          rtbuf_widget_set_label (RtbufWidget *widget,
                                      const gchar *label);
const gchar * rtbuf_widget_get_label (RtbufWidget *widget);
GtkWidget *   rtbuf_widget_get_event_box (RtbufWidget *widget);
s_rtbuf *     rtbuf_widget_get_rtbuf (RtbufWidget *widget);
void          rtbuf_widget_connect_inputs (RtbufWidget *widget,
                                           const char *signal,
                                           GCallback callback);
void          rtbuf_widget_connect_input_checks (RtbufWidget *widget,
                                                 const char *signal,
                                                 GCallback callback);
void          rtbuf_widget_connect_output_checks (RtbufWidget *widget,
                                                  const char *signal,
                                                  GCallback callback);

#endif
