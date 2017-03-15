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

bin_PROGRAMS=sglib-arrayheapsort

sglib_arrayheapsort_SOURCES=../sglib-arraysort/arraysort.c \
			    ../sglib-arraysort/sglib.h

include $(top_srcdir)/src/common.mk.am

sglib_arrayheapsort_CFLAGS = -DHEAP_SORT ${AM_CFLAGS}

sglib_arrayheapsort_SOURCES += ${platform_src}
