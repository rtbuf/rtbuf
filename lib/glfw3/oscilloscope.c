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

#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include <rtbuf/signal.h>
#include <rtbuf/signal_type.h>
#include <rtbuf/music.h>
#include <rtbuf/music_type.h>
#include <rtbuf/glfw3.h>

s_rtbuf_lib_proc_in rtbuf_glfw3_oscilloscope_in[] = {
  { "black", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { "red"  , RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { "green", RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { "blue" , RTBUF_SIGNAL_TYPE, 0.0, -1.0, 1.0 },
  { 0, 0, 0.0, 0.0, 0.0 } };

s_rtbuf_lib_proc_out rtbuf_glfw3_oscilloscope_out[] = {
  { "window", "void*" },
  { 0, 0 } };

int rtbuf_lib_init (s_rtbuf_lib *lib)
{
  (void) lib;
  glfwInit();
  return 0;
}

void rtbuf_glfw3_oscilloscope_size (GLFWwindow *w, int width,
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

void rtbuf_glfw3_oscilloscope_draw (GLFWwindow *window)
{
  s_rtbuf *rtb = (s_rtbuf*) glfwGetWindowUserPointer(window);
  s_rtbuf_signal_fun in[4];
  const float color[4][3] = { { 0.0f, 0.0f, 0.0f },
                              { 1.0f, 0.0f, 0.0f },
                              { 0.0f, 1.0f, 0.0f },
                              { 0.0f, 0.0f, 1.0f } };
  unsigned int i = 0;
  unsigned int j;
  assert(rtb);
  for (j = 0; j < 4; j++)
    rtbuf_signal_fun(rtb, j, &in[j]);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  for (j = 0; j < 4; j++) {
    glColor3f(color[j][0], color[j][1], color[j][2]);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < RTBUF_SIGNAL_SAMPLES; i++) {
      float x = (double) i / RTBUF_SIGNAL_SAMPLES;
      float s = in[j].sample_fun(in[j].signal, i);
      glVertex2f(x, s / 4.0f + 0.5f);
    }
    glEnd();
  }
  glfwSwapBuffers(window);
}

GLFWwindow * rtbuf_glfw3_oscilloscope_window (s_rtbuf *rtb)
{
  GLFWwindow *window = glfwCreateWindow(512, 512,
                                        "rtbuf_glfw3_oscilloscope",
                                        NULL, NULL);
  if (!window) {
    rtbuf_err("glfw3_oscilloscope: glfwCreateWindow failed");
    return 0;
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, rtb);
  glfwSetWindowSizeCallback(window, rtbuf_glfw3_oscilloscope_size);
  glfwSetWindowRefreshCallback(window, rtbuf_glfw3_oscilloscope_draw);
  glfwShowWindow(window);
  return window;
}

int rtbuf_glfw3_oscilloscope_start (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_oscilloscope_data *data;
  assert(rtb->proc->out_bytes == sizeof(*data));
  data = (s_rtbuf_glfw3_oscilloscope_data*) rtb->data;
  if (!data->window &&
      !(data->window = rtbuf_glfw3_oscilloscope_window(rtb)))
    return -1;
  return 0;
}

int rtbuf_glfw3_oscilloscope (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_oscilloscope_data *data;
  assert(rtb);
  assert(rtb->data);
  assert(rtb->proc);
  data = (s_rtbuf_glfw3_oscilloscope_data*) rtb->data;
  rtbuf_glfw3_oscilloscope_draw(data->window);
  glfwPollEvents();
  return 0;
}

s_rtbuf_lib_proc rtbuf_lib_proc =
  { "oscilloscope",
    rtbuf_glfw3_oscilloscope,
    rtbuf_glfw3_oscilloscope_start,
    0,
    rtbuf_glfw3_oscilloscope_in,
    rtbuf_glfw3_oscilloscope_out };

unsigned long rtbuf_lib_ver = RTBUF_LIB_VER;
