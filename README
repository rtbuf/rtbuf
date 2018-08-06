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
  variable definitions which are made of a name and a type designator
  output definitions which are made of a name and a type designator

Your library has to export specific symbols :
  rtbuf_lib_name  the name of your library
  rtbuf_lib_ver   an unsigned long used to verify binary compatiblity
  rtbuf_lib_fun   an array of library function definitions

Optional symbols :
  rtbuf_lib_init  a function to initialize the library

When creating a new buffer from a real time function the outputs are
allocated into buffer->data. When writing library functions please make
sure the size of the types in your code matches the size of rtbuf types.

The buffers are ordered according to bindings and real time computation
can start. For each tick all buffers are computed.

There are three libraries for RTBUF under development :
  RTBUF signal - signal processing, operations on buffers of doubles.
  RTBUF sndio  - OpenBSD sndio library for hardware audio i/o.
  RTBUF glfw3  - Portable OpenGL, keyboard and mouse library.

TODO:
  timing of functions
