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
#include <gtk/gtk.h>
#include "rtbuf_gtk.h"
#include "rtbuf_input_widget.h"
#include <rtbuf/rtbuf.h>

struct _RtbufInputWidgetPrivate {
  s_rtbuf   *rtbuf;
  int        in;
  GtkWidget *check;
  GtkWidget *label;
  GtkWidget *value;
  GtkWidget *min;
  GtkWidget *slider;
  GtkWidget *max;
};

enum {
      PROP_0,
      PROP_RTBUF,
      PROP_IN,
      LAST_PROP
};

static GParamSpec *rtbuf_input_widget_props[LAST_PROP];

static void rtbuf_input_widget_set_property (GObject      *object,
                                             guint         param_id,
                                             const GValue *value,
                                             GParamSpec   *pspec);
static void rtbuf_input_widget_get_property (GObject     *object,
                                             guint        param_id,
                                             GValue      *value,
                                             GParamSpec  *pspec);
static void rtbuf_input_widget_finalize (GObject *object);

G_DEFINE_TYPE_WITH_PRIVATE (RtbufInputWidget, rtbuf_input_widget, GTK_TYPE_BOX)

static void
rtbuf_input_widget_class_init (RtbufInputWidgetClass *klass)
{
  GObjectClass *gobject_class = (GObjectClass*) klass;
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  printf("rtbuf_input_widget class init\n");
  gobject_class->finalize = rtbuf_input_widget_finalize;
  gobject_class->set_property = rtbuf_input_widget_set_property;
  gobject_class->get_property = rtbuf_input_widget_get_property;
  rtbuf_input_widget_props[PROP_RTBUF] =
    g_param_spec_pointer("rtbuf",
                         "Rtbuf",
                         "Pointer to a s_rtbuf",
                         G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  rtbuf_input_widget_props[PROP_IN] =
    g_param_spec_int("in",
                     "Input",
                     "rtbuf input index",
                     -1,                  /* min */
                     RTBUF_PROC_IN_MAX,   /* max */
                     -1,                  /* default */
                     G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  g_object_class_install_properties(gobject_class, LAST_PROP, rtbuf_input_widget_props);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/rtbuf/rtbuf_input_widget.ui");
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, check);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, label);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, value);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, min);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, slider);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufInputWidget, max);
}

static void
rtbuf_input_widget_init (RtbufInputWidget *widget)
{
  printf("rtbuf_input_widget init\n");
  gtk_widget_init_template(GTK_WIDGET(widget));
}

static void 
rtbuf_input_widget_set_property (GObject *object, guint prop_id,
                                 const GValue *value, GParamSpec *pspec)
{
  RtbufInputWidget *widget = RTBUF_INPUT_WIDGET(object);
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_RTBUF:
    priv->rtbuf = g_value_get_pointer(value);
    rtbuf_input_widget_update_rtbuf_in(widget);
    break;
  case PROP_IN:
    priv->in = g_value_get_int(value);
    rtbuf_input_widget_update_rtbuf_in(widget);
    break;
  default:      
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void 
rtbuf_input_widget_get_property (GObject *object, guint prop_id,
                                 GValue *value, GParamSpec *pspec)
{
  RtbufInputWidget *widget = RTBUF_INPUT_WIDGET(object);
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_RTBUF:
    g_value_set_pointer(value, priv->rtbuf);
    break;
  case PROP_IN:
    g_value_set_int(value, priv->in);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
rtbuf_input_widget_finalize (GObject *object)
{
  RtbufInputWidget *widget = RTBUF_INPUT_WIDGET(object);
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  g_clear_object(&priv->check);
  g_clear_object(&priv->label);
  g_clear_object(&priv->value);
  g_clear_object(&priv->min);
  g_clear_object(&priv->slider);
  g_clear_object(&priv->max);
  G_OBJECT_CLASS(rtbuf_input_widget_parent_class)->finalize(object);
}

RtbufInputWidget *
rtbuf_input_widget_new (s_rtbuf *rtbuf, unsigned int in)
{
  return (RtbufInputWidget*) g_object_new(RTBUF_INPUT_WIDGET_TYPE,
                                          "rtbuf", rtbuf,
                                          "in", in,
                                          NULL);
}

s_rtbuf *
rtbuf_input_widget_get_rtbuf (RtbufInputWidget *widget)
{
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  if (priv)
    return priv->rtbuf;
  return NULL;
}

unsigned int
rtbuf_input_widget_get_in (RtbufInputWidget *widget)
{
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  if (priv)
    return priv->in;
  return 0;
}

GtkWidget *
rtbuf_input_widget_get_check (RtbufInputWidget *widget)
{
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  if (priv)
    return priv->check;
  return NULL;
}

void rtbuf_input_widget_slider_value_changed (RtbufInputWidgetPrivate *priv)
{
  double value;
  char str[64];
  value = gtk_range_get_value(GTK_RANGE(priv->slider));
  snprintf(str, sizeof(str), "%lg", value);
  gtk_entry_set_text(GTK_ENTRY(priv->value), str);
}
 
void
rtbuf_input_widget_update_rtbuf_in (RtbufInputWidget *widget)
{
  static const char *sym_double = NULL;
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  if (!sym_double)
    sym_double = symbol_intern("double");
  if (priv && priv->rtbuf && priv->in >= 0) {
    const s_rtbuf_proc *proc = priv->rtbuf->proc;
    const s_rtbuf_proc_in *in = &proc->in[priv->in];
    const char *label;
    char min[64];
    char max[64];
    char value[64];
    double unbound_value;
    assert((long long) priv->in < (long long) proc->in_n);
    label = in->name_type;
    gtk_label_set_text(GTK_LABEL(priv->label), label);
    snprintf(min, sizeof(min), "%lg", in->min);
    gtk_entry_set_text(GTK_ENTRY(priv->min), min);
    snprintf(max, sizeof(max), "%lg", in->max);
    gtk_entry_set_text(GTK_ENTRY(priv->max), max);
    unbound_value = *rtbuf_in_unbound_value(priv->rtbuf, priv->in);
    snprintf(value, sizeof(value), "%lg", unbound_value);
    gtk_entry_set_text(GTK_ENTRY(priv->value), value);
    gtk_range_set_range(GTK_RANGE(priv->slider), in->min, in->max);
    gtk_range_set_value(GTK_RANGE(priv->slider), unbound_value);
    g_signal_connect_swapped(priv->slider, "value-changed",
                             G_CALLBACK(rtbuf_input_widget_slider_value_changed),
                             priv);
  }
}

void rtbuf_input_widget_connect (GtkWidget *widget,
                                 gpointer data)
{
  s_signal_binding *sb = (s_signal_binding*) data;
  g_signal_connect(G_OBJECT(widget), sb->signal,
                   sb->callback, NULL);
}

void rtbuf_input_widget_connect_check (GtkWidget *input,
                                       gpointer data)
{
  RtbufInputWidget *widget = RTBUF_INPUT_WIDGET(input);
  RtbufInputWidgetPrivate *priv =
    rtbuf_input_widget_get_instance_private(widget);
  s_signal_binding *sb = (s_signal_binding*) data;
  g_signal_connect_swapped(G_OBJECT(priv->check), sb->signal,
                           sb->callback, widget);
}
