
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "rtbuf_type.h"
#include "symbol.h"

s_rtbuf_type g_rtbuf_type[RTBUF_TYPE_MAX];
unsigned int g_rtbuf_type_n = 0;

void rtbuf_type_init ()
{
  bzero(g_rtbuf_type, sizeof(g_rtbuf_type));
  rtbuf_type_new("char", sizeof(char));
  rtbuf_type_new("short", sizeof(short));
  rtbuf_type_new("int", sizeof(int));
  rtbuf_type_new("long", sizeof(long));
  rtbuf_type_new("double", sizeof(double));
}

s_rtbuf_type * rtbuf_type_new (const char *name, unsigned int size)
{
  unsigned int i = 0;
  while (i < RTBUF_TYPE_MAX) {
    if (g_rtbuf_type[i].name == 0) {
      g_rtbuf_type[i].name = symbol_intern(name);
      g_rtbuf_type[i].size = size;
      g_rtbuf_type_n++;
      return &g_rtbuf_type[i];
    }
    i++;
  }
  return 0;
}

void rtbuf_type_delete (s_rtbuf_type *rt)
{
  if (rt->name) {
    rt->name = 0;
    rt->size = 0;
    g_rtbuf_type_n--;
  }
}

s_rtbuf_type * rtbuf_type_find (const char *name)
{
  unsigned int i = 0;
  unsigned int n = g_rtbuf_type_n;
  while (i < RTBUF_TYPE_MAX && n > 0) {
    if (g_rtbuf_type[i].name) {
      if (name == g_rtbuf_type[i].name)
        return &g_rtbuf_type[i];
      n--;
    }
    i++;
  }
  return 0;
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
    return rtbuf_type_new(name, element_type->size * size);
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
