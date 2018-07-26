#ifndef RTBUF_LIB_H
#define RTBUF_LIB_H

#include "rtbuf.h"

#define RTBUF_LIB_MAX 1000
#define RTBUF_LIB_VER 0x00010001

typedef struct rtbuf_lib_fun {
  const char *name;
  f_rtbuf_fun *f;
  f_rtbuf_fun *start;
  f_rtbuf_fun *stop;
  unsigned int nmemb; /* number of members in buffer data */
  unsigned int size;  /* size of members in bytes in buffer data */
  const char **var;  /* names of variables, end with NULL */
} s_rtbuf_lib_fun;

typedef struct rtbuf_lib {
  const char *name;
  s_rtbuf_fun **fun;
  unsigned int fun_n;
  void *lib;
  const char *path;
} s_rtbuf_lib;

extern s_rtbuf_lib  g_rtbuf_lib[];
extern unsigned int g_rtbuf_lib_n;

void          rtbuf_lib_delete (s_rtbuf_lib *rl);
int           rtbuf_lib_find (const char *str);
int           rtbuf_lib_find_fun (s_rtbuf_lib *rl, const char *str);
void          rtbuf_lib_init ();
s_rtbuf_lib * rtbuf_lib_load (const char *path);
s_rtbuf_lib * rtbuf_lib_new ();

#endif
