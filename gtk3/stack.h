/* rtbuf
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
#ifndef STACK_H
#define STACK_H

#include "list.h"

typedef struct stack s_stack;

struct stack {
  s_list *head;
  unsigned long length;
};

void   stack_init (s_stack *s);
void   stack_destroy (s_stack *s);
void   stack_push (s_stack *s, void *item);
void * stack_pop (s_stack *s);

#endif /* STACK_H */
