# Makefile.am for building a BEEBS benchmark
#
# Copyright (C) 2013 Embecosm Limited and University of Bristol
#
# This file is part of the Bristol/Embecosm Embedded Benchmark Suite.
#
# Contributor: James Pallister <james.pallister@bristol.ac.uk>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# Change every instance of mont64 to the name of your benchmark

# To get automake to use this file, add the following to toplevel configure.ac
#   AC_CONFIG_FILES([src/mont64/Makefile])
# and add the following line to toplevel Makefile.am
#   SUBDIRS+=src/mont64


# Make sure your sources include the headers as well as the C/C++ source
# files, so they go into the distribution. If there are other files that are
# needed, they should go in EXTRA_SOURCES.

bin_PROGRAMS=aha-mont64
aha_mont64_SOURCES=mont64.c

include $(top_srcdir)/src/common.mk.am

aha_mont64_SOURCES += ${platform_src}
