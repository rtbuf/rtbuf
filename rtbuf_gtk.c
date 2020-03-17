#include <assert.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_var.h"
#include "rtbuf_widget.h"

enum dnd_targets {
  TARGET_RTBUF,
  N_TARGETS
};

unsigned int g_next_id = 0;

GtkBuilder *builder = NULL;
GtkWindow *modular = NULL;
GtkLayout *modular_layout = NULL;
GtkMenu *library_menu = NULL;

GtkTargetList *rtbuf_move_target_list;
#define RTBUF_MOVE_TARGETS 1
GtkTargetEntry rtbuf_move_target_entry[RTBUF_MOVE_TARGETS] = {
  {"RtbufWidget", GTK_TARGET_SAME_APP, TARGET_RTBUF}
};
gint drag_x = 0;
gint drag_y = 0;

gint rtbuf_x = 100;
gint rtbuf_y = 100;

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
  printf("rtbuf-gtk rtbuf popup\n");
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
  gtk_drag_begin_with_coordinates(GTK_WIDGET(widget),
                                  rtbuf_move_target_list,
                                  GDK_ACTION_DEFAULT,
                                  event->button,
                                  (GdkEvent*) event,
                                  -1, -1);
}

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data)
{
  RtbufWidget *rtbuf_widget = RTBUF_WIDGET(widget);
  (void) data;
  printf("rtbuf-gtk rtbuf mouse down\n");
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


RtbufWidget * rtbuf_gtk_modular_layout_new (s_rtbuf *rtbuf,
                                            const gint x, const gint y)
{
  RtbufWidget *widget;
  GtkWidget *event_box;
  char label[1024];
  printf("rtbuf-gtk modular layout new\n");
  snprintf(label, sizeof(label), "%s%02d",
           rtbuf->proc->name,
           g_next_id++);
  rtbuf_var_rtbuf_set(label, (rtbuf - g_rtbuf) / sizeof(s_rtbuf));
  widget = rtbuf_widget_new(rtbuf, label);
  gtk_layout_put(modular_layout, GTK_WIDGET(widget), x, y);
  event_box = rtbuf_widget_get_event_box(widget);
  g_signal_connect_swapped(G_OBJECT(event_box), "button-press-event",
                           G_CALLBACK(rtbuf_gtk_rtbuf_button_press),
                           widget);
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
  }
  else
    rl = &g_rtbuf_lib[i];
  i = rtbuf_lib_find_proc(rl, proc);
  if (i < 0) {
    fprintf(stderr, "rtbuf-gtk new: not found %s %s\n", library, proc);
    return NULL;
  }
  rp = rl->proc[i];
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

gboolean rtbuf_gtk_modular_draw (GtkWidget *widget,
                                 cairo_t *cr,
                                 gpointer data)
{
  (void) data;
  if ((GtkLayout*) widget == modular_layout) {
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_paint(cr);
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
    if (eb->button == 3) {
      printf("rtbuf-gtk modular popup\n");
      gdk_window_get_device_position(eb->window, eb->device,
                                     &rtbuf_x, &rtbuf_y, NULL);
      gtk_menu_popup_at_pointer(library_menu, event);
      return TRUE;
    }
  }
  return FALSE;
}

gboolean rtbuf_gtk_modular_drag_motion (GtkWidget      *widget,
                                        GdkDragContext *context,
                                        gint            x,
                                        gint            y,
                                        guint           time,
                                        gpointer        data)
{
  (void) widget;
  (void) time;
  (void) data;
  GtkWidget *rtbuf_widget = gtk_drag_get_source_widget(context);
  printf("rtbuf-gtk modular drag motion %i %i\n", x, y);
  gtk_layout_move(modular_layout, rtbuf_widget, x - drag_x, y - drag_y);
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

  rtbuf_move_target_list = gtk_target_list_new(rtbuf_move_target_entry,
                                               RTBUF_MOVE_TARGETS);
  gtk_drag_dest_set(GTK_WIDGET(modular_layout), 0, rtbuf_move_target_entry,
                    RTBUF_MOVE_TARGETS, GDK_ACTION_DEFAULT);
  g_signal_connect(modular_layout, "drag-motion",
                   G_CALLBACK(rtbuf_gtk_modular_drag_motion), NULL);
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
  librtbuf_init();
  gtk_init(&argc, &argv);
  if (rtbuf_gtk_builder())
    return 1;
  rtbuf_gtk_modular();
  gtk_main ();
  return 0;
}
