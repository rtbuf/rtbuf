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
#include "rtbuf_input_widget.h"
#include "rtbuf_widget.h"
#include "rtbuf.h"

struct _RtbufWidgetPrivate {
  s_rtbuf *rtbuf;
  GtkWidget *event_box;
  GtkWidget *label;
  GtkWidget *inputs;
  GtkWidget *outputs;
};

enum {
      PROP_0,
      PROP_RTBUF,
      PROP_LABEL,
      LAST_PROP
};

static GParamSpec *rtbuf_widget_props[LAST_PROP];

static void rtbuf_widget_set_property (GObject      *object,
                                       guint         param_id,
                                       const GValue *value,
                                       GParamSpec   *pspec);
static void rtbuf_widget_get_property (GObject     *object,
                                       guint        param_id,
                                       GValue      *value,
                                       GParamSpec  *pspec);
static void rtbuf_widget_finalize (GObject *object);
static gboolean rtbuf_widget_draw (GtkWidget *widget, cairo_t *cr,
                                   gpointer data);

G_DEFINE_TYPE_WITH_PRIVATE (RtbufWidget, rtbuf_widget, GTK_TYPE_BOX)

static void
rtbuf_widget_class_init (RtbufWidgetClass *klass)
{
  GObjectClass *gobject_class = (GObjectClass*) klass;
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  printf("rtbuf_widget class init\n");
  gobject_class->finalize = rtbuf_widget_finalize;
  gobject_class->set_property = rtbuf_widget_set_property;
  gobject_class->get_property = rtbuf_widget_get_property;
  rtbuf_widget_props[PROP_LABEL] =
    g_param_spec_string("label",
                        "Label",
                        "Text of the frame's label",
                        NULL,
                        G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  rtbuf_widget_props[PROP_RTBUF] =
    g_param_spec_pointer("rtbuf",
                         "Rtbuf",
                         "Pointer to a s_rtbuf",
                         G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  g_object_class_install_properties(gobject_class, LAST_PROP, rtbuf_widget_props);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/rtbuf/rtbuf_widget.ui");
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufWidget, event_box);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufWidget, label);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufWidget, inputs);
  gtk_widget_class_bind_template_child_private(widget_class,
                                               RtbufWidget, outputs);
}

static void
rtbuf_widget_init (RtbufWidget *widget)
{
  RtbufWidgetPrivate *priv;
  s_rtbuf_proc *proc;
  unsigned int i = 0;
  printf("rtbuf_widget init\n");
  priv = rtbuf_widget_get_instance_private(widget);
  gtk_widget_init_template(GTK_WIDGET(widget));
  g_signal_connect(G_OBJECT(widget), "draw",
                   G_CALLBACK(rtbuf_widget_draw), priv);
}

static void 
rtbuf_widget_set_property (GObject *object, guint prop_id,
                           const GValue *value, GParamSpec *pspec)
{
  RtbufWidget *widget = RTBUF_WIDGET(object);
  RtbufWidgetPrivate *priv = rtbuf_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_LABEL:
    rtbuf_widget_set_label(widget, g_value_get_string(value));
    break;
  case PROP_RTBUF:
    rtbuf_widget_set_rtbuf(widget, g_value_get_pointer(value));
    break;
  default:      
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void 
rtbuf_widget_get_property (GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec)
{
  RtbufWidget *widget = RTBUF_WIDGET(object);
  RtbufWidgetPrivate *priv = rtbuf_widget_get_instance_private(widget);
  switch (prop_id) {
  case PROP_LABEL:
    g_value_set_string(value, rtbuf_widget_get_label(widget));
    break;
  case PROP_RTBUF:
    g_value_set_pointer(value, priv->rtbuf);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
rtbuf_widget_finalize (GObject *object)
{
  RtbufWidget *widget = RTBUF_WIDGET(object);
  RtbufWidgetPrivate *priv = rtbuf_widget_get_instance_private(widget);
  g_clear_object(&priv->label);
  g_clear_object(&priv->inputs);
  g_clear_object(&priv->outputs);
  G_OBJECT_CLASS(rtbuf_widget_parent_class)->finalize(object);
}

static gboolean
rtbuf_widget_draw (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  GtkAllocation alloc;
  gtk_widget_get_allocation(widget, &alloc);
  cairo_new_path(cr);
  cairo_rectangle(cr, 0, 0, alloc.width, alloc.height);
  cairo_set_source_rgb(cr, 1.0, 1.0, 0.7);
  cairo_fill(cr);
  (void) data;
  return FALSE;
}

RtbufWidget *
rtbuf_widget_new (s_rtbuf *rtb, const gchar *label)
{
  return (RtbufWidget*) g_object_new(RTBUF_WIDGET_TYPE,
                                     "label", label,
                                     "rtbuf", rtb,
                                     NULL);
}

void gtk_container_clear_widget (GtkWidget *widget,
                                 gpointer data)
{
  GtkContainer *container = (GtkContainer*) data;
  gtk_container_remove(container, widget);
}

void gtk_container_clear (GtkContainer *container)
{
  gtk_container_foreach(container,
                        gtk_container_clear_widget,
                        container);
}

void rtbuf_widget_set_rtbuf (RtbufWidget *widget, s_rtbuf *rtbuf)
{
  
  RtbufWidgetPrivate *priv = rtbuf_widget_get_instance_private(widget);
  GtkContainer *inputs;
  s_rtbuf_proc *proc;
  unsigned int i;
  printf("rtbuf_widget set rtbuf\n");
  priv->rtbuf = rtbuf;
  proc = rtbuf->proc;
  inputs = GTK_CONTAINER(priv->inputs);
  gtk_container_clear(inputs);
  for (i = 0; i < proc->in_n; i++) {
    RtbufInputWidget *input_widget =
      rtbuf_input_widget_new(priv->rtbuf, i);
    gtk_container_add(inputs, GTK_WIDGET(input_widget));
    i++;
  }
  for (i = 0; i < proc->out_n; i++) {
  }
}

void rtbuf_widget_set_label (RtbufWidget *widget,
                             const gchar *label)
{
  g_return_if_fail(IS_RTBUF_WIDGET(widget));
  if (label) {
    RtbufWidgetPrivate *priv = rtbuf_widget_get_instance_private(widget);
    if (GTK_IS_LABEL(priv->label))
      gtk_label_set_text(GTK_LABEL(priv->label), label);
  }
}

const gchar *
rtbuf_widget_get_label (RtbufWidget *widget)
{
  RtbufWidgetPrivate *priv;
  priv = rtbuf_widget_get_instance_private(widget);
  if (priv && priv->label)
    return gtk_label_get_text(GTK_LABEL(priv->label));
  return NULL;
}

GtkWidget *
rtbuf_widget_get_event_box (RtbufWidget *widget)
{
  RtbufWidgetPrivate *priv;
  priv = rtbuf_widget_get_instance_private(widget);
  if (priv)
    return priv->event_box;
  return NULL;
}

s_rtbuf *
rtbuf_widget_get_rtbuf (RtbufWidget *widget)
{
  RtbufWidgetPrivate *priv;
  priv = rtbuf_widget_get_instance_private(widget);
  if (priv)
    return priv->rtbuf;
  return NULL;
}
