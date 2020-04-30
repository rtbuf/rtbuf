/*
 * Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> +33614550127
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef DATA_H
#define DATA_H

typedef enum {
  DATA_TYPE_BITS,
  DATA_TYPE_VECTOR,
  DATA_TYPE_INTEGER,
  DATA_TYPE_FLOAT,
  DATA_TYPE_STRUCT
} e_data_type;

#define DATA_TYPE_SIGNED     1
#define DATA_TYPE_BIG_ENDIAN 2

typedef struct data_type {
  unsigned int bits;
  unsigned int type;
} s_data_type;

typedef struct data_type_vector {
  s_data_type  t;
  s_data_type *element_type;
  unsigned int nmemb;
} s_data_type_vector;

typedef struct data_type_integer {
  s_data_type  t;
  unsigned int flags;
} s_data_type_integer;

typedef struct data_type_float {
  s_data_type t;
} s_data_type_float;

typedef struct data_type_struct {
  s_data_type  t;
  s_data_type *first;
  struct data_type_struct *rest;
} s_data_type_struct;

typedef union data_type_ {
  s_data_type         t;
  s_data_type_vector  t_vector;
  s_data_type_integer t_integer;
  s_data_type_float   t_float;
  s_data_type_struct  t_struct;
} u_data_type;

#define DATA_TYPE_MAX 1024

u_data_type * data_type_new (unsigned int bits, unsigned int type);
void          data_type_delete (s_data_type *t);

typedef void f_data_init (void *data);
typedef void f_data_clean (void *data);

typedef struct data_alloc {
  s_data_type  *t;
  unsigned int  max;
  void         *mem;
  unsigned int  n;
  unsigned int *free;
  unsigned int  free_n;
  f_data_init  *init;
  f_data_clean *clean;
} s_data_alloc;

#define DATA_ALLOC_MAX 1024

void * data_new (s_data_alloc *da);
int    data_new_i (s_data_alloc *da);
void   data_delete (s_data_alloc *da, void *data);

void           data_alloc_init (s_data_alloc *da, s_data_type *t,
                                unsigned int max, f_data_init *init,
                                f_data_clean *clean);
void           data_alloc_clean (s_data_alloc *da);
s_data_alloc * data_alloc_new (s_data_type *t,
                               unsigned int max, f_data_init *init,
                               f_data_clean *clean);
void           data_alloc_delete (s_data_alloc *da);

int libdata_init ();

#endif
