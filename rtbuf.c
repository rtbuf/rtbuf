
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf.h"

s_rtbuf      g_rtbuf[RTBUF_MAX];
unsigned int g_rtbuf_max = RTBUF_MAX;
unsigned int g_rtbuf_n = 0;
unsigned int g_rtbuf_sort = 0;
s_rtbuf *    g_rtbuf_sorted[RTBUF_MAX];
unsigned int g_rtbuf_sorted_n = 0;

int rtbuf_init ()
{
  bzero(g_rtbuf, sizeof(g_rtbuf));
  bzero(g_rtbuf_sorted, sizeof(g_rtbuf_sorted));
  return 0;
}

s_rtbuf * rtbuf_next ()
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  while (1) {
    if (i >= RTBUF_MAX)
      return 0;
    if (!rtb->data) {
      g_rtbuf_n++;
      return rtb;
    }
    rtb++;
    i++;
  }
}

s_rtbuf * rtbuf_new (s_rtbuf_fun *rf)
{
  assert(rf);
  s_rtbuf *rtb = rtbuf_next();
  if (!rtb)
    return 0;
  rtb->data = calloc(rf->spec.nmemb, rf->spec.size);
  rtb->fun = rf;
  rtb->var = calloc(rf->spec.nvar, sizeof(void *));
  g_rtbuf_sort = 1;
  return rtb;
}

void rtbuf_var_unbind (s_rtbuf *rtb, unsigned int i)
{
  if (rtb->var[i])
    rtb->var[i]->refc--;
  rtb->var[i] = 0;
}

void rtbuf_var_bind (s_rtbuf *rtb, unsigned int i, s_rtbuf *target)
{
  if (rtb->var[i] == target)
    return;
  if (rtb->var[i])
    rtb->var[i]->refc--;
  rtb->var[i] = target;
  target->refc++;
  g_rtbuf_sort = 1;
}

void rtbuf_unbind (s_rtbuf *rtb)
{
  unsigned int i = rtb->fun->spec.nvar;
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
  s_rtbuf *rtbuf;
  unsigned int var;
} s_rtbuf_var_ptr;

typedef struct rtbuf_var_stack {
  s_rtbuf_var_ptr *st[RTBUF_MAX];
  unsigned int size;
} s_rtbuf_var_stack;

void rtbuf_var_stack_init (s_rtbuf_var_stack *rvs)
{
  bzero(rvs, sizeof(s_rtbuf_var_stack));
}

s_rtbuf_var_ptr * rtbuf_var_stack_push (s_rtbuf_var_stack *rvs,
                                        s_rtbuf *rtbuf,
                                        unsigned int var)
{
  s_rtbuf_var_ptr *top;
  if (rvs->size >= RTBUF_MAX)
    return 0;
  top = rvs->st[rvs->size];
  top->rtbuf = rtbuf;
  top->var = var;
  rvs->size++;
  return top;
}

s_rtbuf_var_ptr * rtbuf_var_stack_pop (s_rtbuf_var_stack *rvs)
{
  if (rvs->size <= 0)
    return 0;
  rvs->size--;
  return rvs->st[rvs->size];
}

s_rtbuf_var_ptr * rtbuf_var_stack_top (s_rtbuf_var_stack *rvs)
{
  if (rvs->size <= 0)
    return 0;
  return rvs->st[rvs->size - 1];
}

void rtbuf_find_roots (s_rtbuf_var_stack *rvs)
{
  s_rtbuf *rtb = g_rtbuf;
  unsigned int i = 0;
  unsigned int n = g_rtbuf_n;
  while (i < RTBUF_MAX && n > 0) {
    if (rtb->flags & RTBUF_DELETE)
      rtbuf_delete(rtb);
    else {
      rtb->flags = rtb->flags & ~RTBUF_SORT;
      if (rtb->refc == 0)
        rtbuf_var_stack_push(rvs, rtb, 0);
    }
    rtb++;
    i++;
    n--;
  }
}

void rtbuf_sort_push_child (s_rtbuf_var_stack *rvs,
                            s_rtbuf_var_ptr *ptr)
{
  s_rtbuf *rtb = ptr->rtbuf->var[ptr->var];
  ptr->var++;
  if (rtb)
    rtbuf_var_stack_push(rvs, rtb, 0);
}

void rtbuf_sort ()
{
  s_rtbuf_var_stack rvs;
  s_rtbuf_var_ptr *ptr;
  unsigned int i = 0;
  if (g_rtbuf_n == 0)
    return;
  rtbuf_var_stack_init(&rvs);
  rtbuf_find_roots(&rvs);
  while ((ptr = rtbuf_var_stack_top(&rvs))) {
    if (ptr->var == ptr->rtbuf->fun->spec.nvar) {
      g_rtbuf_sorted[i++] = ptr->rtbuf;
      rtbuf_var_stack_pop(&rvs);
    } else
      rtbuf_sort_push_child(&rvs, ptr);
  }
  g_rtbuf_sorted_n = i;
  g_rtbuf_sort = 0;
}

void rtbuf_run ()
{
  unsigned int i = 0;
  if (g_rtbuf_sort)
    rtbuf_sort();
  while (i < g_rtbuf_sorted_n) {
    s_rtbuf *rtb = g_rtbuf_sorted[i];
    rtb->fun->f(rtb);
    i++;
  }
}
