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
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include "../librtbuf/rtbuf.h"
#include "../librtbuf/lib.h"
#include "../librtbuf/var.h"
#include "../cli/rtbuf_cli.h"
#include "rtbuf_gtk3.h"
#include "rtbuf_gtk3_library.h"
#include "rtbuf_gtk3_input_widget.h"
#include "rtbuf_gtk3_widget.h"
#include "stack.h"

unsigned int g_next_id = 0;

GtkBuilder *builder = NULL;

GMutex g_mutex;

GtkWindow              *modular = NULL;
GtkToolbar             *modular_toolbar = NULL;
s_rtbuf_gtk_connection *modular_connections = NULL;
GtkScrolledWindow      *modular_scrolled_window = NULL;
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
        dc->input_widget = input_widget;
        rtbuf_bind(src_i, out, dest, in);
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

RtbufWidget * rtbuf_gtk_modular_layout_new (s_rtbuf *rtbuf)
{
  RtbufWidget *widget;
  GtkWidget *event_box;
  char label[1024];
  s_rtbuf_gtk_rtbuf_info *info;
  assert(rtbuf);
  assert(rtbuf >= g_rtbuf && (rtbuf - g_rtbuf) < RTBUF_MAX);
  assert(rtbuf->proc);
  assert(rtbuf->proc->name);
  snprintf(label, sizeof(label), "%s%02d",
           rtbuf->proc->name,
           g_next_id++);
  rtbuf_var_rtbuf_set(label, rtbuf - g_rtbuf);
  widget = rtbuf_widget_new(rtbuf, label);
  info = (s_rtbuf_gtk_rtbuf_info*) rtbuf->user_ptr;
  info->widget = widget;
  gtk_layout_put(modular_layout, GTK_WIDGET(widget), info->x, info->y);
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

void rtbuf_gtk_new (gchar *library, const gint x, const gint y)
{
  int i;
  s_rtbuf_lib *rl = 0;
  s_rtbuf_gtk_rtbuf_info *info;
  printf("rtbuf-gtk new %s\n", library);
  i = rtbuf_lib_find(library);
  if (i < 0) {
    printf("load %s\n", library);
    if (!(rl = rtbuf_lib_load(library))) {
      fprintf(stderr, "rtbuf-gtk: load failed: '%s'\n", library);
      return;
    }
    rtbuf_lib_print(rl);
  }
  else {
    assert(i < RTBUF_LIB_MAX);
    rl = &g_rtbuf_lib[i];
  }
  assert(g_rtbuf);
  info = rtbuf_gtk_rtbuf_info_new();
  if (!info) {
    fprintf(stderr, "rtbuf-gtk: rtbuf_gtk_rtbuf_info_new failed.\n"
            "Please increase RTBUF_MAX.\n");
    return;
  }
  info->x = x;
  info->y = y;
  i = rtbuf_new_ptr(rl->proc, info);
  if (i < 0) {
    fprintf(stderr, "rtbuf-gtk new rtbuf_new failed: %s\n", library);
    return;
  }
}

void rtbuf_gtk_library_menu_activate (GtkMenuItem *menuitem,
                                      gpointer data)
{
  gchar *proc;
  gchar *library = (gchar*) data;
  g_object_get(menuitem, "label", &proc, NULL);
  rtbuf_gtk_new(library, rtbuf_x, rtbuf_y);
  rtbuf_x = 100;
  rtbuf_y = 100;
  g_free(proc);
}

void rtbuf_gtk_library_menu_library_item (GtkWidget *menuitem,
                                          gpointer data)
{
  gchar *library = (gchar*) data;
  g_signal_connect(menuitem, "activate",
                   G_CALLBACK(rtbuf_gtk_library_menu_activate),
                   library);
}

void rtbuf_gtk_destroy (GtkWidget *widget, gpointer data)
{
  (void) data;
  gtk_widget_destroy(widget);
}

void rtbuf_gtk_library_menu ()
{
  if (!library_menu)
    library_menu = GTK_MENU(gtk_builder_get_object(builder,
                                                   "library_menu"));
  gtk_container_foreach(GTK_CONTAINER(library_menu), rtbuf_gtk_destroy,
                        NULL);
  if (g_rtbuf_gtk_library_tree) {
    s_stack s;
    stack_init(&s);
    g_rtbuf_gtk_library_tree->menu = library_menu;
    stack_push(&s, g_rtbuf_gtk_library_tree);
    while (s.length) {
      s_rtbuf_gtk_library_tree *tree = stack_pop(&s);
      GtkMenu *menu = tree->menu;
      while (tree) {
        GtkWidget *item = gtk_menu_item_new_with_label(tree->dir_name);
        gtk_container_add(GTK_CONTAINER(menu), item);
        gtk_widget_show(item);
        if (tree->children) {
          GtkWidget *m = gtk_menu_new();
          tree->children->menu = GTK_MENU(m);
          gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), m);
          stack_push(&s, tree->children);
        }
        else {
          g_signal_connect(item, "activate",
                           G_CALLBACK(rtbuf_gtk_library_menu_activate),
                           (gpointer) tree->leaf_name);
        }
        tree = tree->next;
      }
    }
    stack_destroy(&s);
  }
}

void rtbuf_gtk_modular_close (GtkWidget *widget,
                              gpointer data)
{
  (void) widget;
  (void) data;
  printf("rtbuf-gtk modular close\n");
  exit(0);
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
      GtkAdjustment *adjustment;
      GdkWindow *window =
        gtk_widget_get_window(GTK_WIDGET(modular_layout));
      printf("rtbuf-gtk modular popup\n");
      gdk_window_get_device_position(window, eb->device,
                                     &rtbuf_x, &rtbuf_y, NULL);
      adjustment = gtk_scrolled_window_get_hadjustment(modular_scrolled_window);
      rtbuf_x += gtk_adjustment_get_value(adjustment);
      adjustment = gtk_scrolled_window_get_vadjustment(modular_scrolled_window);
      rtbuf_y += gtk_adjustment_get_value(adjustment);
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
      GtkAdjustment *adjustment;
      GdkWindow *window =
        gtk_widget_get_window(GTK_WIDGET(modular_layout));
      gdk_window_get_device_position(window, event->device,
                                     &x, &y, NULL);
      adjustment = gtk_scrolled_window_get_hadjustment(modular_scrolled_window);
      x += gtk_adjustment_get_value(adjustment);
      adjustment = gtk_scrolled_window_get_vadjustment(modular_scrolled_window);
      y += gtk_adjustment_get_value(adjustment);
      x -= drag_x;
      y -= drag_y;
      if (x < 1)
        x = 1;
      if (y < 1)
        y = 1;
      if (x > 8100)
        x = 8100;
      if (y > 8100)
        y = 8100;
      gtk_layout_move(modular_layout, drag_widget, x, y);
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

void rtbuf_gtk_modular_start (GtkWidget *widget, gpointer data)
{
  (void) widget;
  (void) data;
  printf("rtbuf> start\n");
  rtbuf_cli_start();
}

void rtbuf_gtk_modular_stop (GtkWidget *widget, gpointer data)
{
  (void) widget;
  (void) data;
  printf("rtbuf> stop\n");
  rtbuf_cli_stop();
}

void rtbuf_gtk_modular_toolbar ()
{
  GtkWidget *image;
  GtkToolItem *item;
  modular_toolbar = GTK_TOOLBAR(gtk_builder_get_object(builder,
                                                       "toolbar"));
  image = gtk_image_new_from_resource("/rtbuf/player_play.png");
  gtk_widget_show(image);
  item = gtk_tool_button_new(image, "Start");
  gtk_widget_show(GTK_WIDGET(item));
  g_signal_connect(item, "clicked",
                   G_CALLBACK(rtbuf_gtk_modular_start),
                   NULL);
  gtk_toolbar_insert(modular_toolbar, item, -1);
  image = gtk_image_new_from_resource("/rtbuf/player_stop.png");
  gtk_widget_show(image);
  item = gtk_tool_button_new(image, "Stop");
  gtk_widget_show(GTK_WIDGET(item));
  g_signal_connect(item, "clicked",
                   G_CALLBACK(rtbuf_gtk_modular_stop),
                   NULL);
  gtk_toolbar_insert(modular_toolbar, item, -1);
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

  modular_scrolled_window =
    GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolled_window"));

  modular_layout = GTK_LAYOUT(gtk_builder_get_object(builder, "modular_layout"));
  gtk_widget_add_events(GTK_WIDGET(modular_layout), GDK_BUTTON_PRESS_MASK);
  g_signal_connect(modular_layout, "button-press-event",
                   G_CALLBACK(rtbuf_gtk_modular_button_press), NULL);
  g_signal_connect(modular_layout, "draw",
                   G_CALLBACK(rtbuf_gtk_modular_draw), NULL);

  rtbuf_gtk_library_menu();
  rtbuf_gtk_modular_toolbar();

  g_signal_connect(modular_layout, "motion-notify-event",
                   G_CALLBACK(rtbuf_gtk_modular_motion), NULL);
}

int rtbuf_gtk_builder ()
{
  GError *error = NULL;
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_resource(builder, "/rtbuf/rtbuf_gtk3_modular.ui",
                                    &error) == 0) {
    g_printerr("Error loading resource /rtbuf/rtbuf_gtk3_modular.ui: %s\n",
               error->message);
    g_clear_error(&error);
    return 1;
  }
  return 0;
}

void rtbuf_gtk_lock ()
{
  if (pthread_equal(pthread_self(), g_rtbuf_cli_run_thread))
    g_mutex_lock(&g_mutex);
}

void rtbuf_gtk_unlock ()
{
  if (pthread_equal(pthread_self(), g_rtbuf_cli_run_thread))
    g_mutex_unlock(&g_mutex);
}

void rtbuf_gtk_position_cb (s_rtbuf *rtbuf, short x, short y)
{
  s_rtbuf_gtk_rtbuf_info *info =
    (s_rtbuf_gtk_rtbuf_info*) rtbuf->user_ptr;
  if (!info) {
    info = rtbuf_gtk_rtbuf_info_new();
    rtbuf->user_ptr = info;
    assert(info);
    info->x = x;
    info->y = y;
    rtbuf_gtk_lock();
    rtbuf_gtk_modular_layout_new(rtbuf);
    rtbuf_gtk_unlock();
  }
  else {
    info->x = x;
    info->y = y;
    rtbuf_gtk_lock();
    gtk_layout_move(modular_layout,
                    GTK_WIDGET(info->widget),
                    x, y);
    rtbuf_gtk_unlock();
  }
}

void rtbuf_gtk_delete_cb (s_rtbuf *rtbuf)
{
  if (rtbuf->user_ptr) {
    s_rtbuf_gtk_rtbuf_info *info;
    info = (s_rtbuf_gtk_rtbuf_info*) rtbuf->user_ptr;
    rtbuf_gtk_lock();
    gtk_widget_destroy(GTK_WIDGET(info->widget));
    rtbuf_gtk_unlock();
  }
}

void rtbuf_gtk_bind_cb (s_rtbuf *src, unsigned int out,
                        s_rtbuf *dest, unsigned int in)
{
  s_rtbuf_gtk_rtbuf_info *src_info;
  s_rtbuf_gtk_rtbuf_info *dest_info;
  RtbufOutputWidget *output_widget;
  RtbufInputWidget *input_widget;
  s_rtbuf_gtk_connection *connection;
  assert(src);
  assert(src->proc);
  assert(out < src->proc->out_n);
  assert(dest);
  assert(dest->proc);
  assert(in < dest->proc->in_n);
  src_info = (s_rtbuf_gtk_rtbuf_info*) src->user_ptr;
  if (!src_info)
    return;
  output_widget = src_info->out[out];
  assert(output_widget);
  dest_info = (s_rtbuf_gtk_rtbuf_info*) dest->user_ptr;
  if (!dest_info)
    return;
  input_widget = dest_info->in[in];
  assert(input_widget);
  rtbuf_gtk_lock();
  if (rtbuf_gtk_connection_find(modular_connections,
                                output_widget, input_widget))
    return;
  connection = rtbuf_gtk_connection_new();
  if (!connection) {
    rtbuf_err("failed to allocate rtbuf_gtk_connection");
    return;
  }
  connection->output_widget = output_widget;
  connection->input_widget = input_widget;
  rtbuf_gtk_connection_push(&modular_connections, connection);
  gtk_widget_queue_draw(GTK_WIDGET(modular_layout));
  rtbuf_gtk_unlock();
}

void rtbuf_gtk_unbind_cb (s_rtbuf *src, unsigned int out,
                          s_rtbuf *dest, unsigned int in)
{
  s_rtbuf_gtk_rtbuf_info *src_info;
  s_rtbuf_gtk_rtbuf_info *dest_info;
  RtbufOutputWidget *output_widget;
  RtbufInputWidget *input_widget;
  s_rtbuf_gtk_connection *connection;
  assert(src);
  assert(src->proc);
  assert(out < src->proc->out_n);
  assert(dest);
  assert(dest->proc);
  assert(in < dest->proc->in_n);
  src_info = (s_rtbuf_gtk_rtbuf_info*) src->user_ptr;
  if (!src_info)
    return;
  output_widget = src_info->out[out];
  assert(output_widget);
  dest_info = (s_rtbuf_gtk_rtbuf_info*) dest->user_ptr;
  if (!dest_info)
    return;
  input_widget = dest_info->in[in];
  assert(input_widget);
  rtbuf_gtk_lock();
  connection = rtbuf_gtk_connection_find(modular_connections,
                                         output_widget, input_widget);
  if (connection)
    rtbuf_gtk_connection_remove_one(&modular_connections,
                                    connection);
  gtk_widget_queue_draw(GTK_WIDGET(modular_layout));
  rtbuf_gtk_unlock();
}

void main_loop (void)
{
  struct timespec timeout;
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1000;
  while (1) {
    int sleep = 1;
    if (rtbuf_cli_do_event())
      sleep = 0;
    if (gtk_events_pending()) {
      int i = 100;
      g_mutex_lock(&g_mutex);
      while (gtk_events_pending() && i--)
        gtk_main_iteration();
      g_mutex_unlock(&g_mutex);
      sleep = 0;
    }
    if (sleep)
      nanosleep(&timeout, NULL);
  }
}

int main (int argc, char *argv[])
{
  g_mutex_init(&g_mutex);
  symbols_init();
  g_rtbuf_position_cb = rtbuf_gtk_position_cb;
  g_rtbuf_delete_cb = rtbuf_gtk_delete_cb;
  g_rtbuf_bind_cb = rtbuf_gtk_bind_cb;
  g_rtbuf_unbind_cb = rtbuf_gtk_unbind_cb;
  librtbuf_init();
  assert(g_rtbuf);
  gtk_init(&argc, &argv);
  rtbuf_gtk_connection_init();
  rtbuf_gtk_rtbuf_info_init();
  rtbuf_gtk_library_load();
  if (rtbuf_gtk_builder())
    return 1;
  rtbuf_gtk_modular();
  repl_init();
  rtbuf_cli_args(argc, argv);
  main_loop();
  return 0;
}

extern int rtbuf_err (const char *msg)
{
  fprintf(stderr, "rtbuf-gtk: %s\n", msg);
  return -1;
}
