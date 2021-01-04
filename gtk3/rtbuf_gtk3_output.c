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

#include <assert.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <rtbuf/rtbuf.h>
#include "rtbuf_gtk.h"
#include "rtbuf_output_widget.h"

void rtbuf_gtk_output_disconnect (RtbufOutputWidget *widget)
{
  s_rtbuf *rtbuf;
  unsigned int out;
  printf("rtbuf-gtk output disconnect\n");
  rtbuf = rtbuf_output_widget_get_rtbuf(widget);
  out = rtbuf_output_widget_get_out(widget);
  rtbuf_out_unbind(rtbuf, out);
}

void rtbuf_gtk_output_menu (RtbufOutputWidget *widget, GdkEvent *event)
{
  static GtkMenu *menu = 0;
  static GtkWidget *disconnect;
  static guint signal;
  printf("rtbuf-gtk output menu\n");
  if (!menu) {
    signal = g_signal_lookup("activate", GTK_TYPE_MENU_ITEM);
    assert(signal);
    menu = GTK_MENU(gtk_menu_new());
    disconnect = gtk_menu_item_new_with_label("Disconnect");
    gtk_container_add(GTK_CONTAINER(menu), disconnect);
    gtk_widget_show(disconnect);
  }
  g_signal_handlers_disconnect_matched(G_OBJECT(disconnect),
                                       G_SIGNAL_MATCH_FUNC,
                                       signal,
                                       0,
                                       NULL,
                                       G_CALLBACK(rtbuf_gtk_output_disconnect),
                                       NULL);
  g_signal_connect_swapped(G_OBJECT(disconnect), "activate",
                           G_CALLBACK(rtbuf_gtk_output_disconnect),
                           widget);
  gtk_menu_popup_at_pointer(menu, event);
}

void rtbuf_gtk_output_drag (RtbufOutputWidget *widget,
                            GdkEventButton *event)
{
  s_rtbuf_gtk_connection *connection;
  (void) event;
  printf("rtbuf-gtk output drag\n");
  connection = rtbuf_gtk_connection_new();
  if (!connection) {
    rtbuf_err("failed to allocate rtbuf_gtk_connection");
    return;
  }
  connection->output_widget = widget;
  rtbuf_gtk_connection_push(&modular_connections, connection);
  drag_connection = connection;
}

gboolean rtbuf_gtk_output_check_button_press (RtbufOutputWidget *widget,
                                              GdkEvent *event)
{
  printf("rtbuf-gtk output check button press\n");
  if (event->type == GDK_BUTTON_PRESS) {
    GdkEventButton *eb = (GdkEventButton*) event;
    if (eb->button == 1) {
      rtbuf_gtk_output_drag(widget, eb);
      return TRUE;
    }
    else if (eb->button == 3) {
      rtbuf_gtk_output_menu(widget, event);
      return TRUE;
    }
  }
  return FALSE;
}
