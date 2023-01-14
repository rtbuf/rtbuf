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
#include <stdlib.h>
#include "queue.h"

void queue_init (s_queue *q)
{
  assert(q);
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;
}

void queue_destroy (s_queue *q)
{
  assert(q);
  list_delete_all(q->head);
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;
}

s_queue * queue_new ()
{
  s_queue *q = malloc(sizeof(s_queue));
  if (q)
    queue_init(q);
  return q;
}

void queue_delete (s_queue *q)
{
  assert(q);
  queue_destroy(q);
  free(q);
}

void queue_enqueue (s_queue *q, void *item)
{
  s_list *l;
  assert(q);
  l = list_new(item, NULL);
  assert(l);
  if (q->length == 0) {
    q->head = q->tail = l;
  }
  else {
    assert(q->tail);
    q->tail->next = l;
    q->tail = l;
  }
  q->length++;
}

void * queue_dequeue (s_queue *q)
{
  s_list *head;
  void *item;
  assert(q);
  assert(q->length > 0);
  assert(q->head);
  head = q->head;
  item = head->item;
  q->head = head->next;
  if (q->head == NULL)
    q->tail = NULL;
  q->length--;
  list_delete(head);
  return item;
}
