AC_PREREQ([2.59])
AC_INIT([rtbuf], [0.2.3], [kmx.io], [rtbuf], [http://kmx.io/])
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

CPPFLAGS="-I${PWD}/librtbuf/include -I${PWD}/lib/include $CPPFLAGS"
CFLAGS="-std=c89 -W -Wall -Werror"

AC_CHECK_FUNC([strlcpy], [echo using system strlcpy],
	      AC_SEARCH_LIBS([strlcpy], [bsd],
               	             [LIBRTBUF_CFLAGS="-I/usr/include/bsd -DLIBBSD_OVERLAY"
                             LIBRTBUF_LIBS="$LIBRTBUF_LIBS -lbsd"]))
AC_SUBST([LIBRTBUF_CFLAGS])
AC_SUBST([LIBRTBUF_LIBS])

AC_CHECK_LIB(m, log2, [AC_DEFINE([HAVE_LOG2],[1],[Define to 1 if your system has 'log2'.])])

PKG_CHECK_MODULES([GTK3], [gtk+-3.0],
                  [enable_gtk3=true],
                  [enable_gtk3=false])
AM_CONDITIONAL([ENABLE_GTK3], [test x"$enable_gtk3" = x"true"])
AC_SUBST([GTK3_LIBS])
AC_SUBST([GTK3_CFLAGS])

enable_rtbuf=true
AM_CONDITIONAL([ENABLE_RTBUF], [test x"$enable_rtbuf" = x"true"])

AC_SEARCH_LIBS([pthread_create], [pthread],
               [RTBUF_LIBS="$RTBUF_LIBS -lpthread"],
               [if test x"$enable_rtbuf" = x"true"; then
                  AC_MSG_ERROR([missing libpthread for rtbuf])
                fi])

AC_CHECK_FUNC([dlopen], [echo using system dlopen],
              AC_SEARCH_LIBS([dlopen], [dl],
                             [RTBUF_LIBS="$RTBUF_LIBS -ldl"]))

AC_CHECK_LIB([readline], [readline],
             [RTBUF_LIBS="$RTBUF_LIBS -lreadline"],
             AC_MSG_ERROR(missing readline library for rtbuf))

AC_CHECK_LIB([termcap], [tputs],
             [RTBUF_LIBS="$RTBUF_LIBS -ltermcap"],
             AC_MSG_ERROR(missing termcap library for rtbuf))

AM_CONDITIONAL([ENABLE_RTBUF], [test x"$enable_rtbuf" = x"true"])
AC_SUBST([RTBUF_LIBS])

MUSIC_LIBS="-lm"
AC_SUBST([MUSIC_LIBS])

SIGNAL_LIBS="-lm"
AC_SUBST([SIGNAL_LIBS])

SNDIO_LIBS=
AC_SEARCH_LIBS([sio_open], [sndio],
               [enable_sndio=true
                SNDIO_LIBS=-lsndio])
AC_SUBST([SNDIO_LIBS])
AM_CONDITIONAL([ENABLE_SNDIO], [test x"$enable_sndio" = x"true"])

enable_portaudio=false
PORTAUDIO_LIBS=
AC_SEARCH_LIBS([Pa_Initialize], [portaudio],
               [enable_portaudio=true
                PORTAUDIO_LIBS=-lportaudio])
AC_SUBST([PORTAUDIO_LIBS])
AM_CONDITIONAL([ENABLE_PORTAUDIO], [test x"$enable_portaudio" = x"true"])

PKG_CHECK_MODULES([GLFW3], [glfw3],
                  [enable_glfw3=true],
                  [enable_glfw3=false])
AC_SUBST([GLFW3_CFLAGS])
AC_SUBST([GLFW3_LIBS])
AM_CONDITIONAL([ENABLE_GLFW3], [test x"$enable_glfw3" = x"true"])

LIBS=

AC_PREFIX_DEFAULT([~/.rtbuf])

AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
                 [enable debugging, default: yes]),
  [case "${enableval}" in
     yes) debug=true ;;
      no) debug=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]);;
   esac],
  [debug=true])
if test x"$debug" = x"true"; then
   CFLAGS="$CFLAGS -DDEBUG -O0 -ggdb"
else
   CFLAGS="$CFLAGS -DNDEBUG -O2"
fi
AM_CONDITIONAL([DEBUG], [test x"$debug" = x"true"])

AC_CHECK_PROG([enable_nsis], [makensis], [true], [false])
AM_CONDITIONAL([ENABLE_NSIS], [test x"$enable_nsis" = x"true"])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([cli/Makefile])
AC_CONFIG_FILES([gtk/Makefile])
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
echo "  librtbuf  : true $LIBRTBUF_LIBS"
echo "  rtbuf-gtk : $enable_gtk3 $GTK3_LIBS"
echo "  rtbuf     : $enable_rtbuf $RTBUF_LIBS"
echo "  signal    : $enable_signal $SIGNAL_LIBS"
echo "  sndio     : $enable_sndio $SNDIO_LIBS"
echo "  portaudio : $enable_portaudio $PORTAUDIO_LIBS"
echo "  glfw3     : $enable_glfw3 $GLFW3_LIBS"
