# cutiepi-lxpanel-battery

lxpanel plugin for the CutiePi battery.

## Installation from sources

1. Install the dependencies

```shell
$ sudo apt install \
    build-essential autoconf automake libtool intltool \
    libglib2.0-dev libgtk2.0-dev lxpanel-dev libdbus-glib-1-dev
```

2. Clone the repo, compile and install the plugin

```shell
$ git clone https://github.com/BlackLight/cutiepi-lxpanel-battery.git
$ cd cutiepi-lxpanel-battery
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

3. Restart `lxpanel` if it's running

```shell
$ lxpanelctl restart
```

## Usage

1. Right click on the panel

2. Select `Add / Remove Panel Items`

3. Click `Add`

4. Select `CutiePi battery`

## Credit

[Original repo](https://github.com/BlackLight/cutiepi-lxpanel-battery) by [Fabio Manganiello](https://github.com/BlackLight).

