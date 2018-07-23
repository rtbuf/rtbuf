
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <cli.h>
#include "rtbuf_lib.h"

pthread_t g_rtbuf_cli_thread = 0;

void print_lib (unsigned int i)
{
  assert(i < RTBUF_LIB_MAX);
  printf("#<lib %i \"%s\">\n", i, g_rtbuf_lib[i].path);
}

void print_lib_long (unsigned int i)
{
  s_rtbuf_lib *lib;
  unsigned int j = 0;
  assert(i < RTBUF_LIB_MAX);
  lib = &g_rtbuf_lib[i];
  printf("#<lib %i \"%s\"\n", i, lib->path);
  while (j < lib->fun_n) {
    s_rtbuf_fun *fun = &lib->fun[j];
    printf("  #<fun %i %i %i %i>\n", j, fun->spec.nmemb, fun->spec.size,
           fun->spec.nvar);
    j++;
  }
  printf("  >\n");
  fflush(stdout);
}

void print_rtbuf (unsigned int i)
{
  assert(i < RTBUF_MAX);
  printf("#<rtbuf %u>", i);
  fflush(stdout);
}

void print_rtbuf_long (unsigned int i)
{
  s_rtbuf *rtb;
  unsigned int j = 0;
  assert(i < RTBUF_MAX);
  rtb = &g_rtbuf[i];
  printf("#<rtbuf %i", i);
  printf(" %p", rtb->data);
  if (rtb->data) {
    printf(" %x", rtb->flags);
    printf(" %d", rtb->refc);
  }
  while (j < rtb->fun->spec.nvar) {
    if (rtb->var[j] >= 0) {
      printf("\n  var[%i] = ", j);
      print_rtbuf(rtb->var[j]);
    }
    j++;
  }
  printf(">\n");
  fflush(stdout);
}

int rtbuf_cli_libs (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_lib_n;
  assert(argc == 0);
  (void) argv;
  printf("Listing %i libraries :\n", n);
  while (i < RTBUF_LIB_MAX && n > 0) {
    if (g_rtbuf_lib[i].path[0]) {
      print_lib(i);
      n--;
    }
    i++;
  }
  return 0;
}

int rtbuf_cli_lib (int argc, const char *argv[])
{
  unsigned int i;
  assert(argc == 1);
  i = atoi(argv[1]);
  if (i >= RTBUF_LIB_MAX) {
    printf("library not found\n");
    return -1;
  }
  print_lib_long(i);
  return 0;
}

int rtbuf_cli_load (int argc, const char *argv[])
{
  s_rtbuf_lib *lib;
  unsigned int i;
  assert(argc == 1);
  lib = rtbuf_lib_load(argv[1]);
  if (!lib) {
    printf("load failed\n");
    return -1;
  }
  i = ((void*) lib - (void*) g_rtbuf_lib) / sizeof(s_rtbuf_lib);
  print_lib(i);
  return 0;
}

int rtbuf_cli_buffers (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_n;
  assert(argc == 0);
  (void) argv;
  printf("Listing %i buffers :\n", n);
  while (i < RTBUF_MAX && n > 0) {
    if (g_rtbuf[i].data) {
      print_rtbuf(i);
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
  print_rtbuf_long(i);
  return 0;
}

int rtbuf_cli_new (int argc, const char *argv[])
{
  int rl;
  int rf;
  int rtb;
  if (argc != 2)
    return rtbuf_err("usage: new LIBRARY FUNCTION");
  if ((rl = rtbuf_lib_find(argv[1])) < 0)
    return rtbuf_err("library not found");
  if ((rf = rtbuf_lib_find_fun(&g_rtbuf_lib[rl], argv[2])) < 0)
    return rtbuf_err("function not found");
  if ((rtb = rtbuf_new(&g_rtbuf_lib[rl].fun[rf])) < 0)
    return rtbuf_err("buffer not created");
  print_rtbuf(rtb);
  printf("\n");
  return 0;
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
  print_rtbuf(i);
  printf("\n");
  return 0;
}

int rtbuf_cli_bind (int argc, const char *argv[])
{
  int rtb;
  int var;
  int target;
  if (argc != 3)
    return -1;
  if ((rtb = rtbuf_find(argv[1])) < 0)
    return rtbuf_err("rtbuf not found");
  var = atoi(argv[2]);
  if (var < 0 || (unsigned int) var >= g_rtbuf[rtb].fun->spec.nvar)
    return rtbuf_err("variable not found");
  if ((target = rtbuf_find(argv[3])) < 0)
    return rtbuf_err("target not found");
  rtbuf_var_bind(&g_rtbuf[rtb], var, target);
  print_rtbuf_long(rtb);
  return 0;
}

int rtbuf_cli_unbind (int argc, const char *argv[])
{
  int rtb;
  if (argc < 1 || argc > 2)
    return rtbuf_err("usage: unbind BUFFER [VARIABLE]");
  if ((rtb = rtbuf_find(argv[1])) < 0)
    return rtbuf_err("buffer not found");
  if (argc == 2) {
    int var = atoi(argv[2]);
    if (var < 0 || (unsigned int) var >= g_rtbuf[rtb].fun->spec.nvar)
      return rtbuf_err("variable not found");
    rtbuf_var_unbind(&g_rtbuf[rtb], var);
  }
  else
    rtbuf_unbind(&g_rtbuf[rtb]);
  print_rtbuf_long(rtb);
  return 0;
}

void * rtbuf_cli_thread_fun (void *arg)
{
  (void) arg;
  rtbuf_start();
  g_rtbuf_run = 1;
  while (g_rtbuf_run) {
    rtbuf_run();
  }
  rtbuf_stop();
  return 0;
}

int rtbuf_cli_start (int argc, const char *argv[])
{
  (void) argv;
  if (argc != 0)
    return rtbuf_err("usage: start");
  if (!g_rtbuf_cli_thread) {
    if (pthread_create(&g_rtbuf_cli_thread, 0, &rtbuf_cli_thread_fun,
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
  g_rtbuf_run = 0;
  if (pthread_join(g_rtbuf_cli_thread, 0))
    return rtbuf_err("pthread_join failed");
  g_rtbuf_cli_thread = 0;
  return 0;
}

int rtbuf_cli_help (int argc, const char *argv[])
{
  (void) argc;
  (void) argv;
  printf("Available commands :\n"
         "  libs                   List loaded libraries.\n"
         "  lib N                  Show library N.\n"
         "  load PATH              Load library at PATH.\n"
         "  buffers                List buffers.\n"
         "  buffer N               Show buffer N.\n"
         "  new LIB FUN            Instanciate library function.\n"
         "  delete RTBUF           Unlink and delete RTBUF.\n"
         "  bind RTBUF VAR TARGET  Bind RTBUF VAR to TARGET.\n"
         "  unbind RTBUF VAR       Unbind RTBUF VAR.\n"
         "  help                   Show this help message.\n"
         "  quit                   Quit RTBUF.\n");
  return 0;
}

int rtbuf_cli_quit (int argc, const char *argv[])
{
  assert(argc == 0);
  (void) argv;
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
  { "bind",    3, rtbuf_cli_bind },
  { "unbind",  1, rtbuf_cli_unbind },
  { "unbind",  2, rtbuf_cli_unbind },
  { "start",   0, rtbuf_cli_start },
  { "stop",    0, rtbuf_cli_stop },
  { "h",       0, rtbuf_cli_help },
  { "help",    0, rtbuf_cli_help },
  { "quit",    0, rtbuf_cli_quit },
  { 0, 0, 0 }
};

void debug_read (int argc, const char **argv, f_cli f)
{
  if (argc < 1)
    return;
  printf("CLI READ %i %s(", argc, *argv);
  argv++;
  argc--;
  while (1) {
    if (argc < 1) {
      printf("); %p\n", f);
      return;
    }
    printf("%s", *argv);
    if (argc > 1)
      printf(", ");
    argv++;
    argc--;
  }
}

int repl ()
{
  s_cli cli;
  cli_init(&cli);
  cli_prompt(&cli, "rtbuf> ");
  cli_functions(&cli, rtbuf_cli_functions);
  while (1) {
    if (cli_read(&cli) < 0)
      return 0;
    /* debug_read(cli.argc, cli.argv, cli.f); */
    cli_eval(&cli);
  }
  return 0;
}

int main (int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  return repl();
}

int rtbuf_err (const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  return -1;
}
