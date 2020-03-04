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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_signal.h"
#include "rtbuf_music.h"
#include "rtbuf_music_type.h"
#include "rtbuf_glfw3.h"

#define RTBUF_GLFW3_KEYBOARD_WIDTH 512
#define RTBUF_GLFW3_KEYBOARD_HEIGHT 256

double scancode_frequency (int scancode, unsigned int octave)
{
  int note = -1;
  double freq = 0.0;
  printf("scancode %i\n", scancode);
  switch (scancode) {
  case 52: note =  0; break; /* C */
  case 39: note =  1; break;
  case 53: note =  2; break; /* D */
  case 40: note =  3; break;
  case 54: note =  4; break; /* E */
  case 55: note =  5; break; /* F */
  case 42: note =  6; break;
  case 56: note =  7; break; /* G */
  case 43: note =  8; break;
  case 57: note =  9; break; /* A */
  case 44: note = 10; break;
  case 58: note = 11; break; /* B */
  case 59:                   /* C */
  case 24: note = 12; break;
  case 46:
  case 11: note = 13; break;
  case 60:                   /* D */
  case 25: note = 14; break;
  case 47:
  case 12: note = 15; break;
  case 61:                   /* E */
  case 26: note = 16; break;
  case 27: note = 17; break; /* F */
  case 14: note = 18; break;
  case 28: note = 19; break; /* G */
  case 15: note = 20; break;
  case 29: note = 21; break; /* A */
  case 16: note = 22; break;
  case 30: note = 23; break; /* B */
  case 31: note = 24; break; /* C */
  case 18: note = 25; break;
  case 32: note = 26; break; /* D */
  case 19: note = 27; break;
  case 33: note = 28; break; /* E */
  case 34: note = 29; break; /* F */
  case 21: note = 30; break;
  case 35: note = 31; break; /* G */
  }
  if (note >= 0)
    freq = rtbuf_music_note_frequency(octave, note);
  return freq;
}

static int find_note (s_rtbuf_music_notes *notes, double freq)
{
  unsigned int i = 0;
  unsigned int n = notes->note_n;
  while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
    s_rtbuf_music_note *note = &notes->note[i];
    if (note->velocity > 0.0 && note->start >= 0.0 && note->stop < 0.0) {
      if (freq == notes->note[i].freq)
        return i;
      n--;
    }
    i++;
  }
  return -1;
}

void rtbuf_glfw3_keyboard_key (GLFWwindow *w, int key,
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
    freq = scancode_frequency(scancode, data->octave);
    if ((i = find_note(notes, freq)) >= 0)
      notes->note[i].stop = 0.0;
    break;
  case GLFW_PRESS:
    freq = scancode_frequency(scancode, data->octave);
    if ((i = rtbuf_music_notes_new(notes, 1.0)) < 0)
      break;
    note = &notes->note[i];
    note->freq = freq;
    note->start = 0.0;
    note->stop = -1.0;
    break;
  case GLFW_REPEAT:
    break;
  }
}

void rtbuf_glfw3_keyboard_size (GLFWwindow *w, int width,
                                       int height)
{
  (void) w;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static
void rtbuf_glfw3_keyboard_draw_panel (float y1, float y2)
{
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_QUAD_STRIP);
  glVertex2f(0.0f, y1);
  glVertex2f(0.0f, y2);
  glVertex2f(1.0f, y1);
  glVertex2f(1.0f, y2);
  glEnd();
}

static
void rtbuf_glfw3_keyboard_draw_white (unsigned int i, float n,
                                      float y1, float y2)
{
  float x = (float) i / n;
  glBegin(GL_LINE_STRIP);
  glVertex2f(x, y1);
  glVertex2f(x, y2);
  i++;
  x = (float) i / n;
  glVertex2f(x, y2);
  glVertex2f(x, y1);
  glEnd();
}

static
void rtbuf_glfw3_keyboard_draw_black (unsigned int i, float n,
                                      float y1, float y2)
{
  float x1 = (i - 1.0f / 3.0f) / n;
  float x2 = (i + 1.0f / 3.0f) / n;
  glBegin(GL_QUAD_STRIP);
  glVertex2f(x1, y1);
  glVertex2f(x1, y2);
  glVertex2f(x2, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void rtbuf_glfw3_keyboard_draw (GLFWwindow *w)
{
  float y_buttons = 0.75f;
  float y_black = 0.36f;
  unsigned int notes = 32;
  float n = ceilf((notes + 1) * 7 / 12);
  unsigned int i = 0;
  unsigned int j = 0;
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  rtbuf_glfw3_keyboard_draw_panel(1.0f, y_buttons);
  glColor3f(0.0f, 0.0f, 0.0f);
  while (i < notes) {
    int k = i % 12;
    if (k == 1 || k == 3 || k == 6 || k == 8 || k == 10)
      rtbuf_glfw3_keyboard_draw_black(j, n, y_buttons, y_black);
    else
      rtbuf_glfw3_keyboard_draw_white(j++, n, y_buttons, 0.0f);
    i++;
  }
  glfwSwapBuffers(w);
}

void rtbuf_glfw3_keyboard_close (GLFWwindow *window)
{
  s_rtbuf *rtb = (s_rtbuf*) glfwGetWindowUserPointer(window);
  s_rtbuf_glfw3_keyboard_data* data = (s_rtbuf_glfw3_keyboard_data*)
    rtb->data;
  glfwDestroyWindow(window);
  data->window = 0;
}

GLFWwindow * rtbuf_glfw3_keyboard_window (s_rtbuf *rtb)
{
  GLFWwindow *window = glfwCreateWindow(RTBUF_GLFW3_KEYBOARD_WIDTH,
                                        RTBUF_GLFW3_KEYBOARD_HEIGHT,
                                        "rtbuf_glfw3_keyboard",
                                        NULL, NULL);
  if (!window) {
    rtbuf_err("glfw3_keyboard: glfwCreateWindow failed");
    return 0;
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, rtb);
  glfwSetKeyCallback(window, rtbuf_glfw3_keyboard_key);
  glfwSetWindowCloseCallback(window, rtbuf_glfw3_keyboard_close);
  glfwSetWindowSizeCallback(window, rtbuf_glfw3_keyboard_size);
  glfwSetWindowRefreshCallback(window,
                               rtbuf_glfw3_keyboard_draw);
  glfwShowWindow(window);
  rtbuf_glfw3_keyboard_size(window, RTBUF_GLFW3_KEYBOARD_WIDTH,
                            RTBUF_GLFW3_KEYBOARD_HEIGHT);
  return window;
}

int rtbuf_glfw3_keyboard_start (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  if (!data->window &&
      !(data->window = rtbuf_glfw3_keyboard_window(rtb)))
    return -1;
  data->octave = 3;
  return 0;
}

int rtbuf_glfw3_keyboard_stop (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  rtbuf_music_notes_delete_all(&data->notes);
  return 0;
}

int rtbuf_glfw3_keyboard (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  unsigned int i = 0;
  unsigned int n;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  if (!data->window)
    return 1;
  n = data->notes.note_n;
  while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
    s_rtbuf_music_note *note = &data->notes.note[i];
    if (note->velocity > 0.0) {
      assert(note->start >= 0.0);
      rtbuf_music_note_dt(note, RTBUF_SIGNAL_DT);
      if (note->stop > RTBUF_MUSIC_RELEASE_MAX)
        rtbuf_music_notes_delete(&data->notes, i);
      n--;
    }
    i++;
  }
  glfwPollEvents();
  return 0;
}
