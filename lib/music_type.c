/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com>
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

#include <stdio.h>
#include "../librtbuf/rtbuf.h"
#include "music.h"

void notes_in ()
{
  unsigned int i = 0;
  const char *st = "RTBUF_SIGNAL_SAMPLE_TYPE";
  printf("\n"
         "#define RTBUF_MUSIC_NOTES_IN(note) \\\n"
         "  { note \"s\"            , RTBUF_MUSIC_NOTES_TYPE, 0.0, 0.0, 0.0, 1.0 }"
         ", \\\n");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    printf("  { note \"[%u].velocity\" , %s, 0.0, 0.0, 1.0, 1.0 }, \\\n", i, st);
    printf("  { note \"[%u].frequency\", %s, 0.0, 0.0, "
           "RTBUF_SIGNAL_SAMPLERATE / 2.0, 1.0 }, \\\n", i, st);
    printf("  { note \"[%u].start\"    , %s, -1.0, -1.0, DBL_MAX, 1.0 }, \\\n", i, st);
    printf("  { note \"[%u].stop\"     , %s, -1.0, -1.0, DBL_MAX, 1.0 }", i, st);
    if (i < RTBUF_MUSIC_NOTE_MAX - 1)
      printf(", \\");
    printf("\n");
    i++;
  }
  i = 0;
  printf("\n"
         "#define RTBUF_MUSIC_NOTES_IN_ENUM(note) \\\n"
         "  note ## S           , \\\n");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    printf("  note ## _%u_VELOCITY , \\\n", i);
    printf("  note ## _%u_FREQUENCY, \\\n", i);
    printf("  note ## _%u_START    , \\\n", i);
    printf("  note ## _%u_STOP", i);
    if (i < RTBUF_MUSIC_NOTE_MAX - 1)
      printf("     , \\");
    printf("\n");
    i++;
  }
}

void notes_out ()
{
  unsigned int i = 0;
  const char *st = "RTBUF_SIGNAL_SAMPLE_TYPE";
  printf("\n"
         "#define RTBUF_MUSIC_NOTES_OUT(note) \\\n"
         "  { note \"s\"            , RTBUF_MUSIC_NOTES_TYPE   }"
         ", \\\n");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    printf("  { note \"[%u].velocity\" , %s }, \\\n", i, st);
    printf("  { note \"[%u].frequency\", %s }, \\\n", i, st);
    printf("  { note \"[%u].start\"    , %s }, \\\n", i, st);
    printf("  { note \"[%u].stop\"     , %s }", i, st);
    if (i < RTBUF_MUSIC_NOTE_MAX - 1)
      printf(", \\");
    printf("\n");
    i++;
  }
}

int main ()
{
  printf("/* file generated by rtbuf_music_type */\n");
  printf("#ifndef RTBUF_MUSIC_TYPE_H\n"
         "#define RTBUF_MUSIC_TYPE_H\n");
  notes_in();
  notes_out();
  printf("\n"
         "#endif\n");
  return 0;
}
