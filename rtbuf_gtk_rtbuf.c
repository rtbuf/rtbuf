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
#include "rtbuf.h"
#include "rtbuf_gtk.h"
#include "rtbuf_widget.h"

void rtbuf_gtk_rtbuf_rename (RtbufWidget *widget)
{
  (void) widget;
  printf("rtbuf-gtk rtbuf rename\n");
}

void rtbuf_gtk_rtbuf_delete (RtbufWidget *widget)
{
  s_rtbuf *rtbuf = rtbuf_widget_get_rtbuf(widget);
  printf("rtbuf-gtk rtbuf delete\n");
  gtk_container_remove(GTK_CONTAINER(modular_layout),
                       GTK_WIDGET(widget));
  rtbuf_delete(rtbuf);
}

void rtbuf_gtk_rtbuf_menu (RtbufWidget *widget, GdkEvent *event)
{
  static GtkMenu *menu = 0;
  static GtkWidget *rename;
  static GtkWidget *delete;
  static guint signal;
  printf("rtbuf-gtk rtbuf menu\n");
  if (!menu) {
    signal = g_signal_lookup("activate", GTK_TYPE_MENU_ITEM);
    assert(signal);
    menu = GTK_MENU(gtk_menu_new());
    rename = gtk_menu_item_new_with_label("Rename...");
    gtk_container_add(GTK_CONTAINER(menu), rename);
    gtk_widget_show(rename);
    delete = gtk_menu_item_new_with_label("Delete");
    gtk_container_add(GTK_CONTAINER(menu), delete);
    gtk_widget_show(delete);
  }
  g_signal_handlers_disconnect_matched(G_OBJECT(rename),
                                       G_SIGNAL_MATCH_FUNC,
                                       signal,
                                       0,
                                       NULL,
                                       G_CALLBACK(rtbuf_gtk_rtbuf_rename),
                                       NULL);
  g_signal_connect_swapped(G_OBJECT(rename), "activate",
                           G_CALLBACK(rtbuf_gtk_rtbuf_rename),
                           widget);
  g_signal_handlers_disconnect_matched(G_OBJECT(delete),
                                       G_SIGNAL_MATCH_FUNC,
                                       signal,
                                       0,
                                       NULL,
                                       G_CALLBACK(rtbuf_gtk_rtbuf_delete),
                                       NULL);
  g_signal_connect_swapped(G_OBJECT(delete), "activate",
                           G_CALLBACK(rtbuf_gtk_rtbuf_delete),
                           widget);
  gtk_menu_popup_at_pointer(menu, event);
}

void rtbuf_gtk_rtbuf_drag_begin (GtkWidget      *widget,
                                 GdkDragContext *context,
                                 gpointer        data)
{
  (void) data;
  (void) context;
  printf("rtbuf-gtk rtbuf drag begin\n");
}

void rtbuf_gtk_rtbuf_drag (RtbufWidget *widget,
                           GdkEventButton *event)
{
  (void) widget;
  (void) event;
  GtkWidget *gtk_widget = GTK_WIDGET(widget);
  printf("rtbuf-gtk rtbuf drag\n");
  g_signal_connect(G_OBJECT(widget), "drag-begin",
                   G_CALLBACK (rtbuf_gtk_rtbuf_drag_begin), NULL);
  gdk_window_get_device_position(event->window, event->device,
                                 &drag_x, &drag_y, NULL);
  drag_widget = gtk_widget;
}

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data)
{
  RtbufWidget *rtbuf_widget = RTBUF_WIDGET(widget);
  (void) data;
  printf("rtbuf-gtk rtbuf button press\n");
  if (event->type == GDK_BUTTON_PRESS) {
    GdkEventButton *eb = (GdkEventButton*) event;
    if (eb->button == 1) {
      rtbuf_gtk_rtbuf_drag(rtbuf_widget, eb);
      return TRUE;
    }
    else if (eb->button == 3) {
      rtbuf_gtk_rtbuf_menu(rtbuf_widget, event);
      return TRUE;
    }
  }
  return FALSE;
}
