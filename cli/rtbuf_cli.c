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
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include <rtbuf/symbol.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/var.h>

s_cli     g_cli;
pthread_t g_rtbuf_cli_thread = 0;

int rtbuf_cli_libs (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_lib_alloc.n - g_rtbuf_lib_alloc.free_n;
  assert(argc == 0);
  (void) argv;
  printf("Listing %i libraries :\n", n);
  while (i < g_rtbuf_lib_alloc.n && n > 0) {
    if (g_rtbuf_lib[i].path[0]) {
      rtbuf_lib_print(&g_rtbuf_lib[i]);
      n--;
    }
    i++;
  }
  return 0;
}

int rtbuf_cli_lib (int argc, const char *argv[])
{
  int i;
  if (argc != 1)
    return rtbuf_err("usage: lib LIBRARY");
  i = rtbuf_lib_find(argv[1]);
  if (0 <= i && i < RTBUF_LIB_MAX) {
    rtbuf_lib_print_long(i);
    return 0;
  }
  return rtbuf_err("library not found");
}

int rtbuf_cli_load (int argc, const char *argv[])
{
  s_rtbuf_lib *lib;
  assert(argc == 1);
  lib = rtbuf_lib_load(argv[1]);
  if (!lib) {
    printf("load failed\n");
    return -1;
  }
  rtbuf_lib_print(lib);
  return 0;
}

int rtbuf_cli_buffers (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_alloc.n;
  assert(argc == 0);
  (void) argv;
  printf("Listing %i buffers :\n", n);
  while (i < RTBUF_MAX && n > 0) {
    if (g_rtbuf[i].data) {
      rtbuf_print_long(i);
      printf("\n");
      n--;
    }
    i++;
  }
  return 0;
}

int rtbuf_cli_buffer (int argc, const char *argv[])
{
  int i;
  if (argc != 1)
    return rtbuf_err("usage: buffer N");
  if ((i = rtbuf_find(argv[1])) < 0)
    return rtbuf_err("buffer not found");
  rtbuf_print_long(i);
  return 0;
}

int rtbuf_cli_new (int argc, const char *argv[])
{
  int rl;
  int rtb;
  if (argc != 1)
    return rtbuf_err("usage: new LIBRARY");
  if ((rl = rtbuf_lib_find(argv[1])) < 0)
    return rtbuf_err("library not found");
  if ((rtb = rtbuf_new(g_rtbuf_lib[rl].proc)) < 0)
    return rtbuf_err("buffer not created");
  rtbuf_print(rtb);
  printf("\n");
  return 0;
}


int rtbuf_cli_let (int argc, const char *argv[])
{
  s_rtbuf_var *v;
  if (argc < 4 || argv[2][0] != '=' || argv[2][1])
    return rtbuf_err("usage: let VAR = TYPE ARG [...]");
  if (strncmp("new", argv[3], 4) == 0) {
    int rl;
    int rtb;
    if (argc != 4)
      return rtbuf_err("usage: let VAR = new LIBRARY");
    if ((rl = rtbuf_lib_find(argv[4])) < 0)
      return rtbuf_err("library not found");
    if ((rtb = rtbuf_new(g_rtbuf_lib[rl].proc)) < 0)
      return rtbuf_err("buffer not created");
    v = rtbuf_var_rtbuf_set(argv[1], rtb);
    assert(v);
    rtbuf_var_print(v);
    printf("\n");
    return 0;
  }
  else if (strncmp("buffer", argv[3], 7) == 0) {
    int rtb;
    if (argc != 4)
      return rtbuf_err("usage: let VAR = buffer N");
    if ((rtb = rtbuf_find(argv[4])) < 0)
      return rtbuf_err("buffer not found");
    v = rtbuf_var_rtbuf_set(argv[1], rtb);
    assert(v);
    rtbuf_var_print(v);
    return 0;
  }
  return rtbuf_err("unknown type for let");
}

int rtbuf_cli_delete (int argc, const char *argv[])
{
  int i;
  if (argc != 1)
    return rtbuf_err("usage: delete N");
  i = rtbuf_find(argv[1]);
  if (i < 0)
    return rtbuf_err("buffer not found\n");
  rtbuf_delete(&g_rtbuf[i]);
  rtbuf_print(i);
  printf("\n");
  return 0;
}

int rtbuf_cli_bind (int argc, const char *argv[])
{
  int src;
  int out;
  int dest;
  int in;
  if (argc != 4)
    return rtbuf_err("usage: bind SOURCE OUT DEST IN");
  if ((src = rtbuf_find(argv[1])) < 0)
    return rtbuf_err("source buffer not found");
  if ((out = rtbuf_out_find(&g_rtbuf[src], argv[2])) < 0)
    return rtbuf_err("output not found");
  if ((dest = rtbuf_find(argv[3])) < 0)
    return rtbuf_err("destination buffer not found");
  if ((in = rtbuf_in_find(&g_rtbuf[dest], argv[4])) < 0)
    return rtbuf_err("input not found");
  rtbuf_bind(src, out, &g_rtbuf[dest], in);
  rtbuf_print_long(dest);
  return 0;
}

int rtbuf_cli_unbind (int argc, const char *argv[])
{
  int rtb;
  if (argc < 1 || argc > 2)
    return rtbuf_err("usage: unbind BUFFER [INPUT]");
  if ((rtb = rtbuf_find(argv[1])) < 0)
    return rtbuf_err("buffer not found");
  if (argc == 2) {
    int in = atoi(argv[2]);
    if (in < 0 || (unsigned int) in >= g_rtbuf[rtb].proc->in_n)
      return rtbuf_err("input not found");
    rtbuf_in_unbind(&g_rtbuf[rtb], in);
  }
  else
    rtbuf_unbind_all(&g_rtbuf[rtb]);
  rtbuf_print_long(rtb);
  return 0;
}

void * rtbuf_cli_thread_proc (void *arg)
{
  (void) arg;
  printf("rtbuf thread: start\n");
  if (!rtbuf_start())
    g_rtbuf_run = 1;
  while (g_rtbuf_run) {
    if (rtbuf_run())
      g_rtbuf_run = 0;
  }
  printf("rtbuf thread: stop\n");
  rtbuf_stop();
  return 0;
}

int rtbuf_cli_start (int argc, const char *argv[])
{
  (void) argv;
  if (argc != 0)
    return rtbuf_err("usage: start");
  if (!g_rtbuf_run && g_rtbuf_cli_thread) {
    if (pthread_join(g_rtbuf_cli_thread, 0))
      return rtbuf_err("pthread_join failed");
    g_rtbuf_cli_thread = 0;
  }
  if (!g_rtbuf_cli_thread) {
    if (pthread_create(&g_rtbuf_cli_thread, 0, &rtbuf_cli_thread_proc,
                       0))
      return rtbuf_err("pthread_create failed");
  }
  return 0;
}

int rtbuf_cli_stop (int argc, const char *argv[])
{
  (void) argv;
  if (argc != 0)
    return rtbuf_err("usage: stop");
  if (g_rtbuf_run)
    g_rtbuf_run = 0;
  if (g_rtbuf_cli_thread) {
    if (pthread_join(g_rtbuf_cli_thread, 0))
      return rtbuf_err("pthread_join failed");
    g_rtbuf_cli_thread = 0;
  }
  return 0;
}

int rtbuf_cli_help (int argc, const char *argv[])
{
  (void) argc;
  (void) argv;
  printf("Available commands :\n"
         " libs                        List loaded libraries.\n"
         " lib N                       Show library N.\n"
         " load LIB                    Load library LIB.\n"
         " buffers                     List buffers.\n"
         " buffer N                    Show buffer N.\n"
         " let VAR = buffer N          Set variable to buffer.\n");
  printf(" new LIB                     Instanciate library.\n"
         " let VAR = new LIB           Set variable to new instance.\n"
         " delete BUFFER               Unlink and delete BUFFER.\n"
         " bind SOURCE OUT DEST IN     Bind SOURCE OUT to DEST IN.\n"
         " unbind BUFFER IN            Unbind BUFFER IN.\n"
         " help                        Show this help message.\n"
         " exit                        Quit RTBUF.\n");
  return 0;
}

int rtbuf_cli_exit (int argc, const char *argv[])
{
  (void) argc;
  (void) argv;
  rtbuf_cli_stop(0, 0);
  close(0);
  exit(0);
  return 0;
}

s_cli_function rtbuf_cli_functions[] = {
  { "libs",    0, rtbuf_cli_libs },
  { "lib",     1, rtbuf_cli_lib },
  { "load",    1, rtbuf_cli_load },
  { "buffers", 0, rtbuf_cli_buffers },
  { "buffer",  1, rtbuf_cli_buffer },
  { "new",     2, rtbuf_cli_new },
  { "delete",  1, rtbuf_cli_delete },
  { "bind",    4, rtbuf_cli_bind },
  { "unbind",  1, rtbuf_cli_unbind },
  { "unbind",  2, rtbuf_cli_unbind },
  { "let",    -1, rtbuf_cli_let },
  { "start",   0, rtbuf_cli_start },
  { "stop",    0, rtbuf_cli_stop },
  { "h",       0, rtbuf_cli_help },
  { "help",    0, rtbuf_cli_help },
  { "exit",    0, rtbuf_cli_exit },
  { 0, 0, 0 }
};

void debug_read (int argc, const char **argv, f_cli f)
{
  union {
    f_cli f;
    void *p;
  } fp;
  if (argc < 1)
    return;
  printf("CLI READ %i %s(", argc, *argv);
  argv++;
  argc--;
  fp.f = f;
  while (1) {
    if (argc < 1) {
      printf("); %p\n", fp.p);
      return;
    }
    printf("%s", *argv);
    if (argc > 1)
      printf(", ");
    argv++;
    argc--;
  }
}

void repl_init ()
{
  cli_init(&g_cli);
  cli_prompt(&g_cli, "rtbuf> ");
  cli_functions(&g_cli, rtbuf_cli_functions);
}

int load (const char *path)
{
  FILE *fp = fopen(path, "r");
  if (!fp)
    return -1;
  printf("; Loading %s\n", path);
  while (cli_read_file(&g_cli, fp) >= 0)
    cli_eval(&g_cli);
  fclose(fp);
  return 0;
}

int repl ()
{
  while (cli_read(&g_cli) >= 0) {
    /* debug_read(cli.argc, cli.argv, cli.f); */
    cli_eval(&g_cli);
  }
  return 0;
}

void rtbuf_cli_args (int argc, char *argv[])
{
  while (--argc) {
    char *arg = *++argv;
    printf("loading script %s\n", arg);
    load(arg);
  }
}

int main (int argc, char *argv[])
{
  symbols_init();
  librtbuf_init();
  assert(g_rtbuf);
  repl_init();
  rtbuf_cli_args(argc, argv);
  return repl();
}
