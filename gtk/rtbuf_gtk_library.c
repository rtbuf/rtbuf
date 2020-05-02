/*
 * Copyright 2020 Thomas de Grivel <thoxdg@gmail.com>
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

#include <assert.h>
#include <stdio.h>
#include <gio/gio.h>
#include <rtbuf/rtbuf.h>
#include <rtbuf/lib.h>
#include "rtbuf_gtk_library.h"
#include "queue.h"

s_rtbuf_gtk_library_tree *g_rtbuf_gtk_library_tree = NULL;

s_rtbuf_gtk_library_tree * rtbuf_gtk_library_tree_new
(const char *dir_name, const char *leaf_name,
 s_rtbuf_gtk_library_tree *children,
 s_rtbuf_gtk_library_tree *next)
{
  s_rtbuf_gtk_library_tree *tree =
    malloc(sizeof(s_rtbuf_gtk_library_tree));
  if (tree) {
    tree->dir_name = dir_name;
    tree->leaf_name = leaf_name;
    tree->children = children;
    tree->next = next;
  }
  return tree;
}

void rtbuf_gtk_library_tree_delete (s_rtbuf_gtk_library_tree *tree)
{
  s_queue q;
  assert(tree);
  queue_init(&q);
  queue_enqueue(&q, tree);
  while (q.length > 0) {
    tree = queue_dequeue(&q);
    if (tree->children)
      queue_enqueue(&q, tree->children);
    if (tree->next)
      queue_enqueue(&q, tree->next);
    free((void*) tree->dir_name);
    tree->dir_name = 0;
    free((void*) tree->leaf_name);
    tree->leaf_name = 0;
    free(tree);
  }
  queue_destroy(&q);
}

s_rtbuf_gtk_library_tree *
rtbuf_gtk_library_tree_find_dir_name
(s_rtbuf_gtk_library_tree *tree,
 const char *dir_name,
 size_t len)
{
  while (tree) {
    if (strncmp(tree->dir_name, dir_name, len) == 0)
      return tree;
    tree = tree->next;
  }
  return NULL;
}

s_rtbuf_gtk_library_tree *
rtbuf_gtk_library_tree_insert (s_rtbuf_gtk_library_tree **tree,
                               const char *dir_name,
                               const char *leaf_name)
{
  s_rtbuf_gtk_library_tree *t;
  assert(tree);
  t = rtbuf_gtk_library_tree_new(dir_name, leaf_name, NULL, *tree);
  *tree = t;
  return t;
}

void rtbuf_gtk_library_load_file_1 (const char *path,
                                    size_t len,
                                    size_t prefix_len,
                                    size_t suffix_len)
{
  const char *dir_name;
  const char *sep;
  int leaf = 0;
  s_rtbuf_gtk_library_tree **tree = &g_rtbuf_gtk_library_tree;
  s_rtbuf_gtk_library_tree *found;
  printf("Found library %s\n", path);
  dir_name = path + prefix_len;
  while (!leaf) {
    sep = strchr(dir_name, '/');
    if (!sep) {
      sep = path + len - suffix_len;
      leaf = 1;
    }
    found = rtbuf_gtk_library_tree_find_dir_name(*tree,
                                                 dir_name,
                                                 sep - dir_name);
    if (!found) {
      char *m_leaf_name = 0;
      size_t dir_len = sep - dir_name;
      char *m_dir_name = malloc(dir_len + 1);
      memcpy(m_dir_name, dir_name, dir_len);
      m_dir_name[dir_len] = 0;
      if (leaf) {
        size_t leaf_len = len - prefix_len - suffix_len;
        m_leaf_name = malloc(leaf_len + 1);
        memcpy(m_leaf_name, path + prefix_len, leaf_len);
        m_leaf_name[leaf_len] = 0;
      }
      found = rtbuf_gtk_library_tree_insert(tree, m_dir_name, m_leaf_name);
    }
    printf("dir_name %s\n", found->dir_name);
    if (leaf)
      printf("leaf_name %s\n", found->leaf_name);
    tree = &found->children;
    dir_name = sep + 1;
  }
}

void rtbuf_gtk_library_load_file (const char *path, size_t prefix_len)
{
  size_t len = strlen(path);
  printf("file %s\n", path);
  if (strncmp(&path[len - 7], ".so.0.0", 7) == 0)
    rtbuf_gtk_library_load_file_1(path, len, prefix_len, 7);
  else if (strncmp(&path[len - 4], ".dll", 4) == 0)
    rtbuf_gtk_library_load_file_1(path, len, prefix_len, 4);
}

void rtbuf_gtk_library_load_directory (const char *path, size_t prefix_len)
{
  GFile *gf = g_file_new_for_path(path);
  GError *error = NULL;
  GFileEnumerator *dir_enum =
    g_file_enumerate_children(gf, G_FILE_ATTRIBUTE_STANDARD_NAME,
                              G_FILE_QUERY_INFO_NONE, NULL, &error);
  char child_path[PATH_MAX];
  char *out = child_path;
  const char *in = path;
  while (*in)
    *out++ = *in++;
  printf("dir %s\n", path);
  if (dir_enum) {
    while (1) {
      GFileInfo *info;
      GFileType type;
      char *name;
      char *filename = out;
      if (!g_file_enumerator_iterate(dir_enum, &info, NULL, NULL, &error))
        goto out;
      if (!info)
        break;
      type = g_file_info_get_file_type(info);
      name = g_file_info_get_attribute_as_string(info, G_FILE_ATTRIBUTE_STANDARD_NAME);
      if (name[0] != '.') {
        in = name;
        while (*in)
          *filename++ = *in++;
        *filename = 0;
        if (type == G_FILE_TYPE_DIRECTORY) {
          *filename++ = '/';
          *filename = 0;
          rtbuf_gtk_library_load_directory(child_path, prefix_len);
        }
        else
          rtbuf_gtk_library_load_file(child_path, prefix_len);
      }
    }
  }
  return;
 out:
  g_object_unref(dir_enum);
}

void rtbuf_gtk_library_load ()
{
  char **path = g_rtbuf_lib_path;
  if (g_rtbuf_gtk_library_tree) {
    rtbuf_gtk_library_tree_delete(g_rtbuf_gtk_library_tree);
    g_rtbuf_gtk_library_tree = NULL;
  }
  while (*path) {
    printf("Loading rtbuf library in %s\n", *path);
    rtbuf_gtk_library_load_directory(*path, strlen(*path));
    path++;
  }
}
