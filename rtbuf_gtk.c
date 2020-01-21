#include <gtk/gtk.h>
#include <stdio.h>

void
rtbuf_gtk_modular_popup_rtbuf ()
{
  printf("rtbuf\n");
}

void
rtbuf_gtk_modular_bg_draw (GtkWidget *widget,
			   cairo_t *cr)
{
  (void) widget;
  (void) cr;
  printf("draw modular bg\n");
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;

  gtk_init(&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file(builder, "rtbuf.glade", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object(builder, "modular");
  g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "activate", G_CALLBACK (gtk_main_quit), NULL);

  gtk_main ();

  return 0;
}
