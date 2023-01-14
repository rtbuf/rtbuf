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
#ifndef RTBUF_GLFW3_H
#define RTBUF_GLFW3_H

#include <GLFW/glfw3.h>
#include "../librtbuf/rtbuf.h"
#include "music.h"

typedef struct rtbuf_glfw3_keyboard_data {
  s_rtbuf_music_notes notes;
  GLFWwindow *window;
  unsigned int octave;
} s_rtbuf_glfw3_keyboard_data;

enum {
  RTBUF_GLFW3_OSCILLOSCOPE_IN_BLACK,
  RTBUF_GLFW3_OSCILLOSCOPE_IN_RED,
  RTBUF_GLFW3_OSCILLOSCOPE_IN_GREEN,
  RTBUF_GLFW3_OSCILLOSCOPE_IN_BLUE,
  RTBUF_GLFW3_OSCILLOSCOPE_INS
};

typedef struct rtbuf_glfw3_oscilloscope_data {
  GLFWwindow *window;
} s_rtbuf_glfw3_oscilloscope_data;

#endif
