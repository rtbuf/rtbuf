
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rtbuf_lib.h"
#include "symbol.h"

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

int rtbuf_lib_p (s_rtbuf_lib *lib)
{
  return lib->lib ? 1 : 0;
}

int rtbuf_lib_find (const char *str)
{
  const char *sym;
  if ('0' <= str[0] && str[0] <= '9') {
    int i = atoi(str);
    if (0 <= i && i < RTBUF_LIB_MAX && rtbuf_lib_p(&g_rtbuf_lib[i]))
      return i;
  }
  if ((sym = find_symbol(str))) {
    unsigned int i = 0;
    unsigned int n = g_rtbuf_lib_n;
    while (i < RTBUF_LIB_MAX && n > 0) {
      if (rtbuf_lib_p(&g_rtbuf_lib[i])) {
        if (sym == g_rtbuf_lib[i].name)
          return i;
        n--;
      }
      i++;
    }
  }
  return -1;
}

int rtbuf_lib_find_fun (s_rtbuf_lib *rl, const char *str)
{
  const char *sym;
  if ('0' <= str[0] && str[0] <= '9') {
    int i = atoi(str);
    if (0 <= i && (unsigned) i < rl->fun_n)
      return i;
  }
  if ((sym = find_symbol(str))) {
    unsigned int i = 0;
    while (i < rl->fun_n) {
      if (sym == rl->fun[i]->name)
        return i;
      i++;
    }
  }
  return -1;
}

s_rtbuf_lib * rtbuf_lib_new ()
{
  unsigned int i = 0;
  s_rtbuf_lib *lib = g_rtbuf_lib;
  while (i < RTBUF_LIB_MAX) {
    if (lib->path == 0) {
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
    lib->path = &g_string[g_string_n];
    while (*in)
      g_string[g_string_n++] = *in++;
    in = name;
    while (*in)
      g_string[g_string_n++] = *in++;
    in = ".so";
    while (*in)
      g_string[g_string_n++] = *in++;
    g_string[g_string_n++] = 0;
    printf("lib_load path \"%s\"\n", lib->path);
    lib->lib = dlopen(lib->path, 0);
  }
}

int rtbuf_lib_fun_p (s_rtbuf_lib_fun *fun)
{
  return fun->name || fun->f || fun->start || fun->stop;
}

s_rtbuf_lib * rtbuf_lib_load (const char *name)
{
  s_rtbuf_lib *lib = rtbuf_lib_new();
  s_rtbuf_lib_fun *fun;
  unsigned long *ver;
  unsigned int i = 0;
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
  lib->name = intern(name);
  printf("lib_load name %s\n", lib->name);
  fun = dlsym(lib->lib, "rtbuf_lib_fun");
  lib->fun_n = 0;
  while (lib->fun_n < RTBUF_LIB_MAX &&
         rtbuf_lib_fun_p(&fun[lib->fun_n])) {
    if (fun[lib->fun_n].name == 0) {
      Dl_info info;
      dladdr(fun[lib->fun_n].f, &info);
      fun[lib->fun_n].name = intern(info.dli_sname);
    }
    lib->fun_n++;
  }
  lib->fun = malloc(sizeof(s_rtbuf_fun*) * (lib->fun_n + 1));
  while (i < lib->fun_n) {
    lib->fun[i] = new_rtbuf_fun(&fun[i]);
    lib->fun[i]->lib = lib;
    lib->fun[i]->lib_fun = i;
    i++;
  }
  lib->fun[i] = 0;
  return lib;
}