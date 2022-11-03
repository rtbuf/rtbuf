/* c3
 * Copyright 2018-2022 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted
 * the above copyright notice and this permission paragraph
 * are included in all copies and substantial portions of this
 * software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */

#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "cli.h"

void cli_init (s_cli *cli)
{
  if (isatty(0)) {
    cli->prompt = "> ";
    using_history();
  }
  else
    cli->prompt = 0;
  cli->functions = 0;
}

void cli_prompt (s_cli *cli, const char *prompt)
{
  if (isatty(0))
    cli->prompt = prompt;
  else
    cli->prompt = 0;
}

void cli_functions (s_cli *cli, const s_cli_function *functions)
{
  cli->functions = functions;
}

int cli_scan (s_cli *cli)
{
  unsigned i = 0;
  char *line = cli->line;
  char *arg = cli->arg;
  const char **argv = cli->argv;
  cli->argc = 0;
  *argv = arg;
  while (1) {
    *arg = ((++i == CLI_SIZE) ? 0 : *line);
    if (*arg == 0) {
      if (*argv != arg) {
        argv++;
        cli->argc++;
      }
      *argv = 0;
      return cli->argc;
    }
    else if (*arg == ' ') {
      *arg = 0;
      if (*argv != arg) {
        argv++;
        cli->argc++;
      }
      *argv = arg + 1;
    }
    arg++;
    line++;
  }
}

void trim_newline (char *str)
{
  unsigned int i = 0;
  while (str[i])
    i++;
  if (i > 0 && str[i - 1] == '\n')
    str[i - 1] = 0;
}

int cli_read_file_line (s_cli *cli, FILE *fp)
{
  if (fgets(cli->line, CLI_SIZE, fp)) {
    trim_newline(cli->line);
    fputs(cli->prompt, stdout);
    puts(cli->line);
    return 0;
  }
  return -1;
}

int cli_readline (s_cli *cli)
{
  if (cli->prompt) {
    char *line = readline(cli->prompt);
    if (line == 0)
      return -1;
    strncpy(cli->line, line, CLI_SIZE - 1);
    cli->line[CLI_SIZE - 1] = 0;
    free(line);
    if (*cli->line)
      add_history(cli->line);
    return 0;
  }
  return cli_read_file_line(cli, stdin);
}

int cli_read_file (s_cli *cli, FILE *fp)
{
  cli->argc = -1;
  cli->f = 0;
  if (cli_read_file_line(cli, fp))
    return -1;
  cli_scan(cli);
  if (0 < cli->argc)
    cli->f = cli_find_function(cli, cli->argv[0], cli->argc - 1);
  return cli->argc;
}

int cli_read (s_cli *cli)
{
  cli->argc = -1;
  cli->f = 0;
  if (cli_readline(cli))
    return -1;
  cli_scan(cli);
  if (0 < cli->argc)
    cli->f = cli_find_function(cli, cli->argv[0], cli->argc - 1);
  return cli->argc;
}

int cli_read_nonblocking (s_cli *cli)
{
  fd_set fds;
  struct timeval timeout;
  int s;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  s = select(1, &fds, NULL, NULL, &timeout);
  if (s < 0) {
    warn("cli_read_nonblocking: select");
    return -1;
  }
  if (s == 1)
    return cli_read(cli);
  return -2;
}

f_cli cli_find_function (s_cli *cli, const char *name, int arity)
{
  const s_cli_function *function = cli->functions;
  if (function == 0)
    return 0;
  while (1) {
    if (function->name == 0)
      return 0;
    if (strcmp(function->name, name) == 0 &&
        (function->arity < 0 || arity < 0 || function->arity == arity))
      return function->f;
    function++;
  }
}

int cli_eval (s_cli *cli)
{
  if (0 < cli->argc) {
    if (cli->f)
      return cli->f(cli->argc - 1, cli->argv);
    fprintf(stderr, "rtbuf: function not found: %s\n", cli->argv[0]);
  }
  return -1;
}
