#ifndef RTBUF_GLFW3_H
#define RTBUF_GLFW3_H

#include <GLFW/glfw3.h>
#include "rtbuf_defs.h"
#include "rtbuf_music.h"

typedef struct rtbuf_glfw3_keyboard_data {
  s_rtbuf_music_notes notes;
} s_rtbuf_glfw3_keyboard_data;

s_rtbuf_music_notes g_rtbuf_glfw3_keyboard;

int rtbuf_glfw3_keyboard (s_rtbuf *rtb);
int rtbuf_glfw3_keyboard_start (s_rtbuf *rtb);

#endif
