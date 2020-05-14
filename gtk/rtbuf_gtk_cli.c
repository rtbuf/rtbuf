/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
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
#include <rtbuf/rtbuf.h>
#include <rtbuf/cli.h>
#include "rtbuf_gtk.h"

pthread_t g_rtbuf_gtk_cli_thread = 0;

void * rtbuf_gtk_cli_thread_proc (void *arg)
{
  (void) arg;
  assert(g_rtbuf);
  assert(g_argc);
  assert(g_argv);
  repl();
  return 0;
}

void rtbuf_gtk_cli_start ()
{
  if (pthread_create(&g_rtbuf_gtk_cli_thread, 0,
                     &rtbuf_gtk_cli_thread_proc, 0))
    rtbuf_err("rtbuf_gtk_cli_start: pthread_create failed");
}
