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
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "symbol.h"

s_data_type  g_rtbuf_lib_type = {
  sizeof(s_rtbuf_lib) * 8,
  DATA_TYPE_BITS
};
s_data_alloc g_rtbuf_lib_alloc;
s_rtbuf_lib *g_rtbuf_lib;

char g_rtbuf_lib_user_dir[1024];
char *g_rtbuf_lib_path[] = { "./",
                             "./.libs/",
                             g_rtbuf_lib_user_dir,
                             "/usr/local/lib/",
                             "/usr/lib/",
                             0 };

void rtbuf_lib_init_ ()
{
  char *in = getenv("HOME");
  char *out = g_rtbuf_lib_user_dir;
  data_alloc_init(&g_rtbuf_lib_alloc, &g_rtbuf_lib_type,
                  RTBUF_LIB_MAX, 0, 0);
  g_rtbuf_lib = g_rtbuf_lib_alloc.mem;
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
    if (0 <= i && (unsigned int) i < g_rtbuf_lib_alloc.n &&
        rtbuf_lib_p(&g_rtbuf_lib[i]))
      return i;
  }
  if ((sym = symbol_find(str))) {
    unsigned int i = 0;
    unsigned int n = g_rtbuf_lib_alloc.n - g_rtbuf_lib_alloc.free_n;
    while (i < g_rtbuf_lib_alloc.n && n > 0) {
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
  if ((sym = symbol_find(str))) {
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
  s_rtbuf_lib *lib = data_new(&g_rtbuf_lib_alloc);
  return lib;
}

void rtbuf_lib_delete (s_rtbuf_lib *rl)
{
  data_delete(&g_rtbuf_lib_alloc, rl);
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
    in = "librtbuf_";
    while (*in)
      g_string[g_string_n++] = *in++;
    in = name;
    while (*in)
      g_string[g_string_n++] = *in++;
    in = ".so";
    while (*in)
      g_string[g_string_n++] = *in++;
    g_string[g_string_n++] = 0;
    //printf("lib_load path \"%s\"\n", lib->path);
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
  f_rtbuf_lib_init *init;
  if (!lib)
    return 0;
  rtbuf_lib_load_path(lib, name);
  if (!lib->lib) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  ver = dlsym(lib->lib, "rtbuf_lib_ver");
  //printf("lib_load ver %lu\n", *ver);
  assert(*ver == RTBUF_LIB_VER);
  lib->name = symbol_intern(name);
  //printf("lib_load name %s\n", lib->name);
  if ((init = dlsym(lib->lib, "rtbuf_lib_init")))
    if (init(lib) < 0) {
      rtbuf_lib_delete(lib);
      return 0;
    }
  fun = dlsym(lib->lib, "rtbuf_lib_fun");
  lib->fun_n = 0;
  while (lib->fun_n < RTBUF_LIB_MAX &&
         rtbuf_lib_fun_p(&fun[lib->fun_n])) {
    if (fun[lib->fun_n].name == 0) {
      Dl_info info;
      dladdr(fun[lib->fun_n].f, &info);
      fun[lib->fun_n].name = symbol_intern(info.dli_sname);
    }
    lib->fun_n++;
  }
  lib->fun = malloc(sizeof(s_rtbuf_fun*) * (lib->fun_n + 1));
  while (i < lib->fun_n) {
    lib->fun[i] = rtbuf_fun_new();
    assert(lib->fun[i]);
    rtbuf_lib_fun_init_fun(lib->fun[i], &fun[i]);
    lib->fun[i]->lib = lib;
    lib->fun[i]->lib_fun = i;
    i++;
  }
  lib->fun[i] = 0;
  return lib;
}

void rtbuf_lib_fun_var_init_fun (s_rtbuf_fun *fun,
                                 s_rtbuf_lib_fun_var *var)
{
  unsigned int i = 0;
  bzero(fun->var, sizeof(fun->var));
  if (var) {
    while (var->name && i < RTBUF_FUN_VAR_MAX) {
      s_rtbuf_fun_var *v = &fun->var[i];
      v->name = symbol_intern(var->name);
      v->type = rtbuf_type(var->type);
      var++;
      i++;
    }
    assert(i < RTBUF_FUN_VAR_MAX);
  }
  fun->var_n = i;
}

void rtbuf_lib_fun_out_init_fun (s_rtbuf_fun *fun,
                                 s_rtbuf_lib_fun_out *out)
{
  unsigned int i = 0;
  bzero(fun->out, sizeof(fun->out));
  if (out) {
    unsigned int offset = 0;
    while (out->name && i < RTBUF_FUN_OUT_MAX) {
      s_rtbuf_fun_out *o = &fun->out[i];
      o->name = symbol_intern(out->name);
      o->type = rtbuf_type(out->type);
      assert(o->type);
      o->offset = offset;
      offset += (o->type->t.bits + 7) / 8;
      out++;
      i++;
    }
    assert(i < RTBUF_FUN_OUT_MAX);
    fun->out_bytes = offset;
  }
  fun->out_n = i;
}

void rtbuf_lib_fun_init_fun (s_rtbuf_fun *fun, s_rtbuf_lib_fun *x)
{
  fun->name = symbol_intern(x->name);
  fun->f = x->f;
  fun->start = x->start;
  fun->stop = x->stop;
  rtbuf_lib_fun_var_init_fun(fun, x->var);
  rtbuf_lib_fun_out_init_fun(fun, x->out);
}

void rtbuf_lib_print (unsigned int i)
{
  assert(i < RTBUF_LIB_MAX);
  printf("#<lib %i %s>\n", i, g_rtbuf_lib[i].name);
}

void rtbuf_lib_print_long (unsigned int i)
{
  s_rtbuf_lib *lib;
  unsigned int j = 0;
  assert(i < RTBUF_LIB_MAX);
  lib = &g_rtbuf_lib[i];
  printf("#<lib %i %s", i, lib->name);
  printf("\n  %s", lib->path);
  while (j < lib->fun_n) {
    printf("\n  ");
    rtbuf_fun_print(lib->fun[j]);
    j++;
  }
  printf(">\n");
  fflush(stdout);
}
