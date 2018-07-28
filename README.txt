RTBUF - real time buffers

Minimalist framework for real time signal processing in C.

RTBUF is a modular system : you write real time functions in C
in a library. You can then load these
libraries and instanciate real time functions into buffers which
are bound to one another. Almost all memory allocation is static.

A real time function is made of :
  a name (a string identifier)
  an optional start method that will be called before computation starts
  an optional compute method that will be called in real time
  an optional stop method that will be called after computation finishes
  size and count of buffer members for memory allocation
  variable names that can be bound to other buffers

Your library has to export specific symbols :
  rtbuf_lib_name  the name of your library
  rtbuf_lib_ver   an unsigned long used to verify binary compatiblity
  rtbuf_lib_fun   an array of library function definitions

The buffers are ordered according to bindings and real time computation
can start. For each tick all buffers are computed.

There are three libraries for RTBUF under development :
  RTBUF signal - signal processing, operations on buffers of doubles.
  RTBUF sndio  - OpenBSD sndio library for hardware audio i/o.
  RTBUF glfw3  - Portable OpenGL, keyboard and mouse library.

TODO:
  timing of functions
