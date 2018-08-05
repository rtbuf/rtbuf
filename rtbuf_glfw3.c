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
#include <string.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_music.h"
#include "rtbuf_glfw3.h"

s_rtbuf_lib_fun_out g_rtbuf_glfw3_keyboard_out[] = {
  { "notes", RTBUF_MUSIC_NOTES_TYPE },
  { 0, 0 } };

const char     *rtbuf_lib_name = "glfw3";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "keyboard", rtbuf_glfw3_keyboard, rtbuf_glfw3_keyboard_start, 0,
    0, g_rtbuf_glfw3_keyboard_out },
  { 0, 0, 0, 0, 0, 0 } };

s_rtbuf_music_notes g_rtbuf_glfw3_keyboard;

double scancode_frequency (int scancode)
{
  printf("scancode %i\n", scancode);
  switch (scancode) {
  case 0:
    break;
  }
  return 220;
}

static int find_note (s_rtbuf_music_notes *notes, double freq)
{
  unsigned int i = 0;
  unsigned int n = notes->note_n;
  while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
    if (notes->note[i].freq != 0.0) {
      if (freq == notes->note[i].freq)
        return i;
      n--;
    }
    i++;
  }
  return -1;
}

void rtbuf_glfw3_keyboard_window_key (GLFWwindow *w, int key,
                                      int scancode, int action,
                                      int mods)
{
  int i;
  double freq;
  s_rtbuf_music_note *note;
  s_rtbuf *rtb = (s_rtbuf*) glfwGetWindowUserPointer(w);
  s_rtbuf_glfw3_keyboard_data *data;
  s_rtbuf_music_notes *notes;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  notes = &data->notes;
  (void) key;
  (void) mods;
  switch (action) {
  case GLFW_RELEASE:
    freq = scancode_frequency(scancode);
    if ((i = find_note(notes, freq)) >= 0)
      notes->note[i].stop = 0.0;
    break;
  case GLFW_PRESS:
    freq = scancode_frequency(scancode);
    if ((i = rtbuf_music_notes_new(notes)) < 0)
      break;
    note = &notes->note[i];
    note->freq = freq;
    note->velocity = 1.0;
    note->start = 0.0;
    note->stop = -1.0;
    break;
  case GLFW_REPEAT:
    break;
  }
}

void rtbuf_glfw3_keyboard_window_size (GLFWwindow *w, int width,
                                       int height)
{
  printf("size\n");
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void rtbuf_glfw3_keyboard_window_draw (GLFWwindow *w)
{
  float y_buttons = 0.75f;
  float y_black = 0.36f;
  unsigned int octaves = 4;
  unsigned int i = 0;
  unsigned int j = 0;
  printf("draw\n");
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_QUAD_STRIP);
  glVertex2f(0.0f, 1.0f);
  glVertex2f(0.0f, y_buttons);
  glVertex2f(1.0f, 1.0f);
  glVertex2f(1.0f, y_buttons);
  glEnd();
  glColor3f(0.0f, 0.0f, 0.0f);
  while (i < 12 * octaves) {
    int k = i % 12;
    if (k == 1 || k == 3 || k == 6 || k == 8 || k == 10) {
      float x1 = (j - 1.0f / 3.0f) / (octaves * 7);
      float x2 = (j + 1.0f / 3.0f) / (octaves * 7);
      glBegin(GL_QUAD_STRIP);
      glVertex2f(x1, y_buttons);
      glVertex2f(x1, y_black);
      glVertex2f(x2, y_buttons);
      glVertex2f(x2, y_black);
      glEnd();
    }
    else {
      float x = (float) j / (octaves * 7);
      glBegin(GL_LINE_STRIP);
      glVertex2f(x, y_buttons);
      glVertex2f(x,      0.0f);
      j++;
      x = (float) j / (octaves * 7);
      glVertex2f(x,      0.0f);
      glVertex2f(x, y_buttons);
      glEnd();
    }
    i++;
  }
  glfwSwapBuffers(w);
}

GLFWwindow * rtbuf_glfw3_keyboard_window (s_rtbuf *rtb)
{
  GLFWwindow *window = glfwCreateWindow(512, 128,
                                        "rtbuf_glfw3_keyboard",
                                        NULL, NULL);
  if (!window) {
    rtbuf_err("glfwCreateWindow failed");
    return 0;
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, rtb);
  glfwSetKeyCallback(window, rtbuf_glfw3_keyboard_window_key);
  glfwSetWindowSizeCallback(window, rtbuf_glfw3_keyboard_window_size);
  glfwSetWindowRefreshCallback(window,
                               rtbuf_glfw3_keyboard_window_draw);
  glfwShowWindow(window);
  printf("created glfw3 keyboard window\n");
  return window;
}

int rtbuf_glfw3_keyboard_start (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  if (!data->window &&
      !(data->window = rtbuf_glfw3_keyboard_window(rtb)))
    return -1;
  
  return 0;
}

int rtbuf_glfw3_keyboard (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  glfwPollEvents();
  return 0;
}

int rtbuf_lib_init (s_rtbuf_lib *lib)
{
  (void) lib;
  glfwInit();
}
