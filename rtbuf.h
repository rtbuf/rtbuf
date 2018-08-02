#ifndef RTBUF_H
#define RTBUF_H

#include "rtbuf_defs.h"
#include "rtbuf_type.h"
#include "rtbuf_fun.h"

#define RTBUF_SORT   0x0001
#define RTBUF_DELETE 0x0002

struct rtbuf_binding
{
  int rtb;
  unsigned int out;
};

struct rtbuf
{
  void           *data;
  unsigned int    flags;
  s_rtbuf_fun    *fun;
  unsigned int    refc;
  s_rtbuf_binding var[RTBUF_FUN_VAR_MAX];
  unsigned int    var_n;
};

#define RTBUF_MAX 1000

extern s_rtbuf      g_rtbuf[];
extern unsigned int g_rtbuf_max;
extern unsigned int g_rtbuf_n;
extern int          g_rtbuf_run;
extern unsigned int g_rtbuf_sort;
extern int          g_rtbuf_sorted[];
extern unsigned int g_rtbuf_sorted_n;

int  rtbuf_init ();
void rtbuf_delete (s_rtbuf *rtb);
int  rtbuf_err (const char *msg);
int  rtbuf_find (const char *x);
int  rtbuf_out_find (s_rtbuf *rtb, const char *x);
int  rtbuf_new (s_rtbuf_fun *rf);
void rtbuf_run ();
void rtbuf_sort ();
void rtbuf_start();
void rtbuf_stop();
void rtbuf_unbind (s_rtbuf *rtb);
void rtbuf_var_unbind (s_rtbuf *rtb, unsigned int var);
void rtbuf_var_bind (s_rtbuf *rtb, unsigned int var,
                     unsigned int target, unsigned int target_out);

static inline double min (double a, double b)
{
  return a < b ? a : b;
}

static inline double max (double a, double b)
{
  return a < b ? b : a;
}

static inline double clamp (double inf, double x, double sup)
{
  return max(inf, min(x, sup));
}

#endif /* RTBUF_H */
