
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rtbuf_lib.h"

s_rtbuf_lib g_rtbuf_lib[RTBUF_LIB_MAX];
unsigned int g_rtbuf_lib_n = 0;

char g_rtbuf_lib_user_dir[1024];
char *g_rtbuf_lib_path[] = { "./",
                             g_rtbuf_lib_user_dir,
                             "/usr/local/lib/rtbuf/",
                             "/usr/lib/rtbuf/",
                             0 };

void rtbuf_lib_init ()
{
  char *in = getenv("HOME");
  char *out = g_rtbuf_lib_user_dir;
  bzero(g_rtbuf_lib, sizeof(g_rtbuf_lib));
  if (!in)
    in = ".";
  while (*in)
    *out++ = *in++;
  in = "/.rtbuf/lib/";
  while (*in)
    *out++ = *in++;
  *out = 0;
}

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

void rtbuf_lib_load_path (s_rtbuf_lib *lib, const char *name)
{
  char **path = g_rtbuf_lib_path;
  lib->lib = 0;
  while (*path && !lib->lib) {
    const char *in = *path++;
    char *out = lib->path;
    while (*in)
      *out++ = *in++;
    in = name;
    while (*in)
      *out++ = *in++;
    in = ".so";
    while (*in)
      *out++ = *in++;
    *out = 0;
    printf("lib_load path \"%s\"\n", lib->path);
    lib->lib = dlopen(lib->path, 0);
    printf("lib_load lib %p\n", lib->lib);
  }
}

s_rtbuf_lib * rtbuf_lib_load (const char *name)
{
  s_rtbuf_lib *lib = rtbuf_lib_new();
  unsigned long *ver;
  if (!lib)
    return 0;
  rtbuf_lib_load_path(lib, name);
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
