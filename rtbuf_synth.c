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
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_signal_type.h"
#include "rtbuf_music.h"
#include "rtbuf_music_type.h"
#include "rtbuf_synth.h"

s_rtbuf_lib_proc_in g_rtbuf_synth_adsr_in[] = {
  RTBUF_MUSIC_NOTE_IN(),
  { "attack",  RTBUF_SIGNAL_TYPE },
  { "decay",   RTBUF_SIGNAL_TYPE },
  { "sustain", RTBUF_SIGNAL_TYPE },
  { "release", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_proc_out g_rtbuf_synth_adsr_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "state", "int" },
  { 0, 0 } };

s_rtbuf_lib_proc_in g_rtbuf_synth_synth_in[] = {
  { "envelope", RTBUF_SIGNAL_TYPE },
  { "oscillator", RTBUF_SIGNAL_TYPE },
  RTBUF_MUSIC_NOTES_IN("note"),
  { 0, 0 } };

s_rtbuf_lib_proc_out g_rtbuf_synth_synth_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "note_n", "unsigned int" },
  { 0, 0 } };

const char      *rtbuf_lib_name = "synth";
unsigned long    rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_proc rtbuf_lib_proc[] = {
  { "adsr", rtbuf_synth_adsr, rtbuf_synth_adsr_start, 0,
    g_rtbuf_synth_adsr_in, g_rtbuf_synth_adsr_out },
  { "synth", rtbuf_synth_synth, rtbuf_synth_synth_start, 0,
    g_rtbuf_synth_synth_in, g_rtbuf_synth_synth_out },
  { 0, 0, 0, 0, 0, 0 } };

int rtbuf_lib_init (s_rtbuf_lib *lib)
{
  (void) lib;
  rtbuf_music_init();
  return 0;
}
