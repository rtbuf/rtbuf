
#include <stdio.h>
#include "rtbuf.h"
#include "rtbuf_glfw3.h"

int main ()
{
  printf("/* file generated by rtbuf_glfw3_type */\n");
  printf("#ifndef RTBUF_GLFW3_TYPE_H\n"
         "#define RTBUF_GLFW3_TYPE_H\n"
         "\n");
  printf("#define RTBUF_GLFW3_SAMPLES_TYPE RTBUF_GLFW3_SAMPLE_TYPE"
         " \"[%u]\"\n",
         RTBUF_GLFW3_SAMPLES);
  printf("\n"
         "#endif\n");
  return 0;
}
