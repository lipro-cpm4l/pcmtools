#!/bin/sh

#
# Autotools bootstrap script for top source directory
#
# Copyright (C) 21015  Stephan Linz <linz@li-pro.net>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
#

#
# usage:
#
# banner <target name>
#
banner() {
	echo
	TG=`echo "autogen.sh: $1" | sed -e "s,/.*/,,g"`
	LINE=`echo $TG |sed -e "s/./-/g"`
	echo $LINE
	echo $TG
	echo $LINE
	echo
}

set -e

chmod +x $0
chmod +x ./tools/get-version

[ ! -f .gitmodules ] || {
	banner "updating git submodules"
	git submodule init
	git submodule update

}

case "$(uname)" in
    Darwin)
        LIBTOOLIZE=${LIBTOOLIZE:-glibtoolize}
        ;;
    *)
        LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}
        ;;
esac
AUTORECONF=${AUTORECONF:-autoreconf}
ACLOCAL=${ACLOCAL:-aclocal}
AUTOCONF=${AUTOCONF:-autoconf}
AUTOHEADER=${AUTOHEADER:-autoheader}
AUTOMAKE=${AUTOMAKE:-automake}
AUTOPOINT=${AUTOPOINT:-autopoint}
AUTOM4TE=${AUTOM4TE:-autom4te}
MAKE=${MAKE:-make}
M4=${M4:-m4}

# Check we have all tools installed
check_command() {
    command -v "${1}" > /dev/null 2>&1 || {
        >&2 banner "could not find \`$1'. \`$1' is required to run autogen.sh."
        exit 1
    }
}
check_command "$LIBTOOLIZE"
check_command "$AUTORECONF"
check_command "$ACLOCAL"
check_command "$AUTOCONF"
check_command "$AUTOHEADER"
check_command "$AUTOMAKE"

# Absence of pkg-config or misconfiguration can make some odd error
# messages, we check if it is installed correctly. See:
#  https://blogs.oracle.com/mandy/entry/autoconf_weirdness
#
# We cannot just check for pkg-config command, we need to check for
# PKG_* macros. The pkg-config command can be defined in ./configure,
# we cannot tell anything when not present.
check_pkg_config() {
    grep -q '^AC_DEFUN.*PKG_CHECK_MODULES' aclocal.m4 || {
        cat <<EOF >&2
autogen.sh: could not find PKG_CHECK_MODULES macro.

  Either pkg-config is not installed on your system or
  \`pkg.m4' is missing or not found by aclocal.

  If \`pkg.m4' is installed at an unusual location, re-run
  \`autogen.sh' by setting \`ACLOCAL_FLAGS':

    ACLOCAL_FLAGS="-I <prefix>/share/aclocal" ./autogen.sh

EOF
        exit 1
    }
}

banner "start libtoolize to get ltmain.sh"
${LIBTOOLIZE} --copy --install --force
banner "start aclocal to get all m4 macros locally in m4/"
${ACLOCAL} -I m4 --install --force --warnings=all ${ACLOCAL_FLAGS}
banner "reconfigure with autoreconf"
${AUTORECONF} -I m4 --install --force --warnings=all || {
    banner "autoreconf has failed ($?), let's do it manually"
    for dir in $PWD *; do
        [ -d "$dir" ] || continue
        [ -f "$dir"/configure.ac ] || [ -f "$dir"/configure.in ] || continue
	banner "configure `basename $dir`"
	(cd "$dir" && ${ACLOCAL} -I m4 --install --force --warnings=all ${ACLOCAL_FLAGS})
        (cd "$dir" && check_pkg_config)
	(cd "$dir" && ${LIBTOOLIZE} --automake --copy --install --force)
	(cd "$dir" && ${ACLOCAL} -I m4 --install --force --warnings=all ${ACLOCAL_FLAGS})
	(cd "$dir" && ${AUTOCONF} --force --warnings=all)
	(cd "$dir" && ${AUTOHEADER} --force --warnings=all)
	(cd "$dir" && ${AUTOMAKE} --add-missing --copy --force-missing)
    done
}

banner "well known issues to work on"

eval $(${MAKE} -n -f Makefile.am top_srcdir=. TODO | \
       sed -e 's/\\\s*$//g' -e 's/\s*>\s*TODO\s*$//g')

banner "finished - for the next step, run ./configure"

exit 0
