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
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "../librtbuf/symbol.h"
#include "../librtbuf/rtbuf.h"
#include "../librtbuf/lib.h"
#include "../librtbuf/var.h"
#include "rtbuf_cli.h"

s_cli            g_cli;
pthread_t        g_rtbuf_cli_run_thread = 0;
f_rtbuf_position g_rtbuf_position_cb = 0;

int rtbuf_cli_libs (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_lib_alloc.n - g_rtbuf_lib_alloc.free_n;
  if (argc != 0)
    return rtbuf_err("usage: libs");
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
  if (argc != 1)
    return rtbuf_err("usage: load LIBRARY");
  lib = rtbuf_lib_load(argv[1]);
  if (!lib) {
    printf("load failed\n");
    return -1;
  }
  return 0;
}

int rtbuf_cli_buffers (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_alloc.n;
  if (argc != 0)
    return rtbuf_err("usage: buffers");
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
  short x;
  short y;
  if (argc != 1 && argc != 3)
    return rtbuf_err("usage: new LIBRARY [X Y]");
  if ((rl = rtbuf_lib_find(argv[1])) < 0)
    return rtbuf_err("library not found");
  if ((rtb = rtbuf_new(g_rtbuf_lib[rl].proc)) < 0)
    return rtbuf_err("buffer not created");
  if (argc == 3 &&
      g_rtbuf_position_cb &&
      sscanf(argv[2], "%hd", &x) == 1 &&
      sscanf(argv[3], "%hd", &y) == 1)
    g_rtbuf_position_cb(&g_rtbuf[rtb], x, y);
  return 0;
}


int rtbuf_cli_let (int argc, const char *argv[])
{
  s_rtbuf_var *v;
  short x;
  short y;
  if (argc < 4 || argv[2][0] != '=' || argv[2][1])
    return rtbuf_err("usage: let VAR = TYPE ARG [...]");
  if (strncmp("new", argv[3], 4) == 0) {
    int rl;
    int rtb;
    if (argc != 4 && argc != 6)
      return rtbuf_err("usage: let VAR = new LIBRARY [X Y]");
    if ((rl = rtbuf_lib_find(argv[4])) < 0)
      return rtbuf_err("library not found");
    if ((rtb = rtbuf_new(g_rtbuf_lib[rl].proc)) < 0)
      return rtbuf_err("buffer not created");
    v = rtbuf_var_rtbuf_set(argv[1], rtb);
    if (!v)
      return rtbuf_err("rtbuf_var_rtbuf_set failed");
    if (argc == 6 &&
        g_rtbuf_position_cb &&
        sscanf(argv[5], "%hd", &x) == 1 &&
        sscanf(argv[6], "%hd", &y) == 1)
      g_rtbuf_position_cb(&g_rtbuf[rtb], x, y);
    return 0;
  }
  else if (strncmp("buffer", argv[3], 7) == 0) {
    int rtb;
    if (argc != 4)
      return rtbuf_err("usage: let VAR = buffer N");
    if ((rtb = rtbuf_find(argv[4])) < 0)
      return rtbuf_err("buffer not found");
    v = rtbuf_var_rtbuf_set(argv[1], rtb);
    if (!v)
      return rtbuf_err("rtbuf_var_rtbuf_set failed");
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
  return 0;
}

void * rtbuf_cli_run_thread_proc (void *arg)
{
  (void) arg;
  printf("rtbuf thread: start\n");
  if (!rtbuf_start()) {
    g_rtbuf_run = 1;
    g_rtbuf_running = 1;
  }
  while (g_rtbuf_run) {
    if (rtbuf_run())
      g_rtbuf_run = 0;
  }
  printf("rtbuf thread: stop\n");
  rtbuf_stop();
  g_rtbuf_running = 0;
  return 0;
}

int rtbuf_cli_start ()
{
  if (!g_rtbuf_run && g_rtbuf_cli_run_thread) {
    if (pthread_join(g_rtbuf_cli_run_thread, 0))
      return rtbuf_err("pthread_join failed");
    g_rtbuf_cli_run_thread = 0;
  }
  if (!g_rtbuf_cli_run_thread) {
    if (pthread_create(&g_rtbuf_cli_run_thread, 0, &rtbuf_cli_run_thread_proc,
                       0))
      return rtbuf_err("rtbuf_cli_start: pthread_create failed");
  }
  return 0;
}

int rtbuf_cli_start_ (int argc, const char *argv[])
{
  (void) argv;
  if (argc != 0)
    return rtbuf_err("usage: start");
  return rtbuf_cli_start();
}

int rtbuf_cli_stop ()
{
  if (g_rtbuf_run)
    g_rtbuf_run = 0;
  if (g_rtbuf_cli_run_thread) {
    if (pthread_join(g_rtbuf_cli_run_thread, 0))
      return rtbuf_err("rtbuf_cli_stop: pthread_join failed");
    g_rtbuf_cli_run_thread = 0;
  }
  return 0;
}

int rtbuf_cli_stop_ (int argc, const char *argv[])
{
  (void) argv;
  if (argc != 0)
    return rtbuf_err("usage: stop");
  return rtbuf_cli_stop();
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
         " start                       Start run thread.\n"
         " stop                        Stop run thread.\n"
         " help                        Show this help message.\n"
         " exit                        Quit RTBUF.\n");
  return 0;
}

int rtbuf_cli_exit (int argc, const char *argv[])
{
  (void) argc;
  (void) argv;
  rtbuf_cli_stop();
  librtbuf_shutdown();
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
  { "start",   0, rtbuf_cli_start_ },
  { "stop",    0, rtbuf_cli_stop_ },
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

int rtbuf_cli_do_event ()
{
  if (cli_read_nonblocking(&g_cli) >= 0) {
    /* debug_read(cli.argc, cli.argv, cli.f); */
    cli_eval(&g_cli);
    return 1;
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
