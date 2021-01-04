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
