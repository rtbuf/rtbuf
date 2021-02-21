/*
 * Copyright 2020-2021 Thomas de Grivel <thoxdg@gmail.com>
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

#ifndef RTBUF_INPUT_WIDGET_H
#define RTBUF_INPUT_WIDGET_H

#include "../librtbuf/rtbuf.h"

#define RTBUF_INPUT_WIDGET_TYPE \
  (rtbuf_input_widget_get_type())

#define RTBUF_INPUT_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), RTBUF_INPUT_WIDGET_TYPE, \
                              RtbufInputWidget))

#define RTBUF_INPUT_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), RTBUF_INPUT_WIDGET_TYPE, \
                           RtbufInputWidgetClass))

#define IS_RTBUF_INPUT_WIDGET(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), RTBUF_INPUT_WIDGET_TYPE))

#define IS_RTBUF_INPUT_WIDGET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), RTBUF_INPUT_WIDGET_TYPE))

#define RTBUF_INPUT_WIDGET_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), RTBUF_INPUT_WIDGET_TYPE, \
                             RtbufInputWidgetClass))

typedef struct _RtbufInputWidget RtbufInputWidget;
typedef struct _RtbufInputWidgetClass RtbufInputWidgetClass;
typedef struct _RtbufInputWidgetPrivate RtbufInputWidgetPrivate;

struct _RtbufInputWidget {
  GtkBox box;
};

struct _RtbufInputWidgetClass {
  GtkBoxClass parent_class;
};

GType         rtbuf_input_widget_get_type (void) G_GNUC_CONST;
RtbufInputWidget * rtbuf_input_widget_new (s_rtbuf *rtbuf,
                                           unsigned int in);
s_rtbuf *     rtbuf_input_widget_get_rtbuf (RtbufInputWidget *widget);
unsigned int  rtbuf_input_widget_get_in    (RtbufInputWidget *widget);
GtkWidget *   rtbuf_input_widget_get_check (RtbufInputWidget *widget);
void          rtbuf_input_widget_update_rtbuf_in (RtbufInputWidget *widget);
void          rtbuf_input_widget_connect (GtkWidget *input,
                                          gpointer data);
void          rtbuf_input_widget_connect_check (GtkWidget *input,
                                                gpointer data);

#endif
