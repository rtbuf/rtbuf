
#include "rtbuf.h"
#include "rtbuf_music.h"

int rtbuf_music_notes_new (s_rtbuf_music_notes *notes)
{
  unsigned int i = 0;
  if (!notes)
    return -1;
  if (notes->note_n >= RTBUF_MUSIC_NOTE_MAX)
    return -1;
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    if (notes->note[i].freq == 0.0) {
      notes->note[i].freq = 0.1;
      notes->note_n++;
      return i;
    }
    i++;
  }
  return -1;
}

void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes,
                               unsigned int i)
{
  if (notes && i < RTBUF_MUSIC_NOTE_MAX &&
      notes->note[i].freq != 0.0) {
    notes->note[i].freq = 0.0;
    notes->note_n--;
  }
}

void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt)
{
  if (note) {
    note->start += dt;
    if (note->stop >= 0)
      note->stop += dt;
  }
}

void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt)
{
  unsigned int i = 0;
  if (notes) {
    unsigned int n = notes->note_n;
    while (i < RTBUF_MUSIC_NOTE_MAX && n > 0) {
      if (notes->note[i].freq != 0.0) {
        rtbuf_music_note_dt(&notes->note[i], dt);
        n--;
      }
      i++;
    }
  }
}
