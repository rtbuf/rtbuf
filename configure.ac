AC_PREREQ([2.59])
AC_INIT([rtbuf], [0.1.0], [kmx.io], [rtbuf], [http://kmx.io/])
CFLAGS="$CFLAGS -W -Wall -Werror"
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_SRCDIR([rtbuf.c])
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

enable_rtbuf=true
AM_CONDITIONAL([ENABLE_RTBUF], [test x"$enable_rtbuf" = x"true"])
AC_SEARCH_LIBS([cli_init], [cli],
               [RTBUF_LIBS="$RTBUF_LIBS -lcli"],
               [if test x"$enable_rtbuf" = x"true"; then
                  AC_MSG_ERROR([missing libcli for rtbuf"])
                fi])
AC_SEARCH_LIBS([pthread_create], [pthread],
               [RTBUF_LIBS="$RTBUF_LIBS -lpthread"],
               [if test x"$enable_rtbuf" = x"true"; then
                  AC_MSG_ERROR([missing libpthread for rtbuf"])
                fi])
AC_SUBST([RTBUF_LIBS])

MUSIC_LIBS="-lm"
AC_SUBST([MUSIC_LIBS])

enable_signal=true
AC_ARG_ENABLE(signal,
  AS_HELP_STRING([--enable-signal],
                 [enable building librtbuf_signal, default: yes]),
  [case "${enableval}" in
     no)  enable_signal=false ;;
   esac])
SIGNAL_LIBS="-lm"
AC_SUBST([SIGNAL_LIBS])
AM_CONDITIONAL([ENABLE_SIGNAL], [test x"$enable_signal" = x"true"])

enable_sndio=false
SNDIO_LIBS=
AC_SEARCH_LIBS([sio_open], [sndio],
               [enable_sndio=true
                SNDIO_LIBS=-lsndio])
AC_SUBST([SNDIO_LIBS])
AM_CONDITIONAL([ENABLE_SNDIO], [test x"$enable_sndio" = x"true"])

PKG_CHECK_MODULES([GLFW3], [glfw3],
                  [enable_glfw3=true],
                  [enable_glfw3=false])
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
AM_CONDITIONAL([DEBUG], [test x"$debug" = x"true"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

cat dependencies.mk >> Makefile

echo "Building status :"
echo "  rtbuf  : $enable_rtbuf $RTBUF_LIBS"
echo "  signal : $enable_signal $SIGNAL_LIBS"
echo "  sndio  : $enable_sndio $SNDIO_LIBS"
echo "  glfw   : $enable_glfw $GLFW_LIBS"
