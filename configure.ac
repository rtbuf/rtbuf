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

if test -d /usr/local/include; then
  CPPFLAGS="$CPPFLAGS -I/usr/local/include"
fi
if test -d /usr/local/lib; then
  LDFLAGS="$LDFLAGS -L/usr/local/lib"
fi

enable_rtbuf=true
AC_CHECK_LIB([cli], [cli_init],
             [RTBUF_LIBS="$RTBUF_LIBS -lcli"],
             [if test x"$enable_rtbuf" = x"true"; then
                AC_MSG_ERROR([missing libcli for rtbuf"])
              fi])
AC_CHECK_LIB([pthread], [pthread_create],
             [RTBUF_LIBS="$RTBUF_LIBS -lpthread"],
             [if test x"$enable_rtbuf" = x"true"; then
                AC_MSG_ERROR([missing libpthread for rtbuf"])
              fi])
AC_SUBST([RTBUF_LIBS])
AM_CONDITIONAL([ENABLE_RTBUF], [test x"$enable_rtbuf" = x"true"])

enable_signal=true
SIGNAL_LIBS=
AC_CHECK_LIB([m], [sin], [SIGNAL_LIBS="$SIGNAL_LIBS -lm"])
AC_ARG_ENABLE(signal,
  AS_HELP_STRING([--enable-signal],
                 [enable building librtbuf_signal, default: yes]),
  [case "${enableval}" in
     no)  enable_signal=false ;;
   esac])
AC_SUBST([SIGNAL_LIBS])
AM_CONDITIONAL([ENABLE_SIGNAL], [test x"$enable_signal" = x"true"])

enable_sndio=false
SNDIO_LIBS=
AC_CHECK_LIB([sndio], [sio_open],
             [enable_sndio=true
              SNDIO_LIBS=-lsndio])
AC_SUBST([SNDIO_LIBS])
AM_CONDITIONAL([ENABLE_SNDIO], [test x"$enable_sndio" = x"true"])

if test -d /usr/X11R6/include; then
  CPPFLAGS="$CPPFLAGS -I/usr/X11R6/include"
fi
if test -d /usr/X11R6/lib; then
  LDFLAGS="$LDFLAGS -L/usr/X11R6/lib"
fi

enable_glfw=false
GLFW_LIBS=
AC_CHECK_LIB([glfw], [glfwSwapBuffers],
             [enable_glfw=true
              GLFW_LIBS=-lglfw])
AC_SUBST([GLFW_LIBS])
AM_CONDITIONAL([ENABLE_GLFW], [test x"$enable_glfw" = x"true"])

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
echo "  rtbuf           $enable_rtbuf"
echo "  signal          $enable_signal"
echo "  sndio           $enable_sndio"
echo "  glfw            $enable_glfw"
