/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "rtbuf.h"
#include "lib.h"
#include "symbol.h"

s_data_type  g_rtbuf_lib_type = {
  sizeof(s_rtbuf_lib),
  DATA_TYPE_BYTES
};
s_data_alloc g_rtbuf_lib_alloc;
s_rtbuf_lib *g_rtbuf_lib;

char g_rtbuf_lib_user_dir[1024];
char *g_rtbuf_lib_path[] = { "./lib/",
                             g_rtbuf_lib_user_dir,
                             RTBUF_LIBDIR "/",
                             0 };

void rtbuf_lib_init_ (void)
{
  char *in = getenv("HOME");
  char *out = g_rtbuf_lib_user_dir;
  data_alloc_init(&g_rtbuf_lib_alloc, &g_rtbuf_lib_type,
                  RTBUF_LIB_MAX, 0, 0);
  g_rtbuf_lib = g_rtbuf_lib_alloc.mem;
  assert(g_rtbuf_lib);
  if (!in)
    in = ".";
  while (*in)
    *out++ = *in++;
  in = "/.rtbuf/lib/rtbuf/" VER "/";
  while (*in)
    *out++ = *in++;
  *out = 0;
}

void rtbuf_lib_shutdown (void)
{
  if (g_rtbuf_lib) {
    rtbuf_lib_delete_all();
    g_rtbuf_lib = 0;
    data_alloc_clean(&g_rtbuf_lib_alloc);
  }
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

s_rtbuf_lib * rtbuf_lib_new (void)
{
  s_rtbuf_lib *lib = data_new(&g_rtbuf_lib_alloc);
  return lib;
}

void rtbuf_lib_delete (s_rtbuf_lib *rl)
{
  assert(rl);
  if (rl->proc) {
    if (rl->unload)
      rl->unload(rl);
    rtbuf_lib_proc_clean(rl->proc);
    rtbuf_proc_delete(rl->proc);
    rl->proc = 0;
  }
  data_delete(&g_rtbuf_lib_alloc, rl);
}

void rtbuf_lib_delete_all (void)
{
  unsigned i = 0;
  int n = g_rtbuf_lib_alloc.n;
  while (i < g_rtbuf_lib_alloc.max && n > 0) {
    if (g_rtbuf_lib[i].name) {
      rtbuf_lib_delete(g_rtbuf_lib + i);
      n--;
    }
    i++;
  }
}

int rtbuf_lib_load_path (s_rtbuf_lib *lib, const char *name)
{
  char **path = g_rtbuf_lib_path;
  while (*path) {
    char *lib_path;
    char *ext;
    lib_path = g_str_append(*path, strlen(*path));
    g_str_append(name, strlen(name));
    ext = g_str_append(".so", 4);
    /*printf("lib find in path \"%s\"\n", lib_path);*/
    if (access(lib_path, R_OK) == 0 &&
        (lib->lib = dlopen(lib_path, RTLD_NOW))) {
      lib->path = lib_path;
      return 1;
    }
    g_str_reset(ext);
    g_str_append(".so.0.0", 8);
    /*printf("lib find in path \"%s\"\n", lib_path);*/
    if (access(lib_path, R_OK) == 0 &&
        (lib->lib = dlopen(lib_path, RTLD_NOW))) {
      lib->path = lib_path;
      return 1;
    }
    g_str_reset(ext);
    g_str_append("-0.dll", 7);
    /*printf("lib find in path \"%s\"\n", lib_path);*/
    if (access(lib_path, R_OK) == 0 &&
        (lib->lib = dlopen(lib_path, RTLD_NOW))) {
      lib->path = lib_path;
      return 1;
    }
    g_str_reset(lib_path);
    path++;
  }
  return 0;
}

int rtbuf_lib_proc_p (s_rtbuf_lib_proc *proc)
{
  return proc->name || proc->f || proc->start || proc->stop;
}

s_rtbuf_lib * rtbuf_lib_load (const char *name)
{
  s_rtbuf_lib *lib = rtbuf_lib_new();
  s_rtbuf_lib_proc *proc;
  unsigned long *ver;
  u_rtbuf_lib_init init;
  u_rtbuf_lib_unload unload;
  if (!lib)
    return 0;
  rtbuf_lib_load_path(lib, name);
  if (!lib->lib) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  ver = dlsym(lib->lib, "rtbuf_lib_ver");
  /* printf("lib_load ver %lu\n", *ver); */
  if (*ver != RTBUF_LIB_VER) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  lib->name = symbol_intern(name);
  /* printf("lib_load name %s\n", lib->name); */
  if ((init.ptr = dlsym(lib->lib, "rtbuf_lib_init")))
    if (init.fun(lib) < 0) {
      rtbuf_lib_delete(lib);
      return 0;
    }
  unload.ptr = dlsym(lib->lib, "rtbuf_lib_unload");
  lib->unload = unload.fun;
  proc = dlsym(lib->lib, "rtbuf_lib_proc");
  lib->proc = rtbuf_proc_new();
  assert(lib->proc);
  rtbuf_lib_proc_init(lib->proc, proc);
  lib->proc->lib = lib;
  return lib;
}

unsigned int add_padding (unsigned int offset, unsigned int size)
{
  unsigned int align = 1;
  if (size == 2)
    align = 2;
  else if (size == 4)
    align = 4;
  else if (size == 8)
    align = 8;
  else if (size == 16)
    align = 16;
  return (offset + align - 1) / align * align;
}

void rtbuf_lib_proc_init_in (s_rtbuf_proc *proc,
                             s_rtbuf_lib_proc_in *in)
{
  unsigned int i = 0;
  unsigned int offset = proc->out_bytes;
  bzero(proc->in, sizeof(proc->in));
  if (in) {
    while (in->name && i < RTBUF_PROC_IN_MAX) {
      s_rtbuf_proc_in *v = &proc->in[i];
      unsigned int size;
      v->name = symbol_intern(in->name);
      v->type = rtbuf_type(in->type);
      v->name_type = g_str;
      g_str_append(v->name, strlen(v->name));
      g_str_append(": ", 2);
      g_str_append(v->type->name, strlen(v->type->name) + 1);
      v->def = in->def;
      v->min = in->min;
      v->max = in->max;
      v->log_base = in->log_base;
      size = sizeof(double);
      offset = add_padding(offset, size);
      v->offset = offset;
      offset += size;
      in++;
      i++;
    }
    assert(i < RTBUF_PROC_IN_MAX);
    offset = add_padding(offset, 8);
    proc->in_bytes = offset - proc->out_bytes;
  }
  proc->in_n = i;
  proc->bytes = offset;
  proc->type.bytes = offset;
  proc->type.type = DATA_TYPE_BYTES;
}

void rtbuf_lib_proc_init_out (s_rtbuf_proc *proc,
                              s_rtbuf_lib_proc_out *out)
{
  unsigned int i = 0;
  bzero(proc->out, sizeof(proc->out));
  if (out) {
    unsigned int offset = 0;
    while (out->name && i < RTBUF_PROC_OUT_MAX) {
      s_rtbuf_proc_out *o = &proc->out[i];
      unsigned int size;
      o->name = symbol_intern(out->name);
      o->type = rtbuf_type(out->type);
      o->name_type = g_str;
      g_str_append(o->name, strlen(o->name));
      g_str_append(": ", 2);
      g_str_append(o->type->name, strlen(o->type->name) + 1);
      assert(o->type);
      size = o->type->t.bytes;
      offset = add_padding(offset, size);
      o->offset = offset;
      offset += size;
      out++;
      i++;
    }
    assert(i < RTBUF_PROC_OUT_MAX);
    offset = add_padding(offset, 8);
    proc->out_bytes = offset;
  }
  proc->out_n = i;
}

void rtbuf_lib_proc_init (s_rtbuf_proc *proc, s_rtbuf_lib_proc *x)
{
  proc->name = symbol_intern(x->name);
  proc->f = x->f;
  proc->start = x->start;
  proc->stop = x->stop;
  rtbuf_lib_proc_init_out(proc, x->out);
  rtbuf_lib_proc_init_in(proc, x->in);
  data_alloc_init(&proc->alloc, &proc->type, RTBUF_INSTANCE_MAX,
                  NULL, NULL);
}

void rtbuf_lib_proc_clean (s_rtbuf_proc *proc)
{
  data_alloc_clean(&proc->alloc);
}

void rtbuf_lib_print (const s_rtbuf_lib *lib)
{
  unsigned int i;
  if (lib >= g_rtbuf_lib &&
      (i = lib - g_rtbuf_lib) < RTBUF_LIB_MAX)
    printf("#<lib %i %s>\n", i, lib->name);
  else
    printf("#<lib %p %s>\n", (void*) lib, lib->name);
}

void rtbuf_lib_print_long (unsigned int i)
{
  s_rtbuf_lib *lib;
  assert(i < RTBUF_LIB_MAX);
  lib = &g_rtbuf_lib[i];
  printf("#<lib %i %s", i, lib->name);
  printf("\n  %s", lib->path);
  printf("\n  ");
  rtbuf_proc_print(lib->proc);
  printf(">\n");
  fflush(stdout);
}
