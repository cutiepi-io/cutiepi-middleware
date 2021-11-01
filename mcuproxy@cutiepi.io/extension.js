/* extension.js
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/* exported init */

const GLib = imports.gi.GLib;
const Gio = imports.gi.Gio;
const Util = imports.misc.util;

const ifaceXml = `
<node>
  <interface name="io.cutiepi.interface">
    <signal name="updateEvent">
      <arg name="type" type="s"/>
      <arg name="value" type="i"/>
    </signal>
  </interface>
</node>`;

const McuProxy = Gio.DBusProxy.makeProxyWrapper(ifaceXml);

let proxy = new McuProxy(
    Gio.DBus.system,
    'io.cutiepi.service',
    '/mcu');

let handlerId = null;

const GETTEXT_DOMAIN = 'my-indicator-extension';

const { GObject, St, Clutter } = imports.gi;

const Gettext = imports.gettext.domain(GETTEXT_DOMAIN);
const _ = Gettext.gettext;

const ExtensionUtils = imports.misc.extensionUtils;
const Main = imports.ui.main;
const PanelMenu = imports.ui.panelMenu;
const PopupMenu = imports.ui.popupMenu;

const batteryAttributes = 
    {
        '4.20': 100, '3.99': 95, '3.97': 90, '3.92': 85, '3.87': 80, '3.83': 75, '3.79': 70,
        '3.75': 65, '3.73': 60, '3.70': 55, '3.68': 50, '3.66': 45, '3.65': 40, '3.63': 35,
        '3.62': 30, '3.60': 25, '3.58': 20, '3.545': 15, '3.51': 10, '3.42': 5, '3.00': 0
    }

let voltageQueue = []; 

const Indicator = GObject.registerClass(
class Indicator extends PanelMenu.Button {
    _init() {
        super._init(0.0, _('CutiePi battery indicator'));

        let box = new St.BoxLayout({ style_class: 'panel-status-menu-box' });
        box.add_child(new St.Icon({
            icon_name: 'battery-full-symbolic',
            style_class: 'system-status-icon',
        }));

        box.add_child(new St.Label({ 
            text: 'Calculating', 
            y_align: Clutter.ActorAlign.CENTER,
        }));
        this.add_child(box);

        function getIcon(percentage, charging) {
            if (percentage < 10)
                return charging ? "battery-caution-charging-symbolic" : "battery-caution-symbolic";
            else if (percentage < 30)
                return charging ? "battery-low-charging-symbolic" : "battery-low-symbolic";
            else if (percentage < 60)
                return charging ? "battery-medium-charging-symbolic" : "battery-medium-symbolic";
            else if (percentage < 90)
                return charging ? "battery-good-charging-symbolic" : "battery-good-symbolic";
            return charging ? "battery-full-charging-symbolic" : "battery-full-symbolic";
        }

        let eventValue = null
        let eventType = null 
        let charging = null 
        let locked = false 
        handlerId = proxy.connectSignal('updateEvent', (proxy, nameOwner, args) => {
            //print(`updateEvent: ${args[0]}, ${args[1]}`);
            eventType = args[0];
            eventValue = args[1];
            switch (eventType) {
                case 'button': 
                    if (eventValue === 1) {
                        if (locked)
                            Util.spawn(['/usr/local/bin/lockscreen.sh', '0']);
                        else 
                            Util.spawn(['/usr/local/bin/lockscreen.sh', '1']);
                        locked = !locked; 
                    }
                    if (eventValue === 3) {
                        Util.spawn(['gnome-session-quit', '--power-off']);
                    }
                break;
                case 'charge': 
                    if (eventValue === 5)
                        charging = false 
                    else 
                        charging = true 
                break; 
                case 'battery': 
                    var currentVol = (eventValue / 1000).toFixed(2);
                    var sum = 0;
                    voltageQueue.push(currentVol);
                    if (voltageQueue.length > 10)
                        voltageQueue.shift()
                    for (var i = 0; i < voltageQueue.length; i++) {
                        sum += parseFloat(voltageQueue[i])
                    }
                    var meanVol = (sum / voltageQueue.length).toFixed(2);
                    for (var vol in batteryAttributes) {
                        if (meanVol >= parseFloat(vol)) {
                            var volPercent = batteryAttributes[vol];
                            box.get_child_at_index(0).icon_name = getIcon(parseInt(volPercent), charging);
                            box.get_child_at_index(1).text = charging ? "Charging" : volPercent.toString() + '%';
                            break;
                        }
                    }
                break; 
            }
        });
    }
});

class Extension {
    constructor(uuid) {
        this._uuid = uuid;

        ExtensionUtils.initTranslations(GETTEXT_DOMAIN);
    }

    enable() {
        this._indicator = new Indicator();
        Main.panel.addToStatusArea(this._uuid, this._indicator);
    }

    disable() {
        this._indicator.destroy();
        this._indicator = null;
    }
}

function init(meta) {
    return new Extension(meta.uuid);
}
