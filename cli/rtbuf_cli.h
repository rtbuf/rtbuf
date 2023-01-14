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
#ifndef RTBUF_CLI_H
#define RTBUF_CLI_H

#include <pthread.h>

typedef void (*f_rtbuf_position) (s_rtbuf *rtb, short x, short y);

extern f_rtbuf_position g_rtbuf_position_cb;
extern pthread_t g_rtbuf_cli_run_thread;

int rtbuf_cli_exit (int argc, const char *argv[]);
void repl_init ();
int load (const char *path);
int repl ();
void rtbuf_cli_args (int argc, char *argv[]);

int rtbuf_cli_start ();
int rtbuf_cli_stop ();

int rtbuf_cli_do_event ();

#endif /* RTBUF_CLI_H */
