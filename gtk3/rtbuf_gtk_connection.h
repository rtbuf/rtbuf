#ifndef RTBUF_GTK_CONNECTION_H
#define RTBUF_GTK_CONNECTION_H

#include "rtbuf_input_widget.h"
#include "rtbuf_output_widget.h"

typedef struct rtbuf_gtk_connection s_rtbuf_gtk_connection;

struct rtbuf_gtk_connection {
  RtbufOutputWidget *output_widget;
  RtbufInputWidget *input_widget;
  s_rtbuf_gtk_connection *next;
};

void rtbuf_gtk_connection_init ();
s_rtbuf_gtk_connection * rtbuf_gtk_connection_new ();
void rtbuf_gtk_connection_delete (s_rtbuf_gtk_connection *connection);

void rtbuf_gtk_connection_push (s_rtbuf_gtk_connection **head,
                                s_rtbuf_gtk_connection *item);
int rtbuf_gtk_connection_remove_one (s_rtbuf_gtk_connection **head,
                                     s_rtbuf_gtk_connection *item);
s_rtbuf_gtk_connection * rtbuf_gtk_connection_find
(s_rtbuf_gtk_connection *c,
 RtbufOutputWidget *output_widget,
 RtbufInputWidget *input_widget);

void rtbuf_gtk_connection_remove_rtbuf (s_rtbuf_gtk_connection **head,
                                        s_rtbuf *rtbuf);

#endif
