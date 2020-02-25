#include <gtk/gtk.h>
#include <stdio.h>

GObject *modular;
GObject *modular_layout;

void rtbuf_gtk_modular_close (GtkWidget *widget,
                              gpointer user_data)
{
  printf("rtbuf-gtk modular close\n");
  gtk_main_quit();
}

gboolean rtbuf_gtk_modular_popup (GtkWidget *widget,
                                  GdkEventButton *event,
                                  gpointer user_data)
{
  if (event->button == 3) {
    printf("rtbuf-gtk modular popup\n");
  }
  return TRUE;
}

gboolean rtbuf_gtk_modular_draw (GtkWidget *widget,
                                 cairo_t *cr,
                                 gpointer user_data)
{
  (void) widget;
  (void) cr;
  (void) user_data;
  printf("rtbuf-gtk modular draw\n");
  return TRUE;
}

void rtbuf_gtk_modular_window (GtkBuilder *builder)
{
  GObject *button;

  modular = gtk_builder_get_object(builder, "modular");
  g_signal_connect(modular, "destroy", G_CALLBACK(rtbuf_gtk_modular_close), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "activate", G_CALLBACK(rtbuf_gtk_modular_close), NULL);

  modular_layout = gtk_builder_get_object(builder, "modular_layout");
  gtk_widget_add_events(modular_layout, GDK_BUTTON_PRESS_MASK);
  g_signal_connect(modular_layout, "button-press-event", G_CALLBACK(rtbuf_gtk_modular_popup), NULL);
  g_signal_connect(modular_layout, "draw", G_CALLBACK(rtbuf_gtk_modular_draw), NULL);
}

int main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GError *error = NULL;

  gtk_init(&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file(builder, "rtbuf.glade", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  rtbuf_gtk_modular_window(builder);

  gtk_main ();

  return 0;
}
