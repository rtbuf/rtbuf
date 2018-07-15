#ifndef RTBUF_H
#define RTBUF_H

typedef struct rtbuf_spec
{
  unsigned int nmemb;
  unsigned int size;
  unsigned int nvar;
} s_rtbuf_spec;

typedef struct rtbuf s_rtbuf;

typedef void f_rtbuf_fun (s_rtbuf *rtbuf);

typedef struct rtbuf_fun
{
  f_rtbuf_fun *f;
  s_rtbuf_spec spec;
} s_rtbuf_fun;

#define RTBUF_SORT   0x0001
#define RTBUF_DELETE 0x0002

struct rtbuf
{
  char *data;
  unsigned int flags;
  s_rtbuf_fun *fun;
  unsigned int refc;
  s_rtbuf **var;
};

typedef int f_rtbuf_lib_init ();

#define RTBUF_MAX 1000

extern s_rtbuf      g_rtbuf[];
extern unsigned int g_rtbuf_max;
extern unsigned int g_rtbuf_n;
extern unsigned int g_rtbuf_sort;
extern s_rtbuf *    g_rtbuf_sorted[];
extern unsigned int g_rtbuf_sorted_n;

int       rtbuf_init ();
void      rtbuf_delete (s_rtbuf *rtb);
s_rtbuf * rtbuf_new (s_rtbuf_fun *rf);
void      rtbuf_run ();
void      rtbuf_sort ();
void      rtbuf_unbind (s_rtbuf *rtb);
void      rtbuf_var_unbind (s_rtbuf *rtb, unsigned int var);
void      rtbuf_var_bind (s_rtbuf *rtb, unsigned int var,
                          s_rtbuf *target);

#endif /* RTBUF_H */
