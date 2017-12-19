
#include <stdlib.h>
#include <stdio.h>
#include <cli.h>

int add_n (int argc, const char *argv[])
{
  int a = 0;
  while (argc--) {
    int b = atoi(*++argv);
    a += b;
  }
  printf("%i\n", a);
  return 0;
}

int sub_2 (int argc, const char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  (void) argc;
  printf("%i\n", a - b);
  return 0;
}

int mul_n (int argc, const char *argv[])
{
  int a = 1;
  while (argc--) {
    int b = atoi(*++argv);
    a *= b;
  }
  printf("%i\n", a);
  return 0;
}

int div_2 (int argc, const char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  (void) argc;
  printf("%i\n", a / b);
  return 0;
}

int mod_2 (int argc, const char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  (void) argc;
  printf("%i\n", a % b);
  return 0;
}

int min_2 (int argc, const char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  (void) argc;
  printf("%i\n", a < b ? a : b);
  return 0;
}

int max_2 (int argc, const char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  (void) argc;
  printf("%i\n", a < b ? b : a);
  return 0;
}

s_cli_function demo_functions[] = {
  { "+",   -1, add_n },
  { "-",    2, sub_2 },
  { "*",   -1, mul_n },
  { ":",    2, div_2 },
  { "%",    2, mod_2 },
  { "min",  2, min_2 },
  { "max",  2, max_2 },
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
  cli_prompt(&cli, "clidemo> ");
  cli_functions(&cli, demo_functions);
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
