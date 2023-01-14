/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#ifndef RTBUF_GTK_LIBRARY_H
#define RTBUF_GTK_LIBRARY_H

#include <gtk/gtk.h>

typedef struct rtbuf_gtk_library_tree s_rtbuf_gtk_library_tree;

struct rtbuf_gtk_library_tree {
  const char *dir_name;
  const char *leaf_name;
  GtkMenu *menu;
  s_rtbuf_gtk_library_tree *children;
  s_rtbuf_gtk_library_tree *next;
};

extern s_rtbuf_gtk_library_tree *g_rtbuf_gtk_library_tree;

void rtbuf_gtk_library_load ();

#endif /* RTBUF_GTK_LIBRARY_H */
