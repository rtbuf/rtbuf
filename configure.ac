AC_PREREQ([2.59])
AC_INIT([rtbuf], [0.3], [kmx.io], [rtbuf], [http://kmx.io/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_SRCDIR([librtbuf/rtbuf.c])
AC_CONFIG_HEADERS([config.h:config.h.in])

AM_INIT_AUTOMAKE
AM_PROG_AR

AC_PROG_CC

LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG([0.25])

if test -d /usr/local/include; then
  CPPFLAGS="$CPPFLAGS -I/usr/local/include"
fi
if test -d /usr/local/lib; then
  LDFLAGS="$LDFLAGS -L/usr/local/lib"
fi

if test -d /usr/include; then
  CPPFLAGS="$CPPFLAGS -I/usr/include"
fi
if test -d /usr/lib; then
  LDFLAGS="$LDFLAGS -L/usr/lib"
fi

CPPFLAGS="-I${PWD}/librtbuf/include -I${PWD}/cli/include -I${PWD}/lib/include $CPPFLAGS"
CFLAGS="$CFLAGS -std=c89 -W -Wall -Werror -D_DEFAULT_SOURCE"

AC_CHECK_FUNC([strlcpy], [echo using system strlcpy],
	      AC_SEARCH_LIBS([strlcpy], [bsd],
               	             [LIBRTBUF_CFLAGS="-I/usr/include/bsd -DLIBBSD_OVERLAY"
                             LIBRTBUF_LIBS="$LIBRTBUF_LIBS -lbsd"]))
AC_SUBST([LIBRTBUF_CFLAGS])
AC_SUBST([LIBRTBUF_LIBS])

AC_CHECK_LIB(m, log2, [AC_DEFINE([HAVE_LOG2],[1],[Define to 1 if your system has 'log2'.])])

AC_ARG_ENABLE([gtk3],
  [AS_HELP_STRING([--disable-gtk3], [do not build GTK+3 GUI])],
  [],
  [enable_gtk3=check])
if test x"$enable_gtk3" != x"no"; then
  PKG_CHECK_MODULES([GTK3], [gtk+-3.0],
                    [enable_gtk3=yes],
                    [enable_gtk3=no])
fi
AM_CONDITIONAL([ENABLE_GTK3], [test x"$enable_gtk3" = x"yes"])
AC_SUBST([GTK3_LIBS])
AC_SUBST([GTK3_CFLAGS])

AC_ARG_ENABLE([rtbuf],
  [AS_HELP_STRING([--disable-rtbuf], [do not build rtbuf CLI])],
  [],
  [enable_rtbuf=yes])
if test x"$enable_rtbuf" = x"yes"; then

  AC_SEARCH_LIBS([pthread_create], [pthread],
                 [RTBUF_LIBS="$RTBUF_LIBS -lpthread"],
                 [AC_MSG_ERROR([missing libpthread for rtbuf])])

  AC_CHECK_FUNC([dlopen], [echo using system dlopen],
                AC_SEARCH_LIBS([dlopen], [dl],
                               [RTBUF_LIBS="$RTBUF_LIBS -ldl"]))

  AC_CHECK_LIB([readline], [readline],
               [RTBUF_LIBS="$RTBUF_LIBS -lreadline"],
               AC_MSG_ERROR(missing readline library for rtbuf))

  AC_CHECK_LIB([termcap], [tputs],
               [RTBUF_LIBS="$RTBUF_LIBS -ltermcap"],
               AC_MSG_ERROR(missing termcap library for rtbuf))

  AC_SUBST([RTBUF_LIBS])
fi
AM_CONDITIONAL([ENABLE_RTBUF], [test x"$enable_rtbuf" = x"true"])

MUSIC_LIBS="-lm"
AC_SUBST([MUSIC_LIBS])

AC_ARG_ENABLE([signal],
  [AS_HELP_STRING([--disable-signal], [do not build rtbuf signal library])],
  [],
  [enable_signal=yes])
if test x"$enable_signal" = x"yes"; then
  SIGNAL_LIBS="-lm"
  AC_SUBST([SIGNAL_LIBS])
fi
AM_CONDITIONAL([ENABLE_SIGNAL], [test x"$enable_signal" = x"yes"])

AC_ARG_ENABLE([sndio],
  [AS_HELP_STRING([--disable-sndio], [do not build rtbuf sndio library])],
  [],
  [enable_sndio=check])
if test x"$enable_sndio" != x"no"; then
  SNDIO_LIBS=
  AC_SEARCH_LIBS([sio_open], [sndio],
                 [enable_sndio=yes
                  SNDIO_LIBS=-lsndio],
                 [enable_sndio=no])
  AC_SUBST([SNDIO_LIBS])
fi
AM_CONDITIONAL([ENABLE_SNDIO], [test x"$enable_sndio" = x"yes"])

AC_ARG_ENABLE([portaudio],
  [AS_HELP_STRING([--enable-portaudio], [build rtbuf portaudio library])],
  [],
  [enable_portaudio=check])
if test x"$enable_portaudio" != x"no"; then
  PORTAUDIO_LIBS=
  AC_SEARCH_LIBS([Pa_Initialize], [portaudio],
                 [enable_portaudio=yes
                  PORTAUDIO_LIBS=-lportaudio],
                 [enable_portaudio=no])
  AC_SUBST([PORTAUDIO_LIBS])
fi
AM_CONDITIONAL([ENABLE_PORTAUDIO], [test x"$enable_portaudio" = x"yes"])

AC_ARG_ENABLE([glfw3],
  [AS_HELP_STRING([--enable-glfw3], [build rtbuf OpenGL library])],
  [],
  [enable_glfw3=check])
if test x"$enable_glfw3" != x"no"; then
  PKG_CHECK_MODULES([GLFW3], [glfw3],
                    [enable_glfw3=yes],
                    [enable_glfw3=no])
  AC_SUBST([GLFW3_CFLAGS])
  AC_SUBST([GLFW3_LIBS])
fi
AM_CONDITIONAL([ENABLE_GLFW3], [test x"$enable_glfw3" = x"yes"])

LIBS=

AC_PREFIX_DEFAULT([~/.rtbuf])

AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
                 [enable debugging, default: yes]),
  [],
  [enable_debug=yes])
if test x"$enable_debug" = x"yes"; then
   CFLAGS="$CFLAGS -DDEBUG -O0 -ggdb"
else
   CFLAGS="$CFLAGS -DNDEBUG -O2"
fi
AM_CONDITIONAL([DEBUG], [test x"$enable_debug" = x"true"])

AC_ARG_ENABLE([nsis],
  [AS_HELP_STRING([--enable-nsis], [build rtbuf NSIS installer])],
  [],
  [enable_nsis=check])
if test x"$enable_nsis" != x"no"; then
  AC_CHECK_PROG([enable_nsis],
                [makensis],
                [enable_nsis=yes],
                [enable_nsis=no])
fi
AM_CONDITIONAL([ENABLE_NSIS], [test x"$enable_nsis" = x"yes"])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([cli/Makefile])
AC_CONFIG_FILES([gtk3/Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([lib/dynamic/Makefile])
AC_CONFIG_FILES([lib/glfw3/Makefile])
AC_CONFIG_FILES([lib/portaudio/Makefile])
AC_CONFIG_FILES([lib/signal/Makefile])
AC_CONFIG_FILES([lib/sndio/Makefile])
AC_CONFIG_FILES([lib/synth/Makefile])
AC_CONFIG_FILES([librtbuf/Makefile])
AC_OUTPUT

echo "Building status :"
echo "  librtbuf      : true $LIBRTBUF_LIBS"
echo "  rtbuf-gtk3    : $enable_gtk3 $GTK3_LIBS"
echo "  rtbuf         : $enable_rtbuf $RTBUF_LIBS"
echo "  lib/signal    : $enable_signal $SIGNAL_LIBS"
echo "  lib/sndio     : $enable_sndio $SNDIO_LIBS"
echo "  lib/portaudio : $enable_portaudio $PORTAUDIO_LIBS"
echo "  lib/glfw3     : $enable_glfw3 $GLFW3_LIBS"
