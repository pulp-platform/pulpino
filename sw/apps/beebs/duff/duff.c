/* BEEBS duff benchmark

   Copyright (C) 2014 Embecosm Limited and University of Bristol

   Contributor James Pallister <james.pallister@bristol.ac.uk>

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

/* Original code from: WCET Benchmarks,
       http://www.mrtc.mdh.se/projects/wcet/benchmarks.html
   Permission to license under GPL obtained by email from Björn Lisper
 */

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)


/* $Id: duff.c,v 1.2 2005/04/04 11:34:58 csg Exp $ */



/*----------------------------------------------------------------------

 *  WCET Benchmark created by Jakob Engblom, Uppsala university,

 *  February 2000.

 *

 *  The purpose of this benchmark is to force the compiler to emit an

 *  unstructured loop, which is usually problematic for WCET tools to

 *  handle.

 *

 *  The execution time should be constant.

 *

 *  The original code is "Duff's Device", see the Jargon File, e.g. at

 *  http://www.tf.hut.fi/cgi-bin/jargon.  Created in the early 1980s

 *  as a way to express loop unrolling in C.

 *

 *----------------------------------------------------------------------*/



#define ARRAYSIZE  100

#define INVOCATION_COUNT 43	/* exec time depends on this one! */





void duffcopy( char *to, char *from, int count)

{

  int n=(count+7)/8;

  switch(count%8){

  case 0: do{     *to++ = *from++;

  case 7:         *to++ = *from++;

  case 6:         *to++ = *from++;

  case 5:         *to++ = *from++;

  case 4:         *to++ = *from++;

  case 3:         *to++ = *from++;

  case 2:         *to++ = *from++;

  case 1:         *to++ = *from++;

  } while(--n>0);

  }

}





void initialize( char *arr, int length)

{

  int i;

  for(i=0;i<length;i++)

    {

      arr[i] = length-i;

    }

}





char source[ARRAYSIZE];

char target[ARRAYSIZE];



int benchmark(void)

{

  initialize( source, ARRAYSIZE );

  duffcopy( source, target, INVOCATION_COUNT );
  return 0;
}






/*------------------------------------------------------------

 * $Id: duff.c,v 1.2 2005/04/04 11:34:58 csg Exp $

 *------------------------------------------------------------

 * $Log: duff.c,v $
 * Revision 1.2  2005/04/04 11:34:58  csg
 * again
 *

 * Revision 1.1  2005/03/29 09:34:13  jgn

 * New file.

 *

 * Revision 1.8  2000/10/16 07:48:15  jakob

 * *** empty log message ***

 *

 * Revision 1.7  2000/05/22 11:02:18  jakob

 * Fixed minor errors.

 *

 * Revision 1.6  2000/02/27 16:56:52  jakob

 * *** empty log message ***

 *

 * Revision 1.5  2000/02/15 14:09:24  jakob

 * no message

 *

 * Revision 1.2  2000/02/15 13:32:16  jakob

 * Added duff's device to benchmark suite for LCTES 00 paper.

 *

 *

 *------------------------------------------------------------*/

