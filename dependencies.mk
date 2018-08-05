symbol.$(OBJEXT) symbol.lo: symbol.c /usr/include/stdio.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/sys/_null.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/sys/types.h /usr/include/sys/endian.h \
  /usr/include/sys/_endian.h /usr/include/machine/endian.h \
  /usr/include/string.h /usr/include/strings.h symbol.h
rtbuf_type.$(OBJEXT) rtbuf_type.lo: rtbuf_type.c /usr/include/stdio.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/sys/_null.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/sys/types.h /usr/include/sys/endian.h \
  /usr/include/sys/_endian.h /usr/include/machine/endian.h \
  /usr/include/stdlib.h /usr/include/string.h /usr/include/strings.h \
  rtbuf_type.h rtbuf_defs.h symbol.h
rtbuf_fun.$(OBJEXT) rtbuf_fun.lo: rtbuf_fun.c /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/stdio.h \
  /usr/include/sys/_null.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h /usr/include/stdlib.h \
  /usr/include/strings.h rtbuf.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h \
  symbol.h
rtbuf.$(OBJEXT) rtbuf.lo: rtbuf.c /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/stdio.h \
  /usr/include/sys/_null.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h /usr/include/stdlib.h \
  /usr/include/string.h /usr/include/strings.h rtbuf.h rtbuf_defs.h \
  rtbuf_type.h rtbuf_fun.h symbol.h
rtbuf_lib.$(OBJEXT) rtbuf_lib.lo: rtbuf_lib.c /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/dlfcn.h /usr/include/stdio.h \
  /usr/include/sys/_null.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h /usr/include/stdlib.h \
  /usr/include/string.h /usr/include/strings.h rtbuf.h rtbuf_defs.h \
  rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_lib.h
rtbuf_cli.$(OBJEXT) rtbuf_cli.lo: rtbuf_cli.c /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/local/include/cli.h \
  /usr/include/pthread.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/machine/endian.h /usr/include/sys/time.h \
  /usr/include/sys/select.h /usr/include/time.h /usr/include/sys/_null.h \
  /usr/include/sys/_time.h /usr/include/sys/signal.h \
  /usr/include/machine/signal.h /usr/include/sys/siginfo.h \
  /usr/include/limits.h /usr/include/sys/limits.h \
  /usr/include/machine/limits.h /usr/include/sys/syslimits.h \
  /usr/include/sched.h /usr/include/stdlib.h /usr/include/stdio.h \
  /usr/include/unistd.h /usr/include/sys/unistd.h rtbuf.h rtbuf_defs.h \
  rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_lib.h
rtbuf_music.$(OBJEXT) rtbuf_music.lo: rtbuf_music.c rtbuf.h rtbuf_defs.h rtbuf_type.h \
  rtbuf_fun.h symbol.h rtbuf_music.h
rtbuf_signal.$(OBJEXT) rtbuf_signal.lo: rtbuf_signal.c /usr/include/math.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/sys/limits.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/machine/limits.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h rtbuf.h rtbuf_defs.h rtbuf_type.h \
  rtbuf_fun.h symbol.h rtbuf_lib.h rtbuf_music.h rtbuf_signal.h \
  rtbuf_signal_type.h
rtbuf_signal_sinus.$(OBJEXT) rtbuf_signal_sinus.lo: rtbuf_signal_sinus.c /usr/include/math.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/sys/limits.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/machine/limits.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h rtbuf.h rtbuf_defs.h rtbuf_type.h \
  rtbuf_fun.h symbol.h rtbuf_signal.h rtbuf_music.h
rtbuf_signal_square.$(OBJEXT) rtbuf_signal_square.lo: rtbuf_signal_square.c /usr/include/math.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/sys/limits.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/machine/limits.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h rtbuf.h rtbuf_defs.h rtbuf_type.h \
  rtbuf_fun.h symbol.h rtbuf_signal.h rtbuf_music.h
rtbuf_signal_adsr.$(OBJEXT) rtbuf_signal_adsr.lo: rtbuf_signal_adsr.c rtbuf.h rtbuf_defs.h \
  rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_signal.h rtbuf_music.h
rtbuf_signal_synth.$(OBJEXT) rtbuf_signal_synth.lo: rtbuf_signal_synth.c /usr/include/stdio.h \
  /usr/include/sys/cdefs.h /usr/include/machine/cdefs.h \
  /usr/include/sys/_null.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h /usr/include/strings.h rtbuf.h \
  rtbuf_defs.h rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_signal.h \
  rtbuf_music.h
rtbuf_signal_type.$(OBJEXT) rtbuf_signal_type.lo: rtbuf_signal_type.h
rtbuf_sndio.$(OBJEXT) rtbuf_sndio.lo: rtbuf_sndio.c /usr/include/sndio.h \
  /usr/include/sys/types.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/sys/endian.h \
  /usr/include/sys/_endian.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/machine/endian.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/strings.h \
  rtbuf.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_lib.h \
  rtbuf_signal.h rtbuf_music.h rtbuf_signal_type.h rtbuf_sndio.h \
  rtbuf_sndio_type.h
rtbuf_sndio_input.$(OBJEXT) rtbuf_sndio_input.lo: rtbuf_sndio_input.c /usr/include/sndio.h \
  /usr/include/sys/types.h /usr/include/sys/cdefs.h \
  /usr/include/machine/cdefs.h /usr/include/sys/endian.h \
  /usr/include/sys/_endian.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/machine/endian.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/strings.h \
  rtbuf.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_signal.h \
  rtbuf_music.h rtbuf_sndio.h
rtbuf_sndio_output.$(OBJEXT) rtbuf_sndio_output.lo: rtbuf_sndio_output.c /usr/include/limits.h \
  /usr/include/sys/cdefs.h /usr/include/machine/cdefs.h \
  /usr/include/sys/limits.h /usr/include/machine/limits.h \
  /usr/include/sys/syslimits.h /usr/include/sndio.h \
  /usr/include/sys/types.h /usr/include/sys/endian.h \
  /usr/include/sys/_endian.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/machine/endian.h \
  /usr/include/stdio.h /usr/include/sys/_null.h /usr/include/strings.h \
  rtbuf.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h symbol.h rtbuf_sndio.h \
  rtbuf_signal.h rtbuf_music.h
rtbuf_sndio_type.$(OBJEXT) rtbuf_sndio_type.lo: rtbuf_sndio_type.h
rtbuf_glfw3.$(OBJEXT) rtbuf_glfw3.lo: rtbuf_glfw3.c /usr/include/stdio.h \
  /usr/include/sys/cdefs.h /usr/include/machine/cdefs.h \
  /usr/include/sys/_null.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/sys/types.h \
  /usr/include/sys/endian.h /usr/include/sys/_endian.h \
  /usr/include/machine/endian.h /usr/include/string.h \
  /usr/include/strings.h rtbuf.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h \
  symbol.h rtbuf_lib.h rtbuf_signal.h rtbuf_music.h rtbuf_glfw3.h \
  /usr/local/include/GLFW/glfw3.h /usr/include/stddef.h \
  /usr/include/stdint.h /usr/X11R6/include/GL/gl.h \
  /usr/X11R6/include/GL/glext.h /usr/include/inttypes.h \
  /usr/include/sys/stdint.h
rtbuf_music.$(OBJEXT) rtbuf_music.lo: rtbuf_music.c rtbuf.h rtbuf_defs.h rtbuf_type.h \
  rtbuf_fun.h symbol.h rtbuf_music.h
