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

#include "data.h"
#include "rtbuf_gtk.h"

s_data_type g_rtbuf_gtk_connection_type = {
  sizeof(s_rtbuf_gtk_connection) * 8,
  DATA_TYPE_BITS
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
