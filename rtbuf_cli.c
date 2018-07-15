
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <cli.h>
#include "rtbuf_lib.h"

void print_lib (unsigned int i)
{
  printf("#<lib %i \"%s\">\n", i, g_rtbuf_lib[i].path);
}

void print_rtbuf (unsigned int i)
{
  printf("#<rtbuf %i>\n", i);
}

int rtbuf_cli_libs (int argc, const char *argv[])
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_lib_n;
  assert(argc == 0);
  (void) argv;
  printf("Listing %i libraries :\n", n);
  while (i < RTBUF_LIB_MAX && n > 0) {
    if (g_rtbuf_lib[i].path) {
      print_lib(i);
      n--;
    }
    i++;
  }
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
  i = (lib - g_rtbuf_lib) / sizeof(s_rtbuf_lib);
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
      n--;
    }
    i++;
  }
  return 0;
}

int rtbuf_cli_buffer (int argc, const char *argv[])
{
  unsigned int i;
  s_rtbuf *rtb;
  assert(argc == 1);
  i = atoi(argv[1]);
  if (i >= RTBUF_MAX || g_rtbuf[i].data == 0) {
    printf("buffer not found");
    return -1;
  }
  print_rtbuf_long(i);
  return 0;
}

int rtbuf_cli_new (int argc, const char *argv[])
{
  s_rtbuf_lib *rl;
  s_rtbuf_fun *rf;
  s_rtbuf *rtb;
  unsigned int i;
  assert(argc == 2);
  rl = rtbuf_lib_find(argv[1]);
  if (!rl) {
    printf("bad library\n");
    return -1;
  }
  rf = rtbuf_lib_find_fun(rl, argv[2]);
  if (!rf) {
    printf("bad function\n");
    return -1;
  }
  rtb = rtbuf_new(rf);
  i = (rtb - g_rtbuf) / sizeof(s_rtbuf);
  print_rtbuf(i);
  return i;
}

int rtbuf_cli_delete (int argc, const char *argv[])
{
  unsigned int i;
  assert(argc == 1);
  i = atoi(argv[1]);
  if (i >= RTBUF_MAX || g_rtbuf[i].data == 0)
    printf("rtbuf not found\n");
  rtbuf_delete(&g_rtbuf[i]);
  printf("deleted #<rtbuf %i>\n", i);
  return 0;
}

int rtbuf_cli_help (int argc, const char *argv[])
{
  (void) argc;
  (void) argv;
  printf("Available commands :\n"
         "  libs                   List loaded libraries.\n"
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
  { "load",    1, rtbuf_cli_load },
  { "buffers", 0, rtbuf_cli_buffers },
  { "buffer",  1, rtbuf_cli_buffer },
  { "new",     2, rtbuf_cli_new },
  { "delete",  1, rtbuf_cli_delete },
  /*
  { "bind",    3, rtbuf_cli_bind },
  { "unbind",  2, rtbuf_cli_unbind },
  */
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
