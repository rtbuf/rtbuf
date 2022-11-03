/* rtbuf
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */

#ifndef RTBUF_OUTPUT_WIDGET_H
#define RTBUF_OUTPUT_WIDGET_H

#include "../librtbuf/rtbuf.h"

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
void          rtbuf_output_widget_connect_check (GtkWidget *output,
                                                 gpointer data);

#endif
