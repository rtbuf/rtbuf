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

#include <assert.h>
#include <gtk/gtk.h>
#include "rtbuf_gtk_rtbuf_info.h"

s_data_type  g_rtbuf_gtk_rtbuf_info_type = {
  sizeof(s_rtbuf_gtk_rtbuf_info) * 8,
  DATA_TYPE_BITS
};

s_data_alloc g_rtbuf_gtk_rtbuf_info_alloc;

void rtbuf_gtk_rtbuf_info_init ()
{
  data_alloc_init(&g_rtbuf_gtk_rtbuf_info_alloc,
                  &g_rtbuf_gtk_rtbuf_info_type,
                  RTBUF_MAX, 0, 0);
}

s_rtbuf_gtk_rtbuf_info *rtbuf_gtk_rtbuf_info_new ()
{
  s_rtbuf_gtk_rtbuf_info *info =
    data_new(&g_rtbuf_gtk_rtbuf_info_alloc);
  return info;
}

void rtbuf_gtk_rtbuf_info_delete (s_rtbuf_gtk_rtbuf_info *info)
{
  assert(info);
  data_delete(&g_rtbuf_gtk_rtbuf_info_alloc, info);
}
