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
#include "rtbuf_output_widget.h"
#include "rtbuf.h"

struct _RtbufOutputWidgetPrivate {
  s_rtbuf   *rtbuf;
  int        out;
  GtkWidget *check;
  GtkWidget *label;
};

enum {
      PROP_0,
      PROP_RTBUF,
      PROP_OUT,
      LAST_PROP
};

static GParamSpec *rtbuf_output_widget_props[LAST_PROP];

static void rtbuf_output_widget_set_property (GObject      *object,
                                              guint         param_id,
                                              const GValue *value,
                                              GParamSpec   *pspec);
static void rtbuf_output_widget_get_property (GObject     *object,
                                              guint        param_id,
                                              GValue      *value,
                                              GParamSpec  *pspec);
static void rtbuf_output_widget_finalize (GObject *object);

G_DEFINE_TYPE_WITH_PRIVATE (RtbufOutputWidget, rtbuf_output_widget, GTK_TYPE_BOX)

static void
rtbuf_output_widget_class_init (RtbufOutputWidgetClass *klass)
{
  GObjectClass *gobject_class = (GObjectClass*) klass;
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  printf("rtbuf_output_widget class init\n");
  gobject_class->finalize = rtbuf_output_widget_finalize;
  gobject_class->set_property = rtbuf_output_widget_set_property;
  gobject_class->get_property = rtbuf_output_widget_get_property;
  rtbuf_output_widget_props[PROP_RTBUF] =
    g_param_spec_pointer("rtbuf",
                         "Rtbuf",
                         "Pointer to a s_rtbuf",
                         G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  rtbuf_output_widget_props[PROP_OUT] =
    g_param_spec_int("out",
                     "Output",
                     "rtbuf output index",
                     -1,                   /* min */
                     RTBUF_PROC_OUT_MAX,   /* max */
                     -1,                   /* default */
                     G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  g_object_class_install_properties(gobject_class, LAST_PROP, rtbuf_output_widget_props);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/rtbuf/rtbuf_output_widget.ui");
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufOutputWidget, check);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufOutputWidget, label);
}

static void
rtbuf_output_widget_init (RtbufOutputWidget *widget)
{
  printf("rtbuf_output_widget init\n");
  gtk_widget_init_template(GTK_WIDGET(widget));
}

static void 
rtbuf_output_widget_set_property (GObject *object, guint prop_id,
                                  const GValue *value,
                                  GParamSpec *pspec)
{
  RtbufOutputWidget *widget = RTBUF_OUTPUT_WIDGET(object);
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_RTBUF:
    priv->rtbuf = g_value_get_pointer(value);
    rtbuf_output_widget_update(widget);
    break;
  case PROP_OUT:
    priv->out = g_value_get_int(value);
    rtbuf_output_widget_update(widget);
    break;
  default:      
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void 
rtbuf_output_widget_get_property (GObject *object, guint prop_id,
                                  GValue *value, GParamSpec *pspec)
{
  RtbufOutputWidget *widget = RTBUF_OUTPUT_WIDGET(object);
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_RTBUF:
    g_value_set_pointer(value, priv->rtbuf);
    break;
  case PROP_OUT:
    g_value_set_int(value, priv->out);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
rtbuf_output_widget_finalize (GObject *object)
{
  RtbufOutputWidget *widget = RTBUF_OUTPUT_WIDGET(object);
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  g_clear_object(&priv->check);
  g_clear_object(&priv->label);
  G_OBJECT_CLASS(rtbuf_output_widget_parent_class)->finalize(object);
}

RtbufOutputWidget *
rtbuf_output_widget_new (s_rtbuf *rtbuf, unsigned int out)
{
  return (RtbufOutputWidget*) g_object_new(RTBUF_OUTPUT_WIDGET_TYPE,
                                          "rtbuf", rtbuf,
                                          "out", out,
                                          NULL);
}

s_rtbuf *
rtbuf_output_widget_get_rtbuf (RtbufOutputWidget *widget)
{
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  if (priv)
    return priv->rtbuf;
  return NULL;
}

unsigned int
rtbuf_output_widget_get_out (RtbufOutputWidget *widget)
{
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  if (priv)
    return priv->out;
  return 0;
}

GtkWidget *
rtbuf_output_widget_get_check (RtbufOutputWidget *widget)
{
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  if (priv)
    return priv->check;
  return NULL;
}

void
rtbuf_output_widget_update (RtbufOutputWidget *widget)
{
  const RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  if (priv && priv->rtbuf && priv->out >= 0) {
    const s_rtbuf_proc *proc = priv->rtbuf->proc;
    const char *label;
    assert(priv->out < proc->out_n);
    label = proc->out[priv->out].name;
    gtk_label_set_text(GTK_LABEL(priv->label), label);
  }
}

void rtbuf_output_widget_connect_check (GtkWidget *output,
                                        gpointer data)
{
  RtbufOutputWidget *widget = RTBUF_OUTPUT_WIDGET(output);
  RtbufOutputWidgetPrivate *priv =
    rtbuf_output_widget_get_instance_private(widget);
  s_signal_binding *sb = (s_signal_binding*) data;
  g_signal_connect_swapped(G_OBJECT(priv->check), sb->signal,
                           sb->callback, widget);
}
