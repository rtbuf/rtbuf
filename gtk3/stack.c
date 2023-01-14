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
#include <assert.h>
#include "stack.h"

void stack_init (s_stack *s)
{
  assert(s);
  s->head = 0;
  s->length = 0;
}

void stack_destroy (s_stack *s)
{
  assert(s);
  list_delete_all(s->head);
  s->head = 0;
  s->length = 0;
}

void stack_push (s_stack *s, void *item)
{
  assert(s);
  s->head = list_new(item, s->head);
  assert(s->head);
  s->length++;
}

void * stack_pop (s_stack *s)
{
  void *item;
  assert(s);
  assert(s->length > 0);
  assert(s->head);
  item = s->head->item;
  s->head = s->head->next;
  s->length--;
  return item;
}
