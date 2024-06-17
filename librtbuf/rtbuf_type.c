/* rtbuf
 * Copyright 2018-2023 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software excepted
 * on Apple computers granted the above copyright notice and
 * this permission paragraph are included in all copies and
 * substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "type.h"
#include "symbol.h"

s_data_type   g_rtbuf_data_type_type = {
  sizeof(s_rtbuf_type),
  DATA_TYPE_BYTES
};
s_data_alloc  g_rtbuf_data_type_alloc;
s_rtbuf_type *g_rtbuf_data_type;

void rtbuf_type_init (void)
{
  data_alloc_init(&g_rtbuf_data_type_alloc, &g_rtbuf_data_type_type,
                  RTBUF_TYPE_MAX, 0, 0);
  g_rtbuf_data_type = g_rtbuf_data_type_alloc.mem;
  assert(g_rtbuf_data_type);
  rtbuf_type_define("char"          , sizeof(char));
  rtbuf_type_define("unsigned char" , sizeof(unsigned char));
  rtbuf_type_define("short"         , sizeof(short));
  rtbuf_type_define("unsigned short", sizeof(unsigned short));
  rtbuf_type_define("int"           , sizeof(int));
  rtbuf_type_define("unsigned int"  , sizeof(unsigned int));
  rtbuf_type_define("long"          , sizeof(long));
  rtbuf_type_define("unsigned long" , sizeof(unsigned long));
  rtbuf_type_define("float"         , sizeof(float));
  rtbuf_type_define("double"        , sizeof(double));
}

void rtbuf_type_shutdown (void)
{
  if (g_rtbuf_data_type) {
    rtbuf_type_delete_all();
    g_rtbuf_data_type = 0;
    data_alloc_clean(&g_rtbuf_data_type_alloc);
  }
}

s_rtbuf_type * rtbuf_type_new (const char *name, unsigned int size)
{
  s_rtbuf_type *rt = data_new(&g_rtbuf_data_type_alloc);
  if (!rt)
    return 0;
  rt->name = symbol_intern(name);
  rt->t.bytes = size;
  rt->t.type = DATA_TYPE_BYTES;
  return rt;
}

void rtbuf_type_delete (s_rtbuf_type *rt)
{
  assert(rt);
  data_delete(&g_rtbuf_data_type_alloc, rt);
}

void rtbuf_type_delete_all (void)
{
  unsigned i = 0;
  int n = g_rtbuf_data_type_alloc.n;
  while (i < g_rtbuf_data_type_alloc.max && n > 0) {
    if (g_rtbuf_data_type[i].name) {
      rtbuf_type_delete(g_rtbuf_data_type + i);
      n--;
    }
    i++;
  }
}

s_rtbuf_type * rtbuf_type_find (symbol name)
{
  unsigned int i = 0;
  unsigned int n =
    g_rtbuf_data_type_alloc.n - g_rtbuf_data_type_alloc.free_n;
  while (i < g_rtbuf_data_type_alloc.n && n > 0) {
    if (g_rtbuf_data_type[i].name) {
      if (name == g_rtbuf_data_type[i].name)
        return &g_rtbuf_data_type[i];
      n--;
    }
    i++;
  }
  return 0;
}

s_rtbuf_type * rtbuf_type_define (const char *name, unsigned int size)
{
  s_rtbuf_type *found = rtbuf_type_find(name);
  if (found && found->t.bytes == size)
    return found;
  return rtbuf_type_new(name, size);
}

s_rtbuf_type * rtbuf_type_parse_array (const char *name)
{
  char *bracket = strchr(name, '[');
  char *right_bracket = bracket ? strchr(bracket, ']') : 0;
  if (right_bracket) {
    char buf[1024];
    unsigned int lb = bracket - name;
    unsigned int rb = right_bracket - name;
    s_rtbuf_type *element_type;
    unsigned int size;
    strlcpy(buf, name, sizeof(buf));
    buf[lb] = 0;
    if ((element_type = rtbuf_type(buf)) == 0)
      return 0;
    buf[rb] = 0;
    size = atoi(&buf[lb + 1]);
    return rtbuf_type_new(name, element_type->t.bytes * size);
  }
  return 0;
}

s_rtbuf_type * rtbuf_type_parse_pointer (const char *name)
{
  char *star = strchr(name, '*');
  if (star && star[1] == 0)
    return rtbuf_type_new(name, sizeof(void*));
  return 0;
}

s_rtbuf_type * rtbuf_type (const char *name)
{
  const char *sym = symbol_find(name);
  s_rtbuf_type *rt;
  if (!(rt = sym ? rtbuf_type_find(sym) : 0) &&
      !(rt = rtbuf_type_parse_array(name)) &&
      !(rt = rtbuf_type_parse_pointer(name)))
    printf("type not found: %s\n", name);
  return rt;
}

s_rtbuf_type * rtbuf_type_array (const char *element_type,
                                 unsigned int size)
{
  char buf[1024];
  snprintf(buf, sizeof(buf), "%s[%u]", element_type, size);
  return rtbuf_type(buf);
}
