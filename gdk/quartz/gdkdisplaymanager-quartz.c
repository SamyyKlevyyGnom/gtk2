/* GDK - The GIMP Drawing Kit
 * gdkdisplaymanager-quartz.c
 *
 * Copyright (C) 2005 Imendio AB
 * Copyright 2010 Red Hat, Inc.
 *
 * Author: Matthias clasen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <dlfcn.h>
#include <ApplicationServices/ApplicationServices.h>

#include "gdkquartzdisplay.h"
#include "gdkquartzdisplaymanager.h"
#include "gdkprivate-quartz.h"

#include "gdkdisplaymanagerprivate.h"
#include "gdkinternals.h"

struct _GdkQuartzDisplayManager
{
  GdkDisplayManager parent;
};


G_DEFINE_TYPE (GdkQuartzDisplayManager, gdk_quartz_display_manager, GDK_TYPE_DISPLAY_MANAGER)

static void
gdk_quartz_display_manager_init (GdkQuartzDisplayManager *manager)
{
  ProcessSerialNumber psn = { 0, kCurrentProcess };
  void (*_gtk_quartz_framework_init_ptr) (void);

  /* Make the current process a foreground application, i.e. an app
   * with a user interface, in case we're not running from a .app bundle
   */
  TransformProcessType (&psn, kProcessTransformToForegroundApplication);

  /* Initialize GTK+ framework if there is one. */
  _gtk_quartz_framework_init_ptr = dlsym (RTLD_DEFAULT, "_gtk_quartz_framework_init");
  if (_gtk_quartz_framework_init_ptr)
    _gtk_quartz_framework_init_ptr ();
}

static void
gdk_quartz_display_manager_finalize (GObject *object)
{
  g_error ("A GdkQuartzDisplayManager object was finalized. This should not happen");
  G_OBJECT_CLASS (gdk_quartz_display_manager_parent_class)->finalize (object);
}

static void
gdk_quartz_display_manager_class_init (GdkQuartzDisplayManagerClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayManagerClass *manager_class = GDK_DISPLAY_MANAGER_CLASS (class);

  object_class->finalize = gdk_quartz_display_manager_finalize;

  manager_class->atom_intern = _gdk_quartz_display_manager_atom_intern;
  manager_class->get_atom_name = _gdk_quartz_display_manager_get_atom_name;
}