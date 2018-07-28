#ifndef RTBUF_MUSIC_H
#define RTBUF_MUSIC_H

typedef struct rtbuf_music_note {
  double freq;
  double velocity;    /* between 0.0 and 1.0 */
  double start;       /* time since start in seconds */
  double stop;        /* time since stop in seconds, or -1.0 */
} s_rtbuf_music_note;

#define RTBUF_MUSIC_NOTE_MAX 1024

typedef struct rtbuf_music_notes {
  s_rtbuf_music_note note[RTBUF_MUSIC_NOTE_MAX];
  unsigned int note_n;
} s_rtbuf_music_notes;

void rtbuf_music_notes_init (s_rtbuf_music_notes *notes);
int  rtbuf_music_notes_new (s_rtbuf_music_notes *notes);
void rtbuf_music_notes_delete (s_rtbuf_music_notes *notes, unsigned int i);
void rtbuf_music_notes_dt (s_rtbuf_music_notes *notes, double dt);
void rtbuf_music_note_dt (s_rtbuf_music_note *note, double dt);

#endif
