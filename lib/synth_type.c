
#include <stdio.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/music.h>
#include <rtbuf/music_type.h>

void notes_in ()
{
  unsigned int i = 0;
  const char *st = "RTBUF_SIGNAL_TYPE";
  printf("\n"
         "#define RTBUF_SYNTH_NOTES_IN(note) \\\n");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    printf("  { note \"[%u].envelope\"  , %s }, \\\n", i, st);
    printf("  { note \"[%u].oscillator\", %s }", i, st);
    if (i < RTBUF_MUSIC_NOTE_MAX - 1)
      printf(", \\");
    printf("\n");
    i++;
  }
  i = 0;
  printf("\n"
         "#define RTBUF_SYNTH_NOTES_IN_ENUM(note) \\\n");
  while (i < RTBUF_MUSIC_NOTE_MAX) {
    printf("  note ## _%u_ENVELOPE  , \\\n", i);
    printf("  note ## _%u_OSCILLATOR", i);
    if (i < RTBUF_MUSIC_NOTE_MAX - 1)
      printf(", \\");
    printf("\n");
    i++;
  }
}

int main ()
{
  printf("/* file generated by rtbuf_synth_type */\n");
  printf("#ifndef RTBUF_SYNTH_TYPE_H\n"
         "#define RTBUF_SYNTH_TYPE_H\n"
         "\n"
         "#include <rtbuf/signal_type.h>\n");
  notes_in();
  printf("\n"
         "#endif\n");
  return 0;
}