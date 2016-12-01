/* -*- mode: C++; c-file-style: "gnu-mode" -*- */
/* BEEBS stringsearch1 benchmark

   Copyright (C) 2013 Embecosm Limited and University of Bristol

   Contributor: James Pallister <james.pallister@bristol.ac.uk>

   This file is part of the Bristol/Embecosm Embedded Benchmark Suite.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include "support.h"

#include <string.h>

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

#ifndef	CHARTYPE
#define	CHARTYPE	unsigned char
#endif

void prep1(CHARTYPE *base, int m);
void exec1(CHARTYPE *base, int n);
void prep2(CHARTYPE *base, int m);
void exec2(CHARTYPE *base, int n);

char buf[] = "abacacbabbabbadcabdcabccacacbadbadbcabdcabcbadcbacabadbadcabcbacdcacabacabcabcbadcbacabadbadcabcbac";
char search[] ="abc";

int
benchmark (void)
{
  prep1((CHARTYPE *) search, 3);
  exec1((CHARTYPE *) buf, strlen(buf));
  prep2((CHARTYPE *) search, 3);
  exec2((CHARTYPE *) buf, strlen(buf));
  return 0;
}


