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
#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include "form.h"

#define CLI_SIZE 1000

typedef struct cli s_cli;
typedef struct cli_function s_cli_function;
typedef int (* f_cli) (int argc, const char *argv[]);

struct cli
{
  const char *prompt;
  const s_cli_function *functions;
  char line[CLI_SIZE];
  char arg[CLI_SIZE];
  int argc;
  const char *argv[500];
  f_cli f;
};

void cli_init (s_cli *cli);
void cli_prompt (s_cli *cli, const char *prompt);

struct cli_function {
  const char *name;
  int arity;           /* negative arity means variable arity */
  f_cli f;
};

void cli_functions (s_cli *cli, const s_cli_function *functions);
f_cli cli_find_function (s_cli *cli, const char *name, int arity);

int cli_scan (s_cli *cli);
int cli_read (s_cli *cli);
int cli_read_file (s_cli *cli, FILE *fp);
int cli_eval (s_cli *cli);

#endif /* CLI_H */
