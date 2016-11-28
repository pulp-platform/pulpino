/* BEEBS cover benchmark

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

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

int swi120(int c)
{
	int i;
	for (i=0; i<120; i++) {
		switch (i) {
			case 0: c++; break;
			case 1: c++; break;
			case 2: c++; break;
			case 3: c++; break;
			case 4: c++; break;
			case 5: c++; break;
			case 6: c++; break;
			case 7: c++; break;
			case 8: c++; break;
			case 9: c++; break;
			case 10: c++; break;
			case 11: c++; break;
			case 12: c++; break;
			case 13: c++; break;
			case 14: c++; break;
			case 15: c++; break;
			case 16: c++; break;
			case 17: c++; break;
			case 18: c++; break;
			case 19: c++; break;
			case 20: c++; break;
			case 21: c++; break;
			case 22: c++; break;
			case 23: c++; break;
			case 24: c++; break;
			case 25: c++; break;
			case 26: c++; break;
			case 27: c++; break;
			case 28: c++; break;
			case 29: c++; break;
			case 30: c++; break;
			case 31: c++; break;
			case 32: c++; break;
			case 33: c++; break;
			case 34: c++; break;
			case 35: c++; break;
			case 36: c++; break;
			case 37: c++; break;
			case 38: c++; break;
			case 39: c++; break;
			case 40: c++; break;
			case 41: c++; break;
			case 42: c++; break;
			case 43: c++; break;
			case 44: c++; break;
			case 45: c++; break;
			case 46: c++; break;
			case 47: c++; break;
			case 48: c++; break;
			case 49: c++; break;
			case 50: c++; break;
			case 51: c++; break;
			case 52: c++; break;
			case 53: c++; break;
			case 54: c++; break;
			case 55: c++; break;
			case 56: c++; break;
			case 57: c++; break;
			case 58: c++; break;
			case 59: c++; break;
			case 60: c++; break;
			case 61: c++; break;
			case 62: c++; break;
			case 63: c++; break;
			case 64: c++; break;
			case 65: c++; break;
			case 66: c++; break;
			case 67: c++; break;
			case 68: c++; break;
			case 69: c++; break;
			case 70: c++; break;
			case 71: c++; break;
			case 72: c++; break;
			case 73: c++; break;
			case 74: c++; break;
			case 75: c++; break;
			case 76: c++; break;
			case 77: c++; break;
			case 78: c++; break;
			case 79: c++; break;
			case 80: c++; break;
			case 81: c++; break;
			case 82: c++; break;
			case 83: c++; break;
			case 84: c++; break;
			case 85: c++; break;
			case 86: c++; break;
			case 87: c++; break;
			case 88: c++; break;
			case 89: c++; break;
			case 90: c++; break;
			case 91: c++; break;
			case 92: c++; break;
			case 93: c++; break;
			case 94: c++; break;
			case 95: c++; break;
			case 96: c++; break;
			case 97: c++; break;
			case 98: c++; break;
			case 99: c++; break;
			case 100: c++; break;
			case 101: c++; break;
			case 102: c++; break;
			case 103: c++; break;
			case 104: c++; break;
			case 105: c++; break;
			case 106: c++; break;
			case 107: c++; break;
			case 108: c++; break;
			case 109: c++; break;
			case 110: c++; break;
			case 111: c++; break;
			case 112: c++; break;
			case 113: c++; break;
			case 114: c++; break;
			case 115: c++; break;
			case 116: c++; break;
			case 117: c++; break;
			case 118: c++; break;
			case 119: c++; break;
			default: c--; break;
		}
	}
	return c;
}


int swi50(int c)
{
	int i;
	for (i=0; i<50; i++) {
		switch (i) {
			case 0: c++; break;
			case 1: c++; break;
			case 2: c++; break;
			case 3: c++; break;
			case 4: c++; break;
			case 5: c++; break;
			case 6: c++; break;
			case 7: c++; break;
			case 8: c++; break;
			case 9: c++; break;
			case 10: c++; break;
			case 11: c++; break;
			case 12: c++; break;
			case 13: c++; break;
			case 14: c++; break;
			case 15: c++; break;
			case 16: c++; break;
			case 17: c++; break;
			case 18: c++; break;
			case 19: c++; break;
			case 20: c++; break;
			case 21: c++; break;
			case 22: c++; break;
			case 23: c++; break;
			case 24: c++; break;
			case 25: c++; break;
			case 26: c++; break;
			case 27: c++; break;
			case 28: c++; break;
			case 29: c++; break;
			case 30: c++; break;
			case 31: c++; break;
			case 32: c++; break;
			case 33: c++; break;
			case 34: c++; break;
			case 35: c++; break;
			case 36: c++; break;
			case 37: c++; break;
			case 38: c++; break;
			case 39: c++; break;
			case 40: c++; break;
			case 41: c++; break;
			case 42: c++; break;
			case 43: c++; break;
			case 44: c++; break;
			case 45: c++; break;
			case 46: c++; break;
			case 47: c++; break;
			case 48: c++; break;
			case 49: c++; break;
			case 50: c++; break;
			case 51: c++; break;
			case 52: c++; break;
			case 53: c++; break;
			case 54: c++; break;
			case 55: c++; break;
			case 56: c++; break;
			case 57: c++; break;
			case 58: c++; break;
			case 59: c++; break;
			default: c--; break;
		}
	}
	return c;
}


int swi10(int c)
{
	int i;
	for (i=0; i<10; i++) {
		switch (i) {
			case 0: c++; break;
			case 1: c++; break;
			case 2: c++; break;
			case 3: c++; break;
			case 4: c++; break;
			case 5: c++; break;
			case 6: c++; break;
			case 7: c++; break;
			case 8: c++; break;
			case 9: c++; break;
			default: c--; break;
		}
	}
	return c;
}

int benchmark() 
{
	volatile int cnt=0;

	cnt=swi10(cnt);
	cnt=swi50(cnt);
	cnt=swi120(cnt);

	/* printf("cnt: %d\n", cnt); */

	return cnt;

}

