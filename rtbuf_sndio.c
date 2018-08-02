
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_sndio.h"

s_rtbuf_lib_fun_out g_rtbuf_sndio_input_out[] = {
  { "left", RTBUF_SIGNAL_TYPE },
  { "right", RTBUF_SIGNAL_TYPE },
  { "samples", RTBUF_SNDIO_SAMPLES_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_var g_rtbuf_sndio_output_var[] = {
  { "left", RTBUF_SIGNAL_TYPE },
  { "right", RTBUF_SIGNAL_TYPE },
  { 0, 0 } };

s_rtbuf_lib_fun_out g_rtbuf_sndio_output_out[] = {
  { "samples", RTBUF_SNDIO_SAMPLES_TYPE },
  { "reserved", RTBUF_SNDIO_OUTPUT_RESERVED_TYPE },
  { 0, 0 } };

const char     *rtbuf_lib_name = "sndio";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "input", rtbuf_sndio_input, rtbuf_sndio_input_start,
    rtbuf_sndio_input_stop, 0, g_rtbuf_sndio_input_out },
  { "output", rtbuf_sndio_output, rtbuf_sndio_output_start,
    rtbuf_sndio_output_stop, g_rtbuf_sndio_output_var,
    g_rtbuf_sndio_output_out },
  { 0, 0, 0, 0, 0, 0 } };

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}
