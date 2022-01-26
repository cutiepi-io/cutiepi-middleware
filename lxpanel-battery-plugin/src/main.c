/**
 *
 * Copyright (c) 2022 Fabio Manganiello, see the file AUTHORS for details.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <glib.h>
#include <lxpanel/plugin.h>

#include "dbus.h"

#define    POLL_INTERVAL_MS    1000

static gboolean refresh_state(void* data)
{
    GtkWidget* label = (GtkWidget *) data;
    BatteryInfo info;
    char label_str[32] = {0};

    get_battery_info(&info);
    snprintf(
        label_str,
        sizeof(label_str)-1,
        "%s %d%%",
        info.is_charging ? "⚡" : "🔋",
        info.level
    );

    gtk_label_set_text(GTK_LABEL(label), label_str);
    return TRUE;
}

GtkWidget *constructor(LXPanel *panel, config_setting_t *settings)
{
    /* panel is a pointer to the panel and
       settings is a pointer to the configuration data
       since we don't use it, we'll make sure it doesn't
       give us an error at compilation time */
    (void)panel;
    (void)settings;

    dbus_init();
    GtkWidget *p = gtk_event_box_new();
    gtk_widget_set_has_window(p, FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(p), 1);

    GtkWidget *label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(p), label);
    gtk_widget_show(label);

    g_timeout_add(POLL_INTERVAL_MS, refresh_state, label);

    // set the size we want
    // gtk_widget_set_size_request(p, 100, 25);

    return p;
}

FM_DEFINE_MODULE(lxpanel_gtk, test)

LXPanelPluginInit fm_module_init_lxpanel_gtk = {
   .name = "CutiePi battery",
   .description = "Displays the current state of the CutiePi battery",
   .new_instance = constructor
};
