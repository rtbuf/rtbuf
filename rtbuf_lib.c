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
char *g_rtbuf_lib_path[] = { "",
                             "./",
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

int rtbuf_lib_find_proc (s_rtbuf_lib *rl, const char *str)
{
  const char *sym;
  if ('0' <= str[0] && str[0] <= '9') {
    int i = atoi(str);
    if (0 <= i && (unsigned) i < rl->proc_n)
      return i;
  }
  if ((sym = symbol_find(str))) {
    unsigned int i = 0;
    while (i < rl->proc_n) {
      if (sym == rl->proc[i]->name)
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
  unsigned int i = 0;
  assert(rl);
  if (rl->proc) {
    while (i < rl->proc_n) {
      rtbuf_proc_delete(rl->proc[i]);
      rl->proc[i] = 0;
      i++;
    }
    free(rl->proc);
  }
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
    printf("lib_load path \"%s\"\n", lib->path);
    lib->lib = dlopen(lib->path, RTLD_LAZY);
  }
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
  unsigned int i = 0;
  union {
    void *ptr;
    f_rtbuf_lib_init *init;
  } init_ptr;
  if (!lib)
    return 0;
  rtbuf_lib_load_path(lib, name);
  if (!lib->lib) {
    rtbuf_lib_delete(lib);
    return 0;
  }
  ver = dlsym(lib->lib, "rtbuf_lib_ver");
  /* printf("lib_load ver %lu\n", *ver); */
  assert(*ver == RTBUF_LIB_VER);
  lib->name = symbol_intern(name);
  /* printf("lib_load name %s\n", lib->name); */
  if ((init_ptr.ptr = dlsym(lib->lib, "rtbuf_lib_init")))
    if (init_ptr.init(lib) < 0) {
      rtbuf_lib_delete(lib);
      return 0;
    }
  proc = dlsym(lib->lib, "rtbuf_lib_proc");
  lib->proc_n = 0;
  while (lib->proc_n < RTBUF_PROC_MAX &&
         rtbuf_lib_proc_p(&proc[lib->proc_n]))
    lib->proc_n++;
  lib->proc = malloc(sizeof(s_rtbuf_proc*) * (lib->proc_n + 1));
  while (i < lib->proc_n) {
    lib->proc[i] = rtbuf_proc_new();
    assert(lib->proc[i]);
    rtbuf_lib_proc_init_proc(lib->proc[i], &proc[i]);
    lib->proc[i]->lib = lib;
    lib->proc[i]->lib_proc = i;
    i++;
  }
  lib->proc[i] = 0;
  return lib;
}

void rtbuf_lib_proc_in_init_proc (s_rtbuf_proc *proc,
                                  s_rtbuf_lib_proc_in *in)
{
  unsigned int i = 0;
  bzero(proc->in, sizeof(proc->in));
  if (in) {
    while (in->name && i < RTBUF_PROC_IN_MAX) {
      s_rtbuf_proc_in *v = &proc->in[i];
      v->name = symbol_intern(in->name);
      v->type = rtbuf_type(in->type);
      v->def = in->def;
      v->min = in->min;
      v->max = in->max;
      in++;
      i++;
    }
    assert(i < RTBUF_PROC_IN_MAX);
  }
  proc->in_n = i;
}

void rtbuf_lib_proc_out_init_proc (s_rtbuf_proc *proc,
                                   s_rtbuf_lib_proc_out *out)
{
  unsigned int i = 0;
  bzero(proc->out, sizeof(proc->out));
  if (out) {
    unsigned int offset = 0;
    while (out->name && i < RTBUF_PROC_OUT_MAX) {
      s_rtbuf_proc_out *o = &proc->out[i];
      o->name = symbol_intern(out->name);
      o->type = rtbuf_type(out->type);
      assert(o->type);
      o->offset = offset;
      offset += (o->type->t.bits + 7) / 8;
      out++;
      i++;
    }
    assert(i < RTBUF_PROC_OUT_MAX);
    proc->out_bytes = offset;
  }
  proc->out_n = i;
}

void rtbuf_lib_proc_init_proc (s_rtbuf_proc *proc, s_rtbuf_lib_proc *x)
{
  proc->name = symbol_intern(x->name);
  proc->f = x->f;
  proc->start = x->start;
  proc->stop = x->stop;
  rtbuf_lib_proc_in_init_proc(proc, x->in);
  rtbuf_lib_proc_out_init_proc(proc, x->out);
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
  while (j < lib->proc_n) {
    printf("\n  ");
    rtbuf_proc_print(lib->proc[j]);
    j++;
  }
  printf(">\n");
  fflush(stdout);
}
