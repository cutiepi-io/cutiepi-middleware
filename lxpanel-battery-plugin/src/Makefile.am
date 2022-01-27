AM_CPPFLAGS = \
	-I. \
	-I$(top_srcdir)/src \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(PACKAGE_CFLAGS) \
	$(G_CAST_CHECKS)

module_LTLIBRARIES = cutiepi_battery.la

moduledir = $(libdir)

cutiepi_battery_la_SOURCES = \
	main.c \
	dbus.c

cutiepi_battery_la_LIBADD = \
	$(PACKAGE_LIBS)

cutiepi_battery_la_LDFLAGS = \
	-module \
	@LXPANEL_MODULE@
