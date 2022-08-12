/*
 * Copyright 2018-2021 Thomas de Grivel <thoxdg@gmail.com>
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
#include <strings.h>
#include "data.h"

s_data_type g_data_alloc_type = {
  sizeof(s_data_alloc),
  DATA_TYPE_BYTES
};
s_data_alloc g_data_alloc_alloc;
s_data_alloc *g_data_alloc = NULL;

s_data_type g_data_type_type = {
  sizeof(u_data_type),
  DATA_TYPE_BYTES
};
s_data_alloc *g_data_type_alloc = NULL;
u_data_type *g_data_type = NULL;

int libdata_init ()
{
  data_alloc_init(&g_data_alloc_alloc, &g_data_alloc_type,
                  DATA_ALLOC_MAX, 0, (f_data_clean*) data_alloc_clean);
  g_data_alloc = g_data_alloc_alloc.mem;
  g_data_type_alloc = data_alloc_new(&g_data_type_type, DATA_TYPE_MAX,
                                     0, 0);
  assert(g_data_type_alloc);
  g_data_type = g_data_type_alloc->mem;
  return 0;
}

void libdata_shutdown ()
{
  if (g_data_alloc) {
    data_alloc_delete_all();
    g_data_alloc = 0;
    data_alloc_clean(&g_data_alloc_alloc);
  }
}

void data_alloc_init (s_data_alloc *da, s_data_type *t,
                      unsigned int max, f_data_init *init,
                      f_data_clean *clean)
{
  assert(da);
  assert(t);
  da->t = t;
  da->max = max;
  da->mem = calloc(max, t->bytes);
  da->n = 0;
  da->free = calloc(max, sizeof(unsigned int));
  da->free_n = 0;
  da->init = init;
  da->clean = clean;
}

void data_alloc_clean (s_data_alloc *da)
{
  assert(da);
  bzero(da->mem, da->max * da->t->bytes);
  bzero(da->free, da->max * sizeof(unsigned int));
  free(da->mem);
  free(da->free);
}

void * data_new_at (s_data_alloc *da, unsigned int i)
{
  unsigned int octets = da->t->bytes;
  unsigned int offset = i * octets;
  void *m = (char*) da->mem + offset;
  bzero(m, octets);
  if (da->init)
    da->init(m);
  return m;
}

void * data_new (s_data_alloc *da)
{
  assert(da);
  if (da->free_n) {
    unsigned int i = da->free[--da->free_n];
    da->free[da->free_n] = 0;
    return data_new_at(da, i);
  }
  if (da->n < da->max) {
    unsigned int i = da->n++;
    return data_new_at(da, i);
  }
  return 0;
}

int data_new_i (s_data_alloc *da)
{
  assert(da);
  if (da->free_n) {
    unsigned int i = da->free[--da->free_n];
    da->free[da->free_n] = 0;
    data_new_at(da, i);
    return i;
  }
  if (da->n < da->max) {
    unsigned int i = da->n++;
    data_new_at(da, i);
    return i;
  }
  return -1;
}

void data_delete (s_data_alloc *da, void *data)
{
  unsigned int octets;
  unsigned int i;
  assert(da);
  assert(da->t);
  octets = da->t->bytes;
  assert(da->mem <= data);
  assert((char*) data < (char*) da->mem + da->max * octets);
  if (da->clean)
    da->clean(data);
  bzero(data, octets);
  i = ((char*) data - (char*) da->mem) / octets;
  da->free[da->free_n++] = i;
}

s_data_alloc * data_alloc_new (s_data_type *t, unsigned int max,
                               f_data_init *init, f_data_clean *clean)
{
  s_data_alloc *da = data_new(&g_data_alloc_alloc);
  assert(da);
  data_alloc_init(da, t, max, init, clean);
  return da;
}

void data_alloc_delete (s_data_alloc *da)
{
  assert(da);
  data_delete(&g_data_alloc_alloc, da);
}

void data_alloc_delete_all ()
{
  unsigned i = 0;
  int n = g_data_alloc_alloc.n;
  while (i < g_data_alloc_alloc.max && n > 0) {
    if (g_data_alloc[i].t) {
      data_alloc_delete(g_data_alloc + i);
      n--;
    }
    i++;
  }
}

u_data_type * data_type_new (unsigned int bytes, unsigned int type)
{
  u_data_type *t;
  assert(g_data_type_alloc);
  t = data_new(g_data_type_alloc);
  assert(t);
  t->t.bytes = bytes;
  t->t.type = type;
  return t;
}

void data_type_delete (s_data_type *t)
{
  assert(g_data_type_alloc);
  data_delete(g_data_type_alloc, t);
}
