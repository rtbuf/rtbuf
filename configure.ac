AC_PREREQ([2.59])
AC_INIT([rtbuf], [0.1.0], [kmx.io], [rtbuf], [http://kmx.io/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_SRCDIR([rtbuf.c])
AC_CONFIG_HEADERS([config.h:config.h.in])

AM_INIT_AUTOMAKE
AM_PROG_AR

AC_PROG_CC

LT_INIT

AC_CHECK_LIB([cli], [cli_init],,
             AC_MSG_ERROR(missing cli library))
AC_CHECK_LIB([pthread], [pthread_create],,
             AC_MSG_ERROR(missing pthread library))

AC_CHECK_LIB([sndio], [sio_open],,
             AC_MSG_ERROR(missing sndio library))

AC_CHECK_LIB([glfw], [glfwSwapBuffers],,
             AC_MSG_ERROR(missing glfw3 library))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

cat dependencies.mk >> Makefile
