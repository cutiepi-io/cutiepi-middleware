'use strict';

const GLib = imports.gi.GLib;
const Gio = imports.gi.Gio;

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
        Gio.DBus.session,
        'io.cutiepi.service',
        '/mcu');

proxy.connectSignal('updateEvent', (proxy, nameOwner, args) => {
    print(`updateEvent: ${args[0]}, ${args[1]}`);
});

let loop = GLib.MainLoop.new(null, false);
loop.run();