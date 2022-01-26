#! /bin/sh
AM_VERSION=-1.11
AC_VERSION=

set -x

if [ "x${ACLOCAL_DIR}" != "x" ]; then
  ACLOCAL_ARG=-I ${ACLOCAL_DIR}
fi

${ACLOCAL:-aclocal$AC_VERSION} ${ACLOCAL_ARG}
${AUTOHEADER:-autoheader$AC_VERSION}
AUTOMAKE=${AUTOMAKE:-automake} libtoolize -c --automake --force
AUTOMAKE=${AUTOMAKE:-automake} intltoolize -c --automake --force
${AUTOMAKE:-automake} --add-missing --copy --include-deps
${AUTOCONF:-autoconf$AC_VERSION}

rm -rf autom4te.cache
