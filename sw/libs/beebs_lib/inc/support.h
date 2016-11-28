/* Copyright (C) 2014 Embecosm Limited and the University of Bristol

   Contributor James Pallister <james.pallister@bristol.ac.uk>

   This file is part of BEEBS

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef BEEBS_H

#define BEEBS_H

#warning support.h compiled
#define SUPPORT_H 1
#define HAVE_BOARDSUPPORT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_BOARDSUPPORT_H
#warning ma perche
#include "boardsupport.h"
#else
// Default board support is here
#define BOARD_REPEAT_FACTOR   4096
#endif

#ifndef BOARD_REPEAT_FACTOR
#define BOARD_REPEAT_FACTOR 4096
#endif

// If we don't define a default scaling value for the benchmark, define
#ifndef CALIB_SCALE
#define CALIB_SCALE 0
#endif

// The overall repeat factor is scaled by the command-line given
// CALIB_SCALE.
#define REPEAT_FACTOR (((CALIB_SCALE) > 0)?(BOARD_REPEAT_FACTOR) >> (CALIB_SCALE):(BOARD_REPEAT_FACTOR) << (-CALIB_SCALE))

#ifdef HAVE_CHIPSUPPORT_H
// #include "@srcdir@/config/@ARCH@/chips/@CHIP@/chipsupport.h"
#include "chipsupport.h"
#else
// Default chip support is here
#warning No chipsupport.h
#endif

int benchmark () __attribute__ ((noinline));
#endif