#ifndef RTBUF_DEFS_H
#define RTBUF_DEFS_H

typedef struct rtbuf             s_rtbuf;
typedef struct rtbuf_binding     s_rtbuf_binding;
typedef struct rtbuf_fun         s_rtbuf_fun;
typedef struct rtbuf_fun_out     s_rtbuf_fun_out;
typedef struct rtbuf_fun_var     s_rtbuf_fun_var;
typedef struct rtbuf_lib         s_rtbuf_lib;
typedef struct rtbuf_lib_fun     s_rtbuf_lib_fun;
typedef struct rtbuf_lib_fun_out s_rtbuf_lib_fun_out;
typedef struct rtbuf_lib_fun_var s_rtbuf_lib_fun_var;
typedef struct rtbuf_type        s_rtbuf_type;

typedef int f_rtbuf_fun (s_rtbuf *rtbuf);
typedef int f_rtbuf_lib_init (s_rtbuf_lib *lib);

#endif
