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
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_music.h"
#include "rtbuf_signal.h"
#include "rtbuf_signal_type.h"

s_rtbuf_lib_fun_var g_rtbuf_signal_sinus_var[] = {
  { "frequency", RTBUF_SIGNAL_TYPE },
  { "amplitude", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_sinus_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

s_rtbuf_lib_fun_var g_rtbuf_signal_square_var[] = {
  { "frequency", RTBUF_SIGNAL_TYPE },
  { "amplitude", RTBUF_SIGNAL_TYPE },
  { "pulse",     RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_square_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "phase", "double" },
  { 0, 0 } };

s_rtbuf_lib_fun_var g_rtbuf_signal_adsr_var[] = {
  { "attack",  RTBUF_SIGNAL_TYPE },
  { "decay",   RTBUF_SIGNAL_TYPE },
  { "sustain", RTBUF_SIGNAL_TYPE },
  { "release", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_adsr_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "start", "double" },
  { "stop", "double" },
  { 0, 0 } };

s_rtbuf_lib_fun_var g_rtbuf_signal_synth_var[] = {
  { "keyboard", RTBUF_MUSIC_NOTES_TYPE },
  { "envelope", RTBUF_SIGNAL_TYPE },
  { "oscillator", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_signal_synth_out[] = {
  { "signal", RTBUF_SIGNAL_TYPE },
  { "notes", RTBUF_SIGNAL_SYNTH_NOTES_TYPE },
  { 0, 0 } };

const char     *rtbuf_lib_name = "signal";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "sinus", rtbuf_signal_sinus, rtbuf_signal_sinus_start, 0,
    g_rtbuf_signal_sinus_var, g_rtbuf_signal_sinus_out },
  { "square", rtbuf_signal_square, rtbuf_signal_square_start, 0,
    g_rtbuf_signal_square_var, g_rtbuf_signal_square_out },
  { "adsr", rtbuf_signal_adsr, rtbuf_signal_adsr_start, 0,
    g_rtbuf_signal_adsr_var, g_rtbuf_signal_adsr_out },
  { "synth", rtbuf_signal_synth, rtbuf_signal_synth_start, 0,
    g_rtbuf_signal_synth_var, g_rtbuf_signal_synth_out },
  { 0, 0, 0, 0, 0, 0 } };

int rtbuf_lib_init (s_rtbuf_lib *lib)
{
  (void) lib;
  rtbuf_music_init();
  rtbuf_type_new(RTBUF_SIGNAL_SYNTH_NOTE_TYPE,
                 RTBUF_SIGNAL_SYNTH_NOTE_SIZE);
  rtbuf_type_new(RTBUF_SIGNAL_SYNTH_NOTES_TYPE,
                 RTBUF_SIGNAL_SYNTH_NOTES_SIZE);
  return 0;
}
