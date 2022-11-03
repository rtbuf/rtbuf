/* c3
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
#ifndef RTBUF_GTK_H
#define RTBUF_GTK_H

#include <gtk/gtk.h>
#include "rtbuf_gtk3_connection.h"
#include "rtbuf_gtk3_rtbuf_info.h"
#include "rtbuf_gtk3_input_widget.h"
#include "rtbuf_gtk3_output_widget.h"
#include "rtbuf_gtk3_widget.h"

typedef struct signal_binding {
  const char *signal;
  GCallback callback;
} s_signal_binding;

extern GtkScrolledWindow      *modular_scrolled_window;
extern GtkLayout              *modular_layout;
extern s_rtbuf_gtk_connection *modular_connections;

extern GtkWidget              *drag_widget;
extern s_rtbuf_gtk_connection *drag_connection;
extern gint                    drag_x;
extern gint                    drag_y;

void rtbuf_gtk_drag_connection_end (RtbufInputWidget *input_widget);

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data);

gboolean rtbuf_gtk_input_button_press (RtbufInputWidget *widget,
                                       GdkEvent *event);

gboolean rtbuf_gtk_output_check_button_press (RtbufOutputWidget *widget,
                                              GdkEvent *event);
gboolean rtbuf_gtk_output_check_button_release (RtbufOutputWidget *widget,
                                                GdkEvent *event);

#endif /* RTBUF_GTK_H */
