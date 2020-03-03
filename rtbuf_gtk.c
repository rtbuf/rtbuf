#include <gtk/gtk.h>
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_widget.h"

GtkBuilder *builder = NULL;
GtkWindow *modular = NULL;
GtkLayout *modular_layout = NULL;
GtkMenu *library_menu = NULL;

void rtbuf_gtk_rtbuf_rename (RtbufWidget *widget)
{
  (void) widget;
  printf("rtbuf-gtk rtbuf rename\n");
}

void rtbuf_gtk_rtbuf_menu (RtbufWidget *widget, GdkEventButton *event)
{
  GtkWidget *menu = gtk_menu_new();
  GtkWidget *item = gtk_menu_item_new_with_label("Rename...");
  gtk_container_add(GTK_CONTAINER(menu), item);
  g_signal_connect_swapped(G_OBJECT(item), "activate",
                           G_CALLBACK(rtbuf_gtk_rtbuf_rename),
                           widget);
  (void) event;
  gtk_menu_popup(menu,
                 NULL, NULL, NULL, NULL,
                 event->button, event->time);
}

gboolean rtbuf_gtk_rtbuf_label_mouse_down (GtkWidget *widget,
                                           GdkEventButton *event,
                                           gpointer data)
{
  (void) widget;
  (void) data;
  if (event->type == GDK_BUTTON_PRESS) {
    if (event->button == 1) {
      printf("rtbuf-gtk rtbuf drag\n");
      return TRUE;
    }
    else if (event->button == 3) {
      printf("rtbuf-gtk rtbuf popup\n");
      rtbuf_gtk_rtbuf_menu(RTBUF_WIDGET(widget), event);
      return TRUE;
    }
  }
  return FALSE;
}


RtbufWidget * rtbuf_gtk_modular_layout_new (s_rtbuf *rtb,
                                            const gint x, const gint y)
{
  RtbufWidget *widget = rtbuf_widget_new(rtb, "rtbuf");
  GtkWidget *label;
  gtk_layout_put(modular_layout, GTK_WIDGET(widget), x, y);
  label = rtbuf_widget_get_label_widget(widget);
  g_signal_connect(G_OBJECT(label), "button-press-event",
                   G_CALLBACK(rtbuf_gtk_rtbuf_label_mouse_down),
                   NULL);
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
  rtbuf_gtk_new(library, proc, 100, 100);
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

gboolean rtbuf_gtk_modular_popup (GtkWidget *widget,
                                  GdkEventButton *event,
                                  gpointer data)
{
  (void) widget;
  (void) data;
  if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
    printf("rtbuf-gtk modular popup\n");
    gtk_menu_popup(library_menu, NULL, NULL, NULL, NULL, event->button, event->time);
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
                   G_CALLBACK(rtbuf_gtk_modular_popup), NULL);
  g_signal_connect(modular_layout, "draw",
                   G_CALLBACK(rtbuf_gtk_modular_draw), NULL);

  rtbuf_gtk_library_menu();
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
