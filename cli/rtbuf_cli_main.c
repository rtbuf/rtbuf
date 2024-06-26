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
#include <stdio.h>
#include "../librtbuf/rtbuf.h"
#include "rtbuf_cli.h"

void rtbuf_cli_new_cb (s_rtbuf *rtb);

int main (int argc, char *argv[])
{
  int res = 0;
  symbols_init();
  librtbuf_init();
  g_rtbuf_new_cb = rtbuf_cli_new_cb;
  assert(g_rtbuf);
  repl_init();
  rtbuf_cli_args(argc, argv);
  res = repl();
  rtbuf_cli_stop();
  librtbuf_shutdown();
  return res;
}

void rtbuf_cli_new_cb (s_rtbuf *rtbuf)
{
  rtbuf_print(rtbuf);
}
