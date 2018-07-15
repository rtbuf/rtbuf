
#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <strings.h>
#include "rtbuf_lib.h"

s_rtbuf_lib g_rtbuf_lib[RTBUF_LIB_MAX];
unsigned int g_rtbuf_lib_n = 0;

s_rtbuf_lib * rtbuf_lib_find (const char *str)
{
  unsigned int i = atoi(str);
  if (i < RTBUF_LIB_MAX) {
    s_rtbuf_lib *lib = &g_rtbuf_lib[i];
    if (lib->path && lib->lib && lib->fun)
      return lib;
  }
  return 0;
}

s_rtbuf_fun * rtbuf_lib_find_fun (s_rtbuf_lib *rl, const char *str)
{
  unsigned int i = atoi(str);
  if (i < rl->fun_n)
    return &rl->fun[i];
  return 0;
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
    if (lib->path == 0)
      return lib;
    i++;
    lib++;
  }
  g_rtbuf_lib_n++;
  return 0;
}

void rtbuf_lib_delete (s_rtbuf_lib *rl)
{
  bzero(rl, sizeof(s_rtbuf_lib));
  g_rtbuf_lib_n--;
}

s_rtbuf_lib * rtbuf_lib_load (const char *path)
{
  s_rtbuf_lib *lib = rtbuf_lib_new();
  unsigned int *ver;
  if (!lib)
    return 0;
  lib->path = path;
  lib->lib = dlopen(path, 0);
  if (!lib->lib) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  ver = dlsym(lib, "rtbuf_ver");
  assert(*ver == RTBUF_LIB_VER);
  lib->fun = dlsym(lib, "rtbuf_lib_fun");
  lib->fun_n = 0;
  while (lib->fun_n < RTBUF_LIB_MAX &&
         lib->fun[lib->fun_n].f)
    lib->fun_n++;
  return lib;
}
