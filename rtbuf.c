
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

void print_rtbuf (unsigned int i);
void print_rtbuf_fun (s_rtbuf_fun *fun);

int rtbuf_init ()
{
  bzero(g_rtbuf, sizeof(g_rtbuf));
  bzero(g_rtbuf_sorted, sizeof(g_rtbuf_sorted));
  return 0;
}

int rtbuf_next ()
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  while (i < RTBUF_MAX) {
    if (rtb->data == 0) {
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
  int rtb;
  unsigned int j = 0;
  assert(rf);
  if ((rtb = rtbuf_next()) < 0)
    return -1;
  g_rtbuf[rtb].data = calloc(rf->nmemb, rf->size);
  g_rtbuf[rtb].fun = rf;
  g_rtbuf[rtb].var = calloc(rf->var_n, sizeof(int));
  while (j < rf->var_n)
    g_rtbuf[rtb].var[j++] = -1;
  g_rtbuf_sort = 1;
  return rtb;
}

void rtbuf_var_unbind (s_rtbuf *rtb, unsigned int i)
{
  if (rtb->var[i] >= 0)
    g_rtbuf[rtb->var[i]].refc--;
  rtb->var[i] = -1;
}

void rtbuf_var_bind (s_rtbuf *rtb, unsigned int i, int target)
{
  if (rtb->var[i] == target)
    return;
  if (rtb->var[i] >= 0)
    g_rtbuf[rtb->var[i]].refc--;
  rtb->var[i] = target;
  g_rtbuf[target].refc++;
  g_rtbuf_sort = 1;
}

void rtbuf_unbind (s_rtbuf *rtb)
{
  unsigned int i = rtb->fun->var_n;
  while (i--)
    rtbuf_var_unbind(rtb, i);
}

void rtbuf_delete (s_rtbuf *rtb)
{
  rtbuf_unbind(rtb);
  free(rtb->data);
  free(rtb->var);
  bzero(rtb, sizeof(s_rtbuf));
  g_rtbuf_n--;
  g_rtbuf_sort = 1;
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
        rtbuf_delete(rtb);
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
  rtb = g_rtbuf[ptr->rtb].var[ptr->var];
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

void print_rtbuf_sorted ()
{
  unsigned int i = 0;
  while (i < g_rtbuf_sorted_n) {
    if (i)
      printf(" ");
    print_rtbuf(g_rtbuf_sorted[i]);
    i++;
  }
  printf("\n");
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
  print_rtbuf_sorted();
}

void rtbuf_start ()
{
  unsigned int i = 0;
  printf("rtbuf_start\n");
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = &g_rtbuf[g_rtbuf_sorted[i]];
    if (rtb->fun->start) {
      printf(" start ");
      print_rtbuf(g_rtbuf_sorted[i]);
      printf(" ");
      print_rtbuf_fun(rtb->fun);
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
    //print_rtbuf(g_rtbuf_sorted[i]);
    //printf("\n");
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