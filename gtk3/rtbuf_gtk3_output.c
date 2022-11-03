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

#include <assert.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "../librtbuf/rtbuf.h"
#include "rtbuf_gtk3.h"
#include "rtbuf_gtk3_output_widget.h"

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
                                       (void*) G_CALLBACK(rtbuf_gtk_output_disconnect),
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
