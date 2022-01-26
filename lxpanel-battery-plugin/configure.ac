AC_PREREQ([2.69])
AC_INIT([lxpanel-cutiepi-battery],[0.1])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

LT_INIT
AC_PROG_CC
AC_PROG_INSTALL
AC_CONFIG_HEADERS([config.h.in])

# Checks for libraries.
pkg_modules="glib-2.0 >= 2.6.0 \
             gtk+-2.0 >= 2.6.0 \
             gthread-2.0 \
             dbus-glib-1 \
             lxpanel"

PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

#prefix=$(pkg-config --variable=prefix lxpanel)
LXPANEL_LIBDIR=$(pkg-config --variable=pkglibdir lxpanel)
LXPANEL_PLUGINSDIR=$(pkg-config --variable=pluginsdir lxpanel)
AC_SUBST(LXPANEL_LIBDIR)
prefix=$LXPANEL_LIBDIR
libdir=$LXPANEL_PLUGINSDIR

LXPANEL_CPPFLAGS=$(pkg-config --cflags --libs lxpanel)
AC_SUBST(LXPANEL_CPPFLAGS)

GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

LXPANEL_MODULE="-avoid-version \
-no-undefined "'-export-symbols-regex "^[[^_]].*"'
AC_SUBST(LXPANEL_MODULE)

ALL_LINGUAS=""

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT

echo
echo LXPanel Plugin: $PACKAGE
echo        Version: $VERSION
echo         Prefix: $prefix

