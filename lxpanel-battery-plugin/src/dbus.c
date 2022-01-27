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
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <glib.h>

#include "dbus.h"

static DBusConnection* conn;

static DBusConnection *dbus_connect()
{
    DBusError err;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if (dbus_error_is_set(&err))
    {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
    }

    if (!conn)
    {
        fprintf(stderr, "Cannot initialize the DBus connection");
        exit(1);
    }

    return conn;
}

static void add_signal_listener()
{
    DBusError err;
    dbus_bus_add_match(
        conn,
        "type='signal',interface='io.cutiepi.interface',path='/mcu'",
        &err);

    dbus_connection_flush(conn);
    if (dbus_error_is_set(&err))
    {
        fprintf(stderr, "Match Error (%s)\n", err.message);
        exit(1);
    }
}

static void reset_battery_info(BatteryInfo* info)
{
    info->_is_parsed[0] = info->_is_parsed[1] = false;
}

static bool is_battery_info_parsed(BatteryInfo* info)
{
    return info->_is_parsed[0] && info->_is_parsed[1];
}

static void process_signal(DBusMessage* msg, DBusMessageIter* iter, BatteryInfo* info)
{
    char current_type;
    char* str_val = NO_STRING_VAL;
    int int_val = NO_INT_VAL;

    while ((current_type = dbus_message_iter_get_arg_type(iter)) != DBUS_TYPE_INVALID)
    {
        if (current_type == DBUS_TYPE_STRING)
            dbus_message_iter_get_basic(iter, &str_val);
        else if (current_type == DBUS_TYPE_INT32)
            dbus_message_iter_get_basic(iter, &int_val);

        if (strlen(str_val) && int_val != NO_INT_VAL)
        {
            if (!strncmp(str_val, BATTERY_LEVEL_VARNAME, strlen(str_val)-1))
            {
                info->level = (int) MIN(MAX(((int_val * 100) / MAX_BATTERY_VOLTAGE), 0), 100);
                info->_is_parsed[0] = true;
            }
            else if (!strncmp(str_val, BATTERY_STATE_VARNAME, strlen(str_val)-1))
            {
                info->is_charging = int_val == BATTERY_STATE_CHARGING;
                info->_is_parsed[1] = true;
            }

            str_val = NO_STRING_VAL;
            int_val = NO_INT_VAL;
        }

        dbus_message_iter_next(iter);
    }
}

void dbus_init()
{
    dbus_connect();
    add_signal_listener();
}

void get_battery_info(BatteryInfo* info)
{
    DBusMessage* msg;
    DBusMessageIter iter;
    reset_battery_info(info);

    while (!is_battery_info_parsed(info))
    {
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);

        if (!msg)
            continue;

        if (dbus_message_is_signal(msg, "io.cutiepi.interface", "updateEvent"))
        {
            if (!dbus_message_iter_init(msg, &iter))
                fprintf(stderr, "Message has no arguments!\n");
            else
                process_signal(msg, &iter, info);
        }

        dbus_message_unref(msg);
    }
}

