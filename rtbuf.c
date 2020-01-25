/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "data.h"
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_var.h"
#include "symbol.h"

s_data_type   g_rtbuf_type = {
  sizeof(s_rtbuf) * 8,
  DATA_TYPE_BITS
};
s_data_alloc  g_rtbuf_alloc;
s_rtbuf      *g_rtbuf;
int           g_rtbuf_run = 0;
unsigned int  g_rtbuf_sort = 0;
unsigned int  g_rtbuf_sorted[RTBUF_MAX];
unsigned int  g_rtbuf_sorted_n = 0;

int librtbuf_init ()
{
  libdata_init();
  bzero(g_rtbuf_sorted, sizeof(g_rtbuf_sorted));
  data_alloc_init(&g_rtbuf_alloc, &g_rtbuf_type, RTBUF_MAX, NULL, NULL);
  g_rtbuf = g_rtbuf_alloc.mem;
  rtbuf_type_init();
  rtbuf_proc_init();
  rtbuf_lib_init_();
  rtbuf_var_init();
  return 0;
}

int rtbuf_new (s_rtbuf_proc *rp)
{
  int i;
  s_rtbuf *rtb;
  void *data;
  unsigned int j = 0;
  assert(rp);
  data = malloc(rp->out_bytes);
  if (!data)
    return rtbuf_err("malloc failed");
  bzero(data, rp->out_bytes);
  if ((i = data_new_i(&g_rtbuf_alloc)) < 0)
    return -1;
  rtb = &g_rtbuf[i];
  rtb->data = data;
  rtb->flags = 0;
  rtb->proc = rp;
  while (j < RTBUF_PROC_IN_MAX) {
    rtb->in[j].rtb = -1;
    j++;
  }
  g_rtbuf_sort = 1;
  return i;
}

void rtbuf_in_unbind (s_rtbuf *rtb, unsigned int in)
{
  s_rtbuf_binding *v = &rtb->in[in];
  if (v->rtb >= 0) {
    s_rtbuf *src = &g_rtbuf[v->rtb];
    src->refc--;
    v->rtb = -1;
    v->out = 0;
    rtb->in_n--;
  }
}

static
void rtbuf_unbind_all_out_rtbuf (s_rtbuf *rtb, unsigned int rtb_i,
                                 s_rtbuf *dest)
{
  unsigned int i = 0;
  unsigned int n;
  assert(dest);
  n = dest->in_n;
  while (i < dest->proc->in_n && n > 0 && rtb->refc) {
    s_rtbuf_binding *v = &dest->in[i];
    if (v->rtb >= 0) {
      if ((unsigned int) v->rtb == rtb_i)
        rtbuf_in_unbind(dest, i);
      n--;
    }
    i++;
  }
}

void rtbuf_unbind_all_out (s_rtbuf *rtb)
{
  unsigned int rtb_i;
  unsigned int i = 0;
  unsigned int n = g_rtbuf_alloc.n - g_rtbuf_alloc.free_n;
  assert(rtb);
  assert(g_rtbuf <= rtb);
  assert(rtb < g_rtbuf + RTBUF_MAX);
  rtb_i = rtb - g_rtbuf;
  while (i < g_rtbuf_alloc.n && rtb->refc) {
    if (g_rtbuf[i].data) {
      rtbuf_unbind_all_out_rtbuf(rtb, rtb_i, &g_rtbuf[i]);
      n--;
    }
    i++;
  }
}

void rtbuf_unbind_all (s_rtbuf *rtb)
{
  unsigned int i = 0;
  assert(rtb);
  assert(rtb->proc);
  while (i < rtb->proc->in_n && rtb->in_n > 0) {
    rtbuf_in_unbind(rtb, i);
    i++;
  }
  rtbuf_unbind_all_out(rtb);
}

void rtbuf_delete_ (s_rtbuf *rtb)
{
  assert(rtb);
  rtbuf_unbind_all(rtb);
  bzero(rtb->data, rtb->proc->out_bytes);
  free(rtb->data);
  bzero(rtb, sizeof(s_rtbuf));
  data_delete(&g_rtbuf_alloc, rtb);
}

void rtbuf_delete (s_rtbuf *rtb)
{
  assert(rtb);
  rtb->flags |= RTBUF_DELETE;
  g_rtbuf_sort = 1;
}

int rtbuf_clone (s_rtbuf *rtb)
{
  int new_i;
  s_rtbuf *new;
  unsigned int j = 0;
  assert(rtb);
  if ((new_i = rtbuf_new(rtb->proc)) < 0)
    return -1;
  new = &g_rtbuf[new_i];
  while (j < rtb->proc->in_n) {
    new->in[j] = rtb->in[j];
    j++;
  }
  new->in_n = rtb->in_n;
  return new_i;
}

void rtbuf_bind (unsigned int src, unsigned int out,
                 s_rtbuf *dest, unsigned int in)
{
  s_rtbuf_binding *v = &dest->in[in];
  if ((unsigned int) v->rtb == src && v->out == out)
    return;
  rtbuf_in_unbind(dest, in);
  v->rtb = src;
  v->out = out;
  dest->in_n++;
  g_rtbuf[src].refc++;
  g_rtbuf_sort = 1;
}

int rtbuf_data_set (s_rtbuf *rtb, symbol name, void *value,
                    unsigned int size)
{
  int out_i = rtbuf_proc_out_find(rtb->proc, name);
  if (out_i >= 0) {
    s_rtbuf_proc_out *out = &rtb->proc->out[out_i];
    if (out->type->t.bits == size * 8) {
      void *data = rtb->data + out->offset;
      memcpy(data, value, size);
      return size;
    }
  }
  return 0;
}

typedef struct rtbuf_in_ptr {
  unsigned int rtb;
  unsigned int in;
} s_rtbuf_in_ptr;

typedef struct rtbuf_in_stack {
  s_rtbuf_in_ptr st[RTBUF_MAX];
  unsigned int size;
} s_rtbuf_in_stack;

void rtbuf_in_stack_init (s_rtbuf_in_stack *rvs)
{
  //printf("rtbuf_in_stack_init\n");
  bzero(rvs, sizeof(s_rtbuf_in_stack));
}

s_rtbuf_in_ptr * rtbuf_in_stack_push (s_rtbuf_in_stack *rvs,
                                        unsigned int rtb,
                                        unsigned int in)
{
  //printf("rtbuf_in_stack_push %i %i\n", rtb, in);
  s_rtbuf_in_ptr *top;
  if (rvs->size >= RTBUF_MAX)
    return 0;
  top = &rvs->st[rvs->size];
  top->rtb = rtb;
  top->in = in;
  rvs->size++;
  return top;
}

s_rtbuf_in_ptr * rtbuf_in_stack_pop (s_rtbuf_in_stack *rvs)
{
  //printf("rtbuf_in_stack_pop\n");
  if (rvs->size <= 0)
    return 0;
  rvs->size--;
  return &rvs->st[rvs->size];
}

s_rtbuf_in_ptr * rtbuf_in_stack_top (s_rtbuf_in_stack *rvs)
{
  s_rtbuf_in_ptr *top;
  //printf("rtbuf_in_stack_top\n");
  if (rvs->size <= 0) {
    //printf(" rtbuf_in_stack_top => 0\n");
    return 0;
  }
  top = &rvs->st[rvs->size - 1];
  //printf(" rtbuf_in_stack_top => { %u, %u }\n", top->rtb, top->in);
  return top;
}

void rtbuf_find_roots (s_rtbuf_in_stack *rvs)
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  unsigned int n = g_rtbuf_alloc.n - g_rtbuf_alloc.free_n;
  unsigned int c = 0;
  //printf("rtbuf_find_roots\n");
  while (i < g_rtbuf_alloc.n && n > 0) {
    //printf(" rtbuf_find_roots %u %u\n", i, n);
    if (rtb->data) {
      if (rtb->flags & RTBUF_DELETE)
        rtbuf_delete_(rtb);
      else {
        if (rtb->refc == 0) {
          rtbuf_in_stack_push(rvs, i, 0);
          c++;
        }
      }
      n--;
    }
    rtb++;
    i++;
  }
  //printf(" rtbuf_find_roots => %u\n", c);
}

void rtbuf_sort_push_child (s_rtbuf_in_stack *rvs,
                            s_rtbuf_in_ptr *ptr)
{
  int rtb;
  unsigned int i = 0;
  int found = 0;
  //printf("rtbuf_sort_push_child { %u, %u }\n", ptr->rtb, ptr->in);
  rtb = g_rtbuf[ptr->rtb].in[ptr->in].rtb;
  ptr->in++;
  if (rtb >= 0) {
    while (i < g_rtbuf_sorted_n && !found) {
      if (g_rtbuf_sorted[i] == (unsigned) rtb)
        found = 1;
      i++;
    }
    i = 0;
    while (i < rvs->size && !found) {
      if (rvs->st[i].rtb == (unsigned int) rtb)
        found = 1;
      i++;
    }
    if (!found)
      rtbuf_in_stack_push(rvs, rtb, 0);
  }
}

void rtbuf_sort ()
{
  s_rtbuf_in_stack rvs;
  s_rtbuf_in_ptr *ptr;
  //printf("rtbuf_sort\n");
  if (g_rtbuf_alloc.n == 0)
    return;
  rtbuf_in_stack_init(&rvs);
  rtbuf_find_roots(&rvs);
  g_rtbuf_sorted_n = 0;
  while ((ptr = rtbuf_in_stack_top(&rvs))) {
    if (ptr->in == g_rtbuf[ptr->rtb].proc->in_n) {
      g_rtbuf_sorted[g_rtbuf_sorted_n++] = ptr->rtb;
      rtbuf_in_stack_pop(&rvs);
    } else
      rtbuf_sort_push_child(&rvs, ptr);
  }
  g_rtbuf_sort = 0;
  rtbuf_print_sorted();
}

void rtbuf_start ()
{
  unsigned int i = 0;
  //printf("rtbuf_start\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    assert(rtb->data);
    if (rtb->proc->start) {
      //printf(" start ");
      //rtbuf_print(g_rtbuf_sorted[i]);
      //printf(" ");
      //rtbuf_proc_print(rtb->proc);
      //printf("\n");
      rtb->proc->start(rtb);
    }
    i++;
  }
}

void rtbuf_run ()
{
  unsigned int i = 0;
  //printf("rtbuf_run\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    //printf(" rtbuf_run %i ", i);
    //rtbuf_print(g_rtbuf_sorted[i]);
    //printf("\n");
    assert(rtb->data);
    if (rtb->proc->f)
      rtb->proc->f(rtb);
    i++;
  }
  //printf(" rtbuf_run => %u\n", i);
}

void rtbuf_stop ()
{
  unsigned int i = 0;
  //printf("rtbuf_stop\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    assert(rtb->data);
    if (rtb->proc->stop)
      rtb->proc->stop(rtb);
    i++;
  }
}

int rtbuf_find (const char *x)
{
  int i = atoi(x);
  if (0 <= i && (unsigned int) i < g_rtbuf_alloc.n &&
      g_rtbuf[i].data && g_rtbuf[i].proc)
    return i;
  return -1;
}

int rtbuf_in_find (s_rtbuf *rtb, const char *x)
{
  s_rtbuf_proc *proc = rtb->proc;
  symbol sym;
  //printf("rtbuf_in_find %s\n", x);
  if ('0' <= x[0] && x[0] <= '9') {
    int i = atoi(x);
    if (0 <= i && (unsigned int) i < proc->in_n)
      return i;
  }
  if ((sym = symbol_find(x))) {
    unsigned int i = 0;
    //printf("rtbuf_in_find sym %s\n", sym);
    while (i < proc->in_n) {
      if (sym == proc->in[i].name)
        return i;
      i++;
    }
  }
  return -1;
}

int rtbuf_out_find (s_rtbuf *rtb, const char *x)
{
  s_rtbuf_proc *proc = rtb->proc;
  symbol sym;
  //printf("rtbuf_out_find %s\n", x);
  if ('0' <= x[0] && x[0] <= '9') {
    int i = atoi(x);
    if (0 <= i && (unsigned int) i < proc->out_n)
      return i;
  }
  if ((sym = symbol_find(x))) {
    unsigned int i = 0;
    //printf("rtbuf_out_find sym %s\n", sym);
    while (i < proc->out_n) {
      if (sym == proc->out[i].name)
        return i;
      i++;
    }
  }
  return -1;
}

int rtbuf_out_int (s_rtbuf *rtb, unsigned int out, int default_value)
{
  s_rtbuf_proc_out *o;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->proc);
  assert(out < rtb->proc->out_n);
  o = &rtb->proc->out[out];
  assert(o->type);
  if (o->type->t.bits >= sizeof(int) * 8) {
    int *i = (int*)(rtb->data + o->offset);
    return *i;
  }
  return default_value;
}

void rtbuf_print (unsigned int i)
{
  assert(i < RTBUF_MAX);
  printf("#<rtbuf %u>", i);
  fflush(stdout);
}

void rtbuf_print_long_in (s_rtbuf *rtb, unsigned int j)
{
  assert(rtb);
  assert(rtb->proc);
  assert(j < rtb->proc->in_n);
  assert(rtb->proc->in[j].name);
  assert(rtb->proc->in[j].type);
  assert(rtb->proc->in[j].type->name);
  printf("\n  in %i %s:%s", j, rtb->proc->in[j].name,
         rtb->proc->in[j].type->name);
  if (rtb->in[j].rtb >= 0) {
    s_rtbuf *target = &g_rtbuf[rtb->in[j].rtb];
    unsigned int target_out = rtb->in[j].out;
    printf (" = ");
    rtbuf_print(rtb->in[j].rtb);
    printf(" out %u %s:%s", target_out,
           target->proc->out[target_out].name,
           target->proc->out[target_out].type->name);
  }
}

void rtbuf_print_long (unsigned int i)
{
  s_rtbuf *rtb;
  s_rtbuf_proc *proc;
  unsigned int j = 0;
  assert(i < RTBUF_MAX);
  rtb = &g_rtbuf[i];
  proc = rtb->proc;
  printf("#<rtbuf %i", i);
  printf(" %s %s", proc->lib->name, proc->name);
  if (rtb->data) {
    printf(" %d", rtb->refc);
    while (j < proc->in_n)
      rtbuf_print_long_in(rtb, j++);
    j = 0;
    while (j < proc->out_n) {
      printf("\n  out %i %s:%s", j, proc->out[j].name,
             proc->out[j].type->name);
      j++;
    }
  }
  printf(">\n");
  fflush(stdout);
}

void rtbuf_print_sorted ()
{
  unsigned int i = 0;
  while (i < g_rtbuf_sorted_n) {
    if (i)
      printf(" ");
    rtbuf_print(g_rtbuf_sorted[i]);
    i++;
  }
  printf("\n");
}
