/* c3
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */

#include <assert.h>
#include "../librtbuf/data.h"
#include "rtbuf_gtk3.h"

s_data_type g_rtbuf_gtk_connection_type = {
  sizeof(s_rtbuf_gtk_connection),
  DATA_TYPE_BYTES
};
s_data_alloc g_rtbuf_gtk_connection_alloc;

void rtbuf_gtk_connection_init ()
{
  data_alloc_init(&g_rtbuf_gtk_connection_alloc,
                  &g_rtbuf_gtk_connection_type,
                  RTBUF_MAX * 10, NULL, NULL);
}

s_rtbuf_gtk_connection * rtbuf_gtk_connection_new ()
{
  s_rtbuf_gtk_connection *connection = data_new(&g_rtbuf_gtk_connection_alloc);
  return connection;
}

void rtbuf_gtk_connection_delete (s_rtbuf_gtk_connection *connection)
{
  data_delete(&g_rtbuf_gtk_connection_alloc, connection);
}

void rtbuf_gtk_connection_push (s_rtbuf_gtk_connection **head,
                                s_rtbuf_gtk_connection *item)
{
  assert(!item->next);
  item->next = *head;
  *head = item;
}

int rtbuf_gtk_connection_remove_one (s_rtbuf_gtk_connection **head,
                                     s_rtbuf_gtk_connection *item)
{
  s_rtbuf_gtk_connection **next = head;
  while (*next && *next != item)
    next = &(*next)->next;
  if (*next == item) {
    *next = item->next;
    return 1;
  }
  return 0;
}

s_rtbuf_gtk_connection * rtbuf_gtk_connection_find
(s_rtbuf_gtk_connection *c,
 RtbufOutputWidget *output_widget,
 RtbufInputWidget *input_widget)
{
  while (c && (c->output_widget != output_widget ||
               c->input_widget != input_widget))
    c = c->next;
  return c;
}

void rtbuf_gtk_connection_remove_rtbuf (s_rtbuf_gtk_connection **conn,
                                        s_rtbuf *rtbuf)
{
  while (*conn) {
    s_rtbuf *output_rtbuf;
    output_rtbuf = rtbuf_output_widget_get_rtbuf((*conn)->output_widget);
    if (output_rtbuf != rtbuf) {
      s_rtbuf *input_rtbuf;
      input_rtbuf = rtbuf_input_widget_get_rtbuf((*conn)->input_widget);
      if (input_rtbuf != rtbuf) {
        conn = &(*conn)->next;
        continue;
      }
    }
    *conn = (*conn)->next;
  }
}
