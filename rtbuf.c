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
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "symbol.h"

s_rtbuf      g_rtbuf[RTBUF_MAX];
unsigned int g_rtbuf_max = RTBUF_MAX;
unsigned int g_rtbuf_n = 0;
int          g_rtbuf_run = 0;
unsigned int g_rtbuf_sort = 0;
int          g_rtbuf_sorted[RTBUF_MAX];
unsigned int g_rtbuf_sorted_n = 0;
int          g_rtbuf_delete = 0;

int rtbuf_init ()
{
  bzero(g_rtbuf, sizeof(g_rtbuf));
  bzero(g_rtbuf_sorted, sizeof(g_rtbuf_sorted));
  return 0;
}

int rtbuf_new_ (void *data)
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  if (g_rtbuf_n == RTBUF_MAX)
    return rtbuf_err("RTBUF_MAX exhausted");
  while (i < RTBUF_MAX) {
    if (rtb->data == 0) {
      rtb->data = data;
      g_rtbuf_n++;
      return i;
    }
    rtb++;
    i++;
  }
  return -1;
}

int rtbuf_new (s_rtbuf_fun *rf)
{
  int i;
  s_rtbuf *rtb;
  void *data;
  unsigned int j = 0;
  assert(rf);
  data = malloc(rf->out_bytes);
  if (!data)
    return rtbuf_err("malloc failed");
  bzero(data, rf->out_bytes);
  if ((i = rtbuf_new_(data)) < 0)
    return -1;
  rtb = &g_rtbuf[i];
  rtb->flags = 0;
  rtb->fun = rf;
  while (j < RTBUF_FUN_VAR_MAX) {
    rtb->var[j].rtb = -1;
    j++;
  }
  g_rtbuf_sort = 1;
  return i;
}

void rtbuf_var_unbind (s_rtbuf *rtb, unsigned int var)
{
  s_rtbuf_binding *v = &rtb->var[var];
  if (v->rtb >= 0) {
    s_rtbuf *src = &g_rtbuf[v->rtb];
    src->refc--;
    v->rtb = -1;
    v->out = 0;
    rtb->var_n--;
  }
}

static
void rtbuf_unbind_all_out_rtbuf (s_rtbuf *rtb, unsigned int rtb_i,
                                 s_rtbuf *dest)
{
  unsigned int i = 0;
  unsigned int n;
  assert(dest);
  n = dest->var_n;
  while (i < dest->fun->var_n && n > 0 && rtb->refc) {
    s_rtbuf_binding *v = &dest->var[i];
    if (v->rtb >= 0) {
      if ((unsigned int) v->rtb == rtb_i)
        rtbuf_var_unbind(dest, i);
      n--;
    }
    i++;
  }
}

void rtbuf_unbind_all_out (s_rtbuf *rtb)
{
  unsigned int rtb_i;
  unsigned int i = 0;
  unsigned int n = g_rtbuf_n;
  assert(rtb);
  assert(g_rtbuf <= rtb);
  assert(rtb < g_rtbuf + RTBUF_MAX);
  rtb_i = rtb - g_rtbuf;
  while (i < RTBUF_MAX && n > 0 && rtb->refc) {
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
  assert(rtb->fun);
  while (i < rtb->fun->var_n && rtb->var_n > 0) {
    rtbuf_var_unbind(rtb, i);
    i++;
  }
  rtbuf_unbind_all_out(rtb);
}

void rtbuf_delete_ (s_rtbuf *rtb)
{
  assert(rtb);
  rtbuf_unbind_all(rtb);
  bzero(rtb->data, rtb->fun->out_bytes);
  free(rtb->data);
  bzero(rtb, sizeof(s_rtbuf));
  g_rtbuf_n--;
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
  if ((new_i = rtbuf_new(rtb->fun)) < 0)
    return -1;
  new = &g_rtbuf[new_i];
  while (j < rtb->fun->var_n) {
    new->var[j] = rtb->var[j];
    j++;
  }
  new->var_n = rtb->var_n;
  return new_i;
}

void rtbuf_bind (unsigned int src, unsigned int out,
                 s_rtbuf *dest, unsigned int var)
{
  s_rtbuf_binding *v = &dest->var[var];
  if ((unsigned int) v->rtb == src && v->out == out)
    return;
  rtbuf_var_unbind(dest, var);
  v->rtb = src;
  v->out = out;
  dest->var_n++;
  g_rtbuf[src].refc++;
  g_rtbuf_sort = 1;
}

int rtbuf_data_set (s_rtbuf *rtb, symbol name, void *value,
                    unsigned int size)
{
  int out_i = rtbuf_fun_out_find(rtb->fun, name);
  if (out_i >= 0) {
    s_rtbuf_fun_out *out = &rtb->fun->out[out_i];
    if (out->type->size == size) {
      void *data = rtb->data + out->offset;
      memcpy(data, value, size);
      return size;
    }
  }
  return 0;
}

typedef struct rtbuf_var_ptr {
  unsigned int rtb;
  unsigned int var;
} s_rtbuf_var_ptr;

typedef struct rtbuf_var_stack {
  s_rtbuf_var_ptr st[RTBUF_MAX];
  unsigned int size;
} s_rtbuf_var_stack;

void rtbuf_var_stack_init (s_rtbuf_var_stack *rvs)
{
  //printf("rtbuf_var_stack_init\n");
  bzero(rvs, sizeof(s_rtbuf_var_stack));
}

s_rtbuf_var_ptr * rtbuf_var_stack_push (s_rtbuf_var_stack *rvs,
                                        unsigned int rtb,
                                        unsigned int var)
{
  //printf("rtbuf_var_stack_push %i %i\n", rtb, var);
  s_rtbuf_var_ptr *top;
  if (rvs->size >= RTBUF_MAX)
    return 0;
  top = &rvs->st[rvs->size];
  top->rtb = rtb;
  top->var = var;
  rvs->size++;
  return top;
}

s_rtbuf_var_ptr * rtbuf_var_stack_pop (s_rtbuf_var_stack *rvs)
{
  //printf("rtbuf_var_stack_pop\n");
  if (rvs->size <= 0)
    return 0;
  rvs->size--;
  return &rvs->st[rvs->size];
}

s_rtbuf_var_ptr * rtbuf_var_stack_top (s_rtbuf_var_stack *rvs)
{
  s_rtbuf_var_ptr *top;
  //printf("rtbuf_var_stack_top\n");
  if (rvs->size <= 0) {
    //printf(" rtbuf_var_stack_top => 0\n");
    return 0;
  }
  top = &rvs->st[rvs->size - 1];
  //printf(" rtbuf_var_stack_top => { %u, %u }\n", top->rtb, top->var);
  return top;
}

void rtbuf_find_roots (s_rtbuf_var_stack *rvs)
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  unsigned int n = g_rtbuf_n;
  unsigned int c = 0;
  //printf("rtbuf_find_roots\n");
  while (i < RTBUF_MAX && n > 0) {
    //printf(" rtbuf_find_roots %u %u\n", i, n);
    if (rtb->data) {
      if (rtb->flags & RTBUF_DELETE)
        rtbuf_delete_(rtb);
      else {
        if (rtb->refc == 0) {
          rtbuf_var_stack_push(rvs, i, 0);
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

void rtbuf_sort_push_child (s_rtbuf_var_stack *rvs,
                            s_rtbuf_var_ptr *ptr)
{
  int rtb;
  unsigned int i = 0;
  int found = 0;
  //printf("rtbuf_sort_push_child { %u, %u }\n", ptr->rtb, ptr->var);
  rtb = g_rtbuf[ptr->rtb].var[ptr->var].rtb;
  ptr->var++;
  if (rtb >= 0) {
    while (i < g_rtbuf_sorted_n && !found) {
      if (g_rtbuf_sorted[i] == rtb)
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
      rtbuf_var_stack_push(rvs, rtb, 0);
  }
}

void rtbuf_sort ()
{
  s_rtbuf_var_stack rvs;
  s_rtbuf_var_ptr *ptr;
  printf("rtbuf_sort\n");
  if (g_rtbuf_n == 0)
    return;
  rtbuf_var_stack_init(&rvs);
  rtbuf_find_roots(&rvs);
  g_rtbuf_sorted_n = 0;
  while ((ptr = rtbuf_var_stack_top(&rvs))) {
    if (ptr->var == g_rtbuf[ptr->rtb].fun->var_n) {
      g_rtbuf_sorted[g_rtbuf_sorted_n++] = ptr->rtb;
      rtbuf_var_stack_pop(&rvs);
    } else
      rtbuf_sort_push_child(&rvs, ptr);
  }
  g_rtbuf_sort = 0;
  rtbuf_print_sorted();
}

void rtbuf_start ()
{
  unsigned int i = 0;
  printf("rtbuf_start\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    assert(rtb->data);
    if (rtb->fun->start) {
      printf(" start ");
      rtbuf_print(g_rtbuf_sorted[i]);
      printf(" ");
      rtbuf_fun_print(rtb->fun);
      printf("\n");
      rtb->fun->start(rtb);
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
    if (rtb->fun->f)
      rtb->fun->f(rtb);
    i++;
  }
  //printf(" rtbuf_run => %u\n", i);
}

void rtbuf_stop ()
{
  unsigned int i = 0;
  printf("rtbuf_stop\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    assert(rtb->data);
    if (rtb->fun->stop)
      rtb->fun->stop(rtb);
    i++;
  }
}

int rtbuf_find (const char *x)
{
  int i = atoi(x);
  if (0 <= i && i < RTBUF_MAX && g_rtbuf[i].data && g_rtbuf[i].fun)
    return i;
  return -1;
}

int rtbuf_var_find (s_rtbuf *rtb, const char *x)
{
  s_rtbuf_fun *fun = rtb->fun;
  symbol sym;
  //printf("rtbuf_var_find %s\n", x);
  if ('0' <= x[0] && x[0] <= '9') {
    int i = atoi(x);
    if (0 <= i && (unsigned int) i < fun->var_n)
      return i;
  }
  if ((sym = symbol_find(x))) {
    unsigned int i = 0;
    //printf("rtbuf_var_find sym %s\n", sym);
    while (i < fun->var_n) {
      if (sym == fun->var[i].name)
        return i;
      i++;
    }
  }
  return -1;
}

int rtbuf_out_find (s_rtbuf *rtb, const char *x)
{
  s_rtbuf_fun *fun = rtb->fun;
  symbol sym;
  printf("rtbuf_out_find %s\n", x);
  if ('0' <= x[0] && x[0] <= '9') {
    int i = atoi(x);
    if (0 <= i && (unsigned int) i < fun->out_n)
      return i;
  }
  if ((sym = symbol_find(x))) {
    unsigned int i = 0;
    printf("rtbuf_out_find sym %s\n", sym);
    while (i < fun->out_n) {
      if (sym == fun->out[i].name)
        return i;
      i++;
    }
  }
  return -1;
}

int rtbuf_out_int (s_rtbuf *rtb, unsigned int out, int default_value)
{
  s_rtbuf_fun_out *o;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->fun);
  assert(out < rtb->fun->out_n);
  o = &rtb->fun->out[out];
  assert(o->type);
  if (o->type->size >= sizeof(int)) {
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

void rtbuf_print_long_var (s_rtbuf *rtb, unsigned int j)
{
  assert(rtb);
  assert(rtb->fun);
  assert(j < rtb->fun->var_n);
  assert(rtb->fun->var[j].name);
  assert(rtb->fun->var[j].type);
  assert(rtb->fun->var[j].type->name);
  printf("\n  var %i %s:%s", j, rtb->fun->var[j].name,
         rtb->fun->var[j].type->name);
  if (rtb->var[j].rtb >= 0) {
    s_rtbuf *target = &g_rtbuf[rtb->var[j].rtb];
    unsigned int target_out = rtb->var[j].out;
    printf (" = ");
    rtbuf_print(rtb->var[j].rtb);
    printf(" out %u %s:%s", target_out,
           target->fun->out[target_out].name,
           target->fun->out[target_out].type->name);
  }
}

void rtbuf_print_long (unsigned int i)
{
  s_rtbuf *rtb;
  s_rtbuf_fun *fun;
  unsigned int j = 0;
  assert(i < RTBUF_MAX);
  rtb = &g_rtbuf[i];
  fun = rtb->fun;
  printf("#<rtbuf %i", i);
  printf(" %s %s", fun->lib->name, fun->name);
  if (rtb->data) {
    printf(" %d", rtb->refc);
    while (j < fun->var_n)
      rtbuf_print_long_var(rtb, j++);
    j = 0;
    while (j < fun->out_n) {
      printf("\n  out %i %s:%s", j, fun->out[j].name,
             fun->out[j].type->name);
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
