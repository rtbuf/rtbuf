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

#include <stdio.h>
#include "../librtbuf/rtbuf.h"
#include "portaudio.h"

int main ()
{
  printf("/* file generated by rtbuf_portaudio_type */\n");
  printf("#ifndef RTBUF_PORTAUDIO_TYPE_H\n"
         "#define RTBUF_PORTAUDIO_TYPE_H\n"
         "\n");
  printf("#define RTBUF_PORTAUDIO_SAMPLES_TYPE RTBUF_PORTAUDIO_SAMPLE_TYPE"
         " \"[%u]\"\n",
         RTBUF_PORTAUDIO_SAMPLES);
  printf("#define RTBUF_PORTAUDIO_OUTPUT_RESERVED_TYPE"
         " \"char[%u]\"\n",
         (unsigned int) RTBUF_PORTAUDIO_OUTPUT_RESERVED_SIZE);
  printf("\n"
         "#endif\n");
  return 0;
}
