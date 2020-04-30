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
#include "rtbuf_input_widget.h"
#include <rtbuf/lib.h>
#include <rtbuf/var.h>
#include "rtbuf_widget.h"

unsigned int g_next_id = 0;

GtkBuilder *builder = NULL;

GtkWindow              *modular = NULL;
s_rtbuf_gtk_connection *modular_connections = NULL;
GtkLayout              *modular_layout = NULL;

GtkMenu *library_menu = NULL;

gint rtbuf_x = 100;
gint rtbuf_y = 100;

s_rtbuf_gtk_connection *drag_connection = NULL;
GtkWidget              *drag_widget = NULL;
gint                    drag_x = 0;
gint                    drag_y = 0;

void rtbuf_gtk_drag_connection_end (RtbufInputWidget *input_widget)
{
  if (drag_connection) {
    if (input_widget) {
      s_rtbuf_gtk_connection *dc = drag_connection;
      printf("rtbuf-gtk drag connection connected to input\n");
      drag_connection = NULL;
      if (!rtbuf_gtk_connection_find(modular_connections,
                                     dc->output_widget, input_widget)) {
        s_rtbuf *src = rtbuf_output_widget_get_rtbuf(dc->output_widget);
        unsigned int out = rtbuf_output_widget_get_out(dc->output_widget);
        s_rtbuf *dest = rtbuf_input_widget_get_rtbuf(input_widget);
        unsigned int in = rtbuf_input_widget_get_in(input_widget);
        unsigned int src_i = src - g_rtbuf;
        rtbuf_bind(src_i, out, dest, in);
        dc->input_widget = input_widget;
      }
      else
        rtbuf_gtk_connection_remove_one(&modular_connections, dc);
      gtk_widget_queue_draw(GTK_WIDGET(modular_layout));
    }
    else {
      printf("rtbuf-gtk drag connection abort\n");
      rtbuf_gtk_connection_remove_one(&modular_connections,
                                      drag_connection);
      drag_connection = NULL;
      gtk_widget_queue_draw(GTK_WIDGET(modular_layout));
    }
  }
}

gboolean rtbuf_gtk_input_button_press (RtbufInputWidget *widget,
                                       GdkEvent *event)
{
  printf("rtbuf-gtk input button press\n");
  (void) widget;
  (void) event;
  if (drag_connection) {
    rtbuf_gtk_drag_connection_end(widget);
    return TRUE;
  }
  return TRUE;
}

RtbufWidget * rtbuf_gtk_modular_layout_new (s_rtbuf *rtbuf,
                                            const gint x, const gint y)
{
  RtbufWidget *widget;
  GtkWidget *event_box;
  char label[1024];
  assert(rtbuf);
  assert(rtbuf >= g_rtbuf && (rtbuf - g_rtbuf) < RTBUF_MAX);
  printf("rtbuf-gtk modular layout new\n");
  assert(rtbuf->proc);
  assert(rtbuf->proc->name);
  snprintf(label, sizeof(label), "%s%02d",
           rtbuf->proc->name,
           g_next_id++);
  rtbuf_var_rtbuf_set(label, rtbuf - g_rtbuf);
  widget = rtbuf_widget_new(rtbuf, label);
  gtk_layout_put(modular_layout, GTK_WIDGET(widget), x, y);
  event_box = rtbuf_widget_get_event_box(widget);
  g_signal_connect_swapped(G_OBJECT(event_box), "button-press-event",
                           G_CALLBACK(rtbuf_gtk_rtbuf_button_press),
                           widget);
  rtbuf_widget_connect_inputs
    (widget, "button-press-event",
     G_CALLBACK(rtbuf_gtk_input_button_press));
  rtbuf_widget_connect_input_checks
    (widget, "button-press-event",
     G_CALLBACK(rtbuf_gtk_input_button_press));
  rtbuf_widget_connect_output_checks
    (widget, "button-press-event",
     G_CALLBACK(rtbuf_gtk_output_check_button_press));
  return widget;
}

RtbufWidget * rtbuf_gtk_new (gchar *library, gchar *proc,
                             const gint x, const gint y)
{
  int i;
  s_rtbuf_lib *rl = 0;
  s_rtbuf_proc *rp = 0;
  s_rtbuf *rtb = 0;
  printf("rtbuf-gtk new %s %s\n", library, proc);
  i = rtbuf_lib_find(library);
  if (i < 0) {
    printf("load %s\n", library);
    if (!(rl = rtbuf_lib_load(library))) {
      fprintf(stderr, "rtbuf-gtk: load failed: '%s'\n", library);
      return NULL;
    }
    rtbuf_lib_print(rl);
  }
  else {
    assert(i < RTBUF_LIB_MAX);
    rl = &g_rtbuf_lib[i];
  }
  i = rtbuf_lib_find_proc(rl, proc);
  if (i < 0) {
    fprintf(stderr, "rtbuf-gtk new: not found %s %s\n", library, proc);
    return NULL;
  }
  rp = rl->proc[i];
  assert(g_rtbuf);
  i = rtbuf_new(rp);
  if (i < 0) {
    fprintf(stderr, "rtbuf-gtk new rtbuf_new failed: %s %s\n", library, proc);
    return NULL;
  }
  rtb = &g_rtbuf[i];
  return rtbuf_gtk_modular_layout_new(rtb, x, y);
}

void rtbuf_gtk_library_menu_activate (GtkMenuItem *menuitem,
                                      gpointer data)
{
  gchar *proc;
  gchar *library = (gchar*) data;
  g_object_get(menuitem, "label", &proc, NULL);
  rtbuf_gtk_new(library, proc, rtbuf_x, rtbuf_y);
  rtbuf_x = 100;
  rtbuf_y = 100;
  g_free(proc);
}

void rtbuf_gtk_library_menu_library_item (GtkWidget *menuitem, gpointer data)
{
  gchar *library = (gchar*) data;
  g_signal_connect(menuitem, "activate", G_CALLBACK(rtbuf_gtk_library_menu_activate), library);
}

void rtbuf_gtk_library_menu_library (GtkWidget *menuitem, gpointer data)
{
  GtkContainer *submenu;
  gchar *library;
  (void) data;
  g_object_get(menuitem,
               "label", &library,
               "submenu", &submenu,
               NULL);
  gtk_container_foreach(submenu,
                        rtbuf_gtk_library_menu_library_item,
                        library);
  g_object_unref(G_OBJECT(submenu));
}

void rtbuf_gtk_library_menu ()
{
  library_menu = GTK_MENU(gtk_builder_get_object(builder, "library_menu"));
  gtk_container_foreach(GTK_CONTAINER(library_menu),
                        rtbuf_gtk_library_menu_library,
                        NULL);
}

void rtbuf_gtk_modular_close (GtkWidget *widget,
                              gpointer data)
{
  (void) widget;
  (void) data;
  printf("rtbuf-gtk modular close\n");
  gtk_main_quit();
}

void rtbuf_gtk_modular_draw_arrow (cairo_t *cr, int x1, int y1,
                                   int x2, int y2)
{
  const int arrow_size = 4;
  int mx = (x2 - x1) / 3;
  if (mx < 0) {
    mx = -mx;
    mx += mx / 2;
  }

  cairo_move_to(cr, x1, y1);
  cairo_curve_to(cr,
                 x1 + mx, y1,
                 x2 - mx - arrow_size, y2,
                 x2 - arrow_size, y2);
  cairo_move_to (cr, x2, y2);
  cairo_line_to (cr, x2 - arrow_size, y2);
  cairo_move_to (cr, x2 - arrow_size, y2 - arrow_size);
  cairo_line_to (cr, x2, y2);
  cairo_line_to (cr, x2 - arrow_size, y2 + arrow_size);
}

void rtbuf_gtk_modular_draw_connection (s_rtbuf_gtk_connection *c,
                                        cairo_t *cr)
{
  int x1, y1, x2, y2;
  GtkWidget *out = GTK_WIDGET(c->output_widget);
  GtkAllocation allocation;
  gtk_widget_get_allocation(out, &allocation);
  gtk_widget_translate_coordinates(out, GTK_WIDGET(modular_layout),
                                   allocation.width,
                                   allocation.height / 2,
                                   &x1, &y1);
  if (c->input_widget) {
    GtkWidget *in = GTK_WIDGET(c->input_widget);
    gtk_widget_get_allocation(in, &allocation);
    gtk_widget_translate_coordinates(in, GTK_WIDGET(modular_layout),
                                     0,
                                     allocation.height / 2,
                                     &x2, &y2);
    x2 -= 2;
  }
  else {
    x2 = drag_x;
    y2 = drag_y;
  };
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
  rtbuf_gtk_modular_draw_arrow(cr, x1, y1, x2, y2);
  cairo_stroke(cr);
}

gboolean rtbuf_gtk_modular_draw (GtkWidget *widget,
                                 cairo_t *cr,
                                 gpointer data)
{
  (void) data;
  if ((GtkLayout*) widget == modular_layout) {
    s_rtbuf_gtk_connection *c = modular_connections;
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_paint(cr);
    while (c) {
      rtbuf_gtk_modular_draw_connection(c, cr);
      c = c->next;
    }
    GTK_WIDGET_GET_CLASS(widget)->draw(widget, cr);
    return TRUE;
  }
  return FALSE;
}

gboolean rtbuf_gtk_modular_button_press (GtkWidget *widget,
                                         GdkEvent *event,
                                         gpointer data)
{
  (void) widget;
  (void) data;
  if (widget == GTK_WIDGET(modular_layout) &&
      event->type == GDK_BUTTON_PRESS) {
    GdkEventButton *eb = (GdkEventButton*) event;
    if (eb->button == 1) {
      if (drag_connection) {
        rtbuf_gtk_drag_connection_end(NULL);
        return TRUE;
      }
    }
    else if (eb->button == 3) {
      GdkWindow *window =
        gtk_widget_get_window(GTK_WIDGET(modular_layout));
      printf("rtbuf-gtk modular popup\n");
      gdk_window_get_device_position(window, eb->device,
                                     &rtbuf_x, &rtbuf_y, NULL);
      gtk_menu_popup_at_pointer(library_menu, event);
      return TRUE;
    }
  }
  return FALSE;
}

gboolean rtbuf_gtk_modular_motion (GtkWidget       *widget,
                                   GdkEventMotion  *event,
                                   gpointer         data)
{
  (void) widget;
  (void) data;
  if (drag_widget) {
    if (!(event->state & GDK_BUTTON1_MASK)) {
      printf("rtbuf-gtk modular drop widget\n");
      drag_widget = NULL;
      return TRUE;
    }
    else {
      gint x = 0;
      gint y = 0;
      GdkWindow *window =
        gtk_widget_get_window(GTK_WIDGET(modular_layout));
      gdk_window_get_device_position(window, event->device,
                                     &x, &y, NULL);
      printf("rtbuf-gtk modular drag motion %i %i\n", x, y);
      gtk_layout_move(modular_layout, drag_widget, x - drag_x,
                      y - drag_y);
      return TRUE;
    }
  }
  else if (drag_connection) {
    GdkWindow *window;
    window = gtk_widget_get_window(GTK_WIDGET(modular_layout));
    gdk_window_get_device_position(window, event->device,
                                   &drag_x, &drag_y, NULL);
    drag_connection->input_widget = NULL;
    gtk_widget_queue_draw(GTK_WIDGET(modular_layout));
    return TRUE;
  }
  return FALSE;
}

void rtbuf_gtk_modular ()
{
  GObject *button;

  modular = GTK_WINDOW(gtk_builder_get_object(builder, "modular"));
  g_signal_connect(modular, "destroy",
                   G_CALLBACK(rtbuf_gtk_modular_close), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "activate",
                   G_CALLBACK(rtbuf_gtk_modular_close), NULL);

  modular_layout = GTK_LAYOUT(gtk_builder_get_object(builder, "modular_layout"));
  gtk_widget_add_events(GTK_WIDGET(modular_layout), GDK_BUTTON_PRESS_MASK);
  g_signal_connect(modular_layout, "button-press-event",
                   G_CALLBACK(rtbuf_gtk_modular_button_press), NULL);
  g_signal_connect(modular_layout, "draw",
                   G_CALLBACK(rtbuf_gtk_modular_draw), NULL);

  rtbuf_gtk_library_menu();

  g_signal_connect(modular_layout, "motion-notify-event",
                   G_CALLBACK(rtbuf_gtk_modular_motion), NULL);
}

int rtbuf_gtk_builder ()
{
  GError *error = NULL;
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_resource(builder, "/rtbuf/rtbuf_modular.ui", &error) == 0) {
    g_printerr("Error loading resource /rtbuf/rtbuf_modular.ui: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }
  return 0;
}

int main (int argc, char *argv[])
{
  symbols_init();
  librtbuf_init();
  assert(g_rtbuf);
  gtk_init(&argc, &argv);
  rtbuf_gtk_connection_init();
  rtbuf_gtk_output_init();
  if (rtbuf_gtk_builder())
    return 1;
  rtbuf_gtk_modular();
  gtk_main ();
  return 0;
}

extern int rtbuf_err (const char *msg)
{
  fprintf(stderr, "rtbuf-gtk: %s\n", msg);
  return -1;
}
