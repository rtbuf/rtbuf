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

#include <assert.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "../librtbuf/rtbuf.h"
#include "rtbuf_gtk3.h"
#include "rtbuf_gtk3_input_widget.h"

void rtbuf_gtk_input_disconnect (RtbufInputWidget *widget)
{
  s_rtbuf *rtbuf;
  unsigned int in;
  printf("rtbuf-gtk input disconnect\n");
    rtbuf = rtbuf_input_widget_get_rtbuf(widget);
  in = rtbuf_input_widget_get_in(widget);
  rtbuf_in_unbind(rtbuf, in);
}

void rtbuf_gtk_input_menu (RtbufInputWidget *widget, GdkEvent *event)
{
  static GtkMenu *menu = 0;
  static GtkWidget *disconnect;
  static guint signal;
  printf("rtbuf-gtk input menu\n");
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
                                       (void*) G_CALLBACK(rtbuf_gtk_input_disconnect),
                                       NULL);
  g_signal_connect_swapped(G_OBJECT(disconnect), "activate",
                           G_CALLBACK(rtbuf_gtk_input_disconnect),
                           widget);
  gtk_menu_popup_at_pointer(menu, event);
}

gboolean rtbuf_gtk_input_button_press (RtbufInputWidget *widget,
                                       GdkEvent *event)
{
  printf("rtbuf-gtk input check button press\n");
  if (event->type == GDK_BUTTON_PRESS) {
    GdkEventButton *eb = (GdkEventButton*) event;
    if (eb->button == 1) {
      if (drag_connection)
        rtbuf_gtk_drag_connection_end(widget);
      return TRUE;
    }
    else if (eb->button == 3) {
      rtbuf_gtk_input_menu(widget, event);
      return TRUE;
    }
  }
  return FALSE;
}
