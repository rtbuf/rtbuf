
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_music.h"
#include "rtbuf_glfw3.h"

const char     *rtbuf_lib_name = "glfw3";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "keyboard", rtbuf_glfw3_keyboard, rtbuf_glfw3_keyboard_start, 0,
    sizeof(s_rtbuf_glfw3_keyboard_data) / sizeof(double) + 1,
    sizeof(double),
    (const char*[]) { 0 } },
  { 0, 0, 0, 0, 0, 0, 0 }
};

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

void keyboard_event (GLFWwindow *w, int key, int scancode, int action,
                     int mods)
{
  int i;
  double freq;
  s_rtbuf_music_note *note;
  (void) w;
  (void) key;
  (void) mods;
  switch (action) {
  case GLFW_RELEASE:
    freq = scancode_frequency(scancode);
    if ((i = find_note(&g_rtbuf_glfw3_keyboard, freq)) < 0)
      break;
    g_rtbuf_glfw3_keyboard.note[i].stop = 0.0;
    break;
  case GLFW_PRESS:
    freq = scancode_frequency(scancode);
    if ((i = rtbuf_music_notes_new(&g_rtbuf_glfw3_keyboard)) < 0)
      break;
    note = &g_rtbuf_glfw3_keyboard.note[i];
    note->freq = freq;
    note->velocity = 1.0;
    note->start = 0.0;
    note->stop = -1.0;
    break;
  case GLFW_REPEAT:
    break;
  }
}

int rtbuf_glfw3_keyboard_start (s_rtbuf *rtb)
{
  (void) rtb;
  bzero(&g_rtbuf_glfw3_keyboard, sizeof(g_rtbuf_glfw3_keyboard));
  return 0;
}

int rtbuf_glfw3_keyboard (s_rtbuf *rtb)
{
  s_rtbuf_glfw3_keyboard_data *data;
  data = (s_rtbuf_glfw3_keyboard_data*) rtb->data;
  memcpy(&data->notes, &g_rtbuf_glfw3_keyboard,
         sizeof(s_rtbuf_music_notes));
  return 0;
}
