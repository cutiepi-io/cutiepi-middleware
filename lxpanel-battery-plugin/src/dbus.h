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

#ifndef _DBUS_BATTERY_H
#define _DBUS_BATTERY_H

#include <stdint.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

#define   BATTERY_STATE_CHARGING       (4)
#define   BATTERY_STATE_DISCHARGING    (5)
#define   NO_STRING_VAL                ("")
#define   NO_INT_VAL                   (-1)
#define   BATTERY_LEVEL_VARNAME        ("battery")
#define   BATTERY_STATE_VARNAME        ("charge")
/* Expressed in mV (I guess). Value based on empirical evidence */
#define   MAX_BATTERY_VOLTAGE          (4000)

typedef struct {
    uint8_t level;  /* Battery level in percentage between 0-100 */
    bool    is_charging;
    bool    _is_parsed[2];
} BatteryInfo;

extern void dbus_init();
extern void get_battery_info(BatteryInfo* info);

#endif   /* _DBUS_BATTERY_H */
