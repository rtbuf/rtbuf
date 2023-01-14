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
#include <stdlib.h>
#include "list.h"

s_list * list_new (void *item, s_list *next)
{
  s_list *list = malloc(sizeof(s_list));
  if (list) {
    list->item = item;
    list->next = next;
  }
  return list;
}

void list_delete (s_list *list)
{
  free(list);
}

void list_delete_all (s_list *list)
{
  while (list) {
    s_list *l = list;
    list = list->next;
    list_delete(l);
  }
}
