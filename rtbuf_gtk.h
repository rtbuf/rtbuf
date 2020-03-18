#ifndef RTBUF_GTK_H
#define RTBUF_GTK_H

GtkLayout *modular_layout;

GtkTargetList *rtbuf_move_target_list;

gint drag_x;
gint drag_y;

gboolean rtbuf_gtk_rtbuf_button_press (GtkWidget *widget,
                                       GdkEvent *event,
                                       gpointer data);

#endif /* RTBUF_GTK_H */
