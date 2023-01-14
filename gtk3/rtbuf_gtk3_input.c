/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
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
