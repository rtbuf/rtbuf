
#include <sndio.h>
#include <stdio.h>
#include <strings.h>
#include "rtbuf.h"
#include "rtbuf_lib.h"
#include "rtbuf_signal.h"
#include "rtbuf_sndio.h"

const char     *rtbuf_lib_name = "sndio";
unsigned long   rtbuf_lib_ver = RTBUF_LIB_VER;
s_rtbuf_lib_fun rtbuf_lib_fun[] = {
  { "input", rtbuf_sndio_input, rtbuf_sndio_input_start,
    rtbuf_sndio_input_stop,
    sizeof(s_rtbuf_sndio_input_data) / sizeof(double) + 1,
    sizeof(double), 0 },
  { "output", rtbuf_sndio_output, rtbuf_sndio_output_start,
    rtbuf_sndio_output_stop,
    sizeof(s_rtbuf_sndio_output_data) / sizeof(short) + 1,
    sizeof(short),
    (const char*[]) { "left", "right", 0 } },
  { 0, 0, 0, 0, 0, 0, 0 }
};

void print_sio_par (struct sio_par *par)
{
  printf("#<sio_par bits=%i sig=%i rchan=%i pchan=%i rate=%i>",
         par->bits, par->sig, par->rchan, par->pchan, par->rate);
}
