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
#include <gtk/gtk.h>
#include "rtbuf_gtk3_rtbuf_info.h"

s_data_type  g_rtbuf_gtk_rtbuf_info_type = {
  sizeof(s_rtbuf_gtk_rtbuf_info),
  DATA_TYPE_BYTES
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
