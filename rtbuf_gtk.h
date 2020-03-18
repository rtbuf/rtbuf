#ifndef RTBUF_GTK_H
#define RTBUF_GTK_H

#include <gtk/gtk.h>
#include "rtbuf_input_widget.h"
#include "rtbuf_output_widget.h"
#include "rtbuf_widget.h"

GtkLayout *modular_layout;

GtkTargetList *rtbuf_move_target_list;

gint drag_x;
gint drag_y;

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data);

gboolean rtbuf_gtk_input_check_button_press (RtbufInputWidget *widget,
                                             GdkEvent *event);

gboolean rtbuf_gtk_output_check_button_press (RtbufOutputWidget *widget,
                                              GdkEvent *event);

#endif /* RTBUF_GTK_H */
