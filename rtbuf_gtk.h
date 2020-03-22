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
#ifndef RTBUF_GTK_H
#define RTBUF_GTK_H

#include <gtk/gtk.h>
#include "rtbuf_gtk_connection.h"
#include "rtbuf_input_widget.h"
#include "rtbuf_output_widget.h"
#include "rtbuf_widget.h"

enum dnd_targets {
  TARGET_RTBUF,
  TARGET_RTBUF_OUTPUT,
  N_TARGETS
};

typedef struct signal_binding {
  const char *signal;
  GCallback callback;
} s_signal_binding;

GtkLayout              *modular_layout;
s_rtbuf_gtk_connection *modular_connections;

GtkTargetList *rtbuf_move_target_list;

GtkWidget *drag_widget;
gint       drag_x;
gint       drag_y;

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data);

gboolean rtbuf_gtk_input_check_button_press (RtbufInputWidget *widget,
                                             GdkEvent *event);

void     rtbuf_gtk_output_init ();
gboolean rtbuf_gtk_output_check_button_press (RtbufOutputWidget *widget,
                                              GdkEvent *event);

#endif /* RTBUF_GTK_H */
