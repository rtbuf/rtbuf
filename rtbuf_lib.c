
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rtbuf_lib.h"

s_rtbuf_lib g_rtbuf_lib[RTBUF_LIB_MAX];
unsigned int g_rtbuf_lib_n = 0;

int rtbuf_lib_find (const char *str)
{
  int i = atoi(str);
  if (0 <= i && i < RTBUF_LIB_MAX) {
    s_rtbuf_lib *lib = &g_rtbuf_lib[i];
    if (lib->path[0] && lib->lib && lib->fun)
      return i;
  }
  return -1;
}

int rtbuf_lib_find_fun (s_rtbuf_lib *rl, const char *str)
{
  int i = atoi(str);
  if (0 <= i && (unsigned) i < rl->fun_n)
    return i;
  return -1;
}

void rtbuf_lib_init ()
{
  bzero(g_rtbuf_lib, sizeof(g_rtbuf_lib));
}

s_rtbuf_lib * rtbuf_lib_new ()
{
  unsigned int i = 0;
  s_rtbuf_lib *lib = g_rtbuf_lib;
  while (i < RTBUF_LIB_MAX) {
    if (lib->path[0] == 0) {
      g_rtbuf_lib_n++;
      return lib;
    }
    i++;
    lib++;
  }
  return 0;
}

void rtbuf_lib_delete (s_rtbuf_lib *rl)
{
  if (rl->path[0])
    g_rtbuf_lib_n--;
  bzero(rl, sizeof(s_rtbuf_lib));
}

s_rtbuf_lib * rtbuf_lib_load (const char *path)
{
  s_rtbuf_lib *lib = rtbuf_lib_new();
  unsigned long *ver;
  if (!lib)
    return 0;
  strlcpy(lib->path, path, sizeof(lib->path));
  printf("lib_load path \"%s\"\n", lib->path);
  lib->lib = dlopen(path, 0);
  printf("lib_load lib %p\n", lib->lib);
  if (!lib->lib) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  ver = dlsym(lib->lib, "rtbuf_lib_ver");
  printf("lib_load ver %lu\n", *ver);
  assert(*ver == RTBUF_LIB_VER);
  lib->fun = dlsym(lib->lib, "rtbuf_lib_fun");
  lib->fun_n = 0;
  while (lib->fun_n < RTBUF_LIB_MAX &&
         lib->fun[lib->fun_n].f)
    lib->fun_n++;
  return lib;
}
