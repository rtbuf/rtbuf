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
#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef struct queue s_queue;

struct queue {
  s_list *head;
  s_list *tail;
  unsigned long length;
};

void      queue_init (s_queue *q);
void      queue_destroy (s_queue *q);
s_queue * queue_new (void);
void      queue_delete (s_queue *q);
void      queue_enqueue (s_queue *q, void *item);
void *    queue_dequeue (s_queue *q);

#endif /* LIST_H */
