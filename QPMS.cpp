/*
* File:   QPMS.c
* Author: yeguo
*
* Created on April 5, 2015, 4:29 PM
*/

#include "QPMS.h"
#include "Rawiv-reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

const int edgeTable[256] = {
	0x0, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33, 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa, 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66, 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff, 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55, 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66, 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa, 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33, 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99, 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

const int triTable[256][16] = {
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
	{ 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
	{ 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
	{ 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
	{ 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
	{ 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
	{ 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
	{ 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
	{ 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
	{ 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
	{ 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
	{ 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
	{ 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
	{ 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
	{ 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
	{ 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
	{ 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
	{ 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
	{ 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
	{ 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
	{ 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
	{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
	{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
	{ 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
	{ 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
	{ 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
	{ 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
	{ 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
	{ 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
	{ 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
	{ 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
	{ 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
	{ 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
	{ 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
	{ 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
	{ 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
	{ 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
	{ 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
	{ 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
	{ 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
	{ 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
	{ 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
	{ 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
	{ 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
	{ 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
	{ 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
	{ 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
	{ 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
	{ 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
	{ 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
	{ 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
	{ 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
	{ 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
	{ 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
	{ 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
	{ 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
	{ 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
	{ 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
	{ 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
	{ 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
	{ 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
	{ 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
	{ 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
	{ 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
	{ 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
	{ 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
	{ 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
	{ 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
	{ 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
	{ 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
	{ 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
	{ 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
	{ 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
	{ 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
	{ 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
	{ 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
	{ 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
	{ 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
	{ 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
	{ 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
	{ 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
	{ 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
	{ 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
	{ 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
	{ 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
	{ 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
	{ 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
	{ 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
	{ 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
	{ 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
	{ 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
	{ 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
	{ 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
	{ 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
	{ 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
	{ 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
	{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
	{ 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
	{ 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
	{ 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
	{ 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
	{ 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
	{ 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
	{ 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
	{ 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
	{ 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
	{ 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
	{ 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
	{ 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

int index_of_cube(int x, int y, int z) {
	return x + y * DIM + z * DIM * DIM;
}

void initialize(POINT *vertice, TETRAHEDRON *tetra, int num_tetra) {
	for (int i = 0; i < num_tetra; i++) {
		tetra[i].p1 = &vertice[i * 4];
		tetra[i].p2 = &vertice[i * 4 + 1];
		tetra[i].p3 = &vertice[i * 4 + 2];
		tetra[i].p4 = &vertice[i * 4 + 3];

		tetra[i].a = (PLANE *)malloc(sizeof(PLANE));
		tetra[i].b = (PLANE *)malloc(sizeof(PLANE));
		tetra[i].c = (PLANE *)malloc(sizeof(PLANE));
		tetra[i].d = (PLANE *)malloc(sizeof(PLANE));

		make_plane(tetra[i].p2, tetra[i].p3, tetra[i].p4, tetra[i].p1, tetra[i].a);
		make_plane(tetra[i].p1, tetra[i].p3, tetra[i].p4, tetra[i].p2, tetra[i].b);
		make_plane(tetra[i].p1, tetra[i].p2, tetra[i].p4, tetra[i].p3, tetra[i].c);
		make_plane(tetra[i].p1, tetra[i].p2, tetra[i].p3, tetra[i].p4, tetra[i].d);
	}
}

void make_plane(const POINT *p1, const POINT *p2, const POINT *p3, const POINT *p4, PLANE *plane) {
	float x1 = p1->x, x2 = p2->x, x3 = p3->x;
	float y1 = p1->y, y2 = p2->y, y3 = p3->y;
	float z1 = p1->z, z2 = p2->z, z3 = p3->z;

	float a = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	float b = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
	float c = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	float d = -a * x1 - b * y1 - c * z1;

	if (a * (p4->x) + b * (p4->y) + c * (p4->z) + d >= 0) {
		plane->a = a;
		plane->b = b;
		plane->c = c;
		plane->d = d;
	} else {
		plane->a = -a;
		plane->b = -b;
		plane->c = -c;
		plane->d = -d;
	}
}

float dist_to_plane(const POINT *v, const PLANE *p) {
	float a = p->a, b = p->b, c = p->c, d = p->d;
	float x = v->x, y = v->y, z = v->z;
	return (a * x + b * y + c * z + d) / sqrt(a * a + b * b + c * c);
}

float dist_between_points(POINT *p1, POINT *p2) {
	float a = (p1->x - p2->x), b = (p1->y - p2->y), c = (p1->z - p2->z);
	return sqrt(a * a + b * b + c * c);
}

float volumn_tetra(TETRAHEDRON *tetra) {
	float x1 = tetra->p1->x, x2 = tetra->p2->x, x3 = tetra->p3->x, x4 = tetra->p4->x;
	float y1 = tetra->p1->y, y2 = tetra->p2->y, y3 = tetra->p3->y, y4 = tetra->p4->y;
	float z1 = tetra->p1->z, z2 = tetra->p2->z, z3 = tetra->p3->z, z4 = tetra->p4->z;
	return fabs(((x4 - x1) * ((y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1)) + (y4 - y1) * ((z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1)) + (z4 - z1) * ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1))) / 6);
}

void generate_dataset(float* dataset, TETRAHEDRON *tetra, int num_tetra, SURFACE_TYPE type) {
	int x, y, z, i;
	float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
	float max_x = FLT_MIN, max_y = FLT_MIN, max_z = FLT_MIN;
	float den;

	for (int i = 0; i < DIM * DIM * DIM; i++) {
		dataset[i] = OUTSIDE;
	}

	for (i = 0; i < num_tetra; i++) {
		min_x = fminf(min_x, fminf(fminf(fminf(tetra[i].p1->x, tetra[i].p2->x), tetra[i].p3->x), tetra[i].p4->x));
		min_y = fminf(min_y, fminf(fminf(fminf(tetra[i].p1->y, tetra[i].p2->y), tetra[i].p3->y), tetra[i].p4->y));
		min_z = fminf(min_z, fminf(fminf(fminf(tetra[i].p1->z, tetra[i].p2->z), tetra[i].p3->z), tetra[i].p4->z));

		max_x = fmaxf(max_x, fmaxf(fmaxf(fmaxf(tetra[i].p1->x, tetra[i].p2->x), tetra[i].p3->x), tetra[i].p4->x));
		max_y = fmaxf(max_y, fmaxf(fmaxf(fmaxf(tetra[i].p1->y, tetra[i].p2->y), tetra[i].p3->y), tetra[i].p4->y));
		max_z = fmaxf(max_z, fmaxf(fmaxf(fmaxf(tetra[i].p1->z, tetra[i].p2->z), tetra[i].p3->z), tetra[i].p4->z));
	}

	den = fmaxf(max_x - min_x, fmaxf(max_y - min_y, max_z - min_z)) / DIM;

	POINT *v = (POINT *)malloc(sizeof(POINT));

	TETRAHEDRON *t1 = (TETRAHEDRON *)malloc(sizeof(TETRAHEDRON));
	TETRAHEDRON *t2 = (TETRAHEDRON *)malloc(sizeof(TETRAHEDRON));
	TETRAHEDRON *t3 = (TETRAHEDRON *)malloc(sizeof(TETRAHEDRON));
	TETRAHEDRON *t4 = (TETRAHEDRON *)malloc(sizeof(TETRAHEDRON));

	float da, db, dc, dd, de, df, dg, dh;
	float d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	float r1, r2, r3, r4, length, l;
	float volumn;

	for (i = 0; i < num_tetra; i++) {

		t1->p1 = tetra[i].p2;
		t1->p2 = tetra[i].p3;
		t1->p3 = tetra[i].p4;
		t1->p4 = v;

		t2->p1 = tetra[i].p1;
		t2->p2 = tetra[i].p3;
		t2->p3 = tetra[i].p4;
		t2->p4 = v;

		t3->p1 = tetra[i].p1;
		t3->p2 = tetra[i].p2;
		t3->p3 = tetra[i].p4;
		t3->p4 = v;

		t4->p1 = tetra[i].p1;
		t4->p2 = tetra[i].p2;
		t4->p3 = tetra[i].p3;
		t4->p4 = v;

		volumn = volumn_tetra(&tetra[i]);

		d1 = dist_to_plane(tetra[i].p1, tetra[i].a);
		d2 = dist_to_plane(tetra[i].p2, tetra[i].b);
		d3 = dist_to_plane(tetra[i].p3, tetra[i].c);
		d4 = dist_to_plane(tetra[i].p4, tetra[i].d);

		d5 = dist_between_points(tetra[i].p1, tetra[i].p2);
		d6 = dist_between_points(tetra[i].p1, tetra[i].p3);
		d7 = dist_between_points(tetra[i].p1, tetra[i].p4);
		d8 = dist_between_points(tetra[i].p2, tetra[i].p3);
		d9 = dist_between_points(tetra[i].p2, tetra[i].p4);
		d10 = dist_between_points(tetra[i].p3, tetra[i].p4);

		length = fmaxf(d5, fmaxf(d6, fmaxf(d7, fmaxf(d8, fmaxf(d9, d10)))));

		TETRAHEDRON t = tetra[i];

		for (x = 0; x < DIM; x++)
			for (y = 0; y < DIM; y++)
				for (z = 0; z < DIM; z++) {
					v->x = min_x + x * den;
					v->y = min_y + y * den;
					v->z = min_z + z * den;

					da = dist_to_plane(v, tetra[i].a);
					db = dist_to_plane(v, tetra[i].b);
					dc = dist_to_plane(v, tetra[i].c);
					dd = dist_to_plane(v, tetra[i].d);

					if (da >= 0 && db >= 0 && dc >= 0 && dd >= 0) {
						da = da / d1 * 2 * PI;
						db = db / d2 * 2 * PI;
						dc = dc / d3 * 2 * PI;
						dd = dd / d4 * 2 * PI;

						de = dist_between_points(v, tetra[i].p1);
						df = dist_between_points(v, tetra[i].p2);
						dg = dist_between_points(v, tetra[i].p3);
						dh = dist_between_points(v, tetra[i].p4);

						l = fminf(de, fminf(df, fminf(dg, dh)));

						r1 = volumn_tetra(t1) / volumn;
						r2 = volumn_tetra(t2) / volumn;
						r3 = volumn_tetra(t3) / volumn;
						r4 = volumn_tetra(t4) / volumn;

						switch (type) {
						case P:
							//dataset[index_of_cube(x, y, z)] = cos(da) + cos(db) + cos(dc) + cos(dd) + cos(da) * cos(db) * cos(dc) * cos(dd);
							//dataset[index_of_cube(x, y, z)] = r1 * cos(de) + r2 * cos(df) + r3 * cos(dg) + r4 * cos(dh);
							//dataset[index_of_cube(x, y, z)] = cos(da)*(2 * PI - da) / (2 * PI) + cos(db)*(2 * PI - db) / (2 * PI) + cos(dc)*(2 * PI - dc) / (2 * PI) + cos(dd)*(2 * PI - dd) / (2 * PI);
							//dataset[index_of_cube(x, y, z)] = cos(da)*(da) / (2 * PI) + cos(db)*(db) / (2 * PI) + cos(dc)*(dc) / (2 * PI) + cos(dd)*(dd) / (2 * PI);
							dataset[index_of_cube(x, y, z)] = cos(l / length * 2 * PI);
							break;
						case D:
							dataset[index_of_cube(x, y, z)] = sin(da) * cos(db)	* cos(dc) * cos(dd) + cos(da) * sin(db)	* cos(dc) * cos(dd) + cos(da) * cos(db)	* sin(dc) * cos(dd) + cos(da) * cos(db)	* cos(dc) * sin(dd) + cos(da) * cos(db)	* cos(dc) * cos(dd);
							break;
						case G:
							dataset[index_of_cube(x, y, z)] = cos(da) * sin(db) + cos(db) * sin(dc) + cos(dc) * sin(dd) + cos(dd) * sin(da) + cos(da) * cos(db)	* cos(dc) * cos(dd);
							break;
							/*
							case I_WP:
							dataset[index_of_cube(x, y, z)] = 2 * (cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							+ cos(dist_to_plane(v, b)) * cos(dist_to_plane(v, c))
							//+ cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, c))
							//+ cos(dist_to_plane(v, b)) * cos(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, d)) * cos(dist_to_plane(v, a)))
							+ cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							* cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							- cos(2 * dist_to_plane(v, a)) - cos(2 * dist_to_plane(v, b))
							- cos(2 * dist_to_plane(v, c)) - cos(2 * dist_to_plane(v, d));
							break;
							case N:
							dataset[index_of_cube(x, y, z)] = 4 * (cos(dist_to_plane(v, a)) + cos(dist_to_plane(v, b))
							+ cos(dist_to_plane(v, c)) + cos(dist_to_plane(v, d)))
							+ 5 * cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							* cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d));
							break;
							case W:
							dataset[index_of_cube(x, y, z)] = cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							+ cos(dist_to_plane(v, b)) * cos(dist_to_plane(v, c))
							+ cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, d)) * cos(dist_to_plane(v, a))
							+ cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							* cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d));
							break;
							case P_W:
							dataset[index_of_cube(x, y, z)] = 5 * (cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							+ cos(dist_to_plane(v, b)) * cos(dist_to_plane(v, c))
							+ cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, d)) * cos(dist_to_plane(v, a)))
							- 4 * cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							* cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d));
							break;
							case T_1:
							dataset[index_of_cube(x, y, z)] = sin(dist_to_plane(v, a)) + sin(dist_to_plane(v, b))
							+ sin(dist_to_plane(v, c)) + sin(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							* cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d));
							break;
							case T_2:
							dataset[index_of_cube(x, y, z)] = cos(dist_to_plane(v, a)) * cos(dist_to_plane(v, b)) * cos(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, a)) * sin(dist_to_plane(v, b)) * sin(dist_to_plane(v, c)) * sin(dist_to_plane(v, d))
							+ sin(dist_to_plane(v, a)) * cos(dist_to_plane(v, b)) * sin(dist_to_plane(v, c)) * sin(dist_to_plane(v, d))
							+ sin(dist_to_plane(v, a)) * sin(dist_to_plane(v, b)) * cos(dist_to_plane(v, c)) * sin(dist_to_plane(v, d))
							+ sin(dist_to_plane(v, a)) * sin(dist_to_plane(v, b)) * sin(dist_to_plane(v, c)) * cos(dist_to_plane(v, d));
							break;
							case T_3:
							dataset[index_of_cube(x, y, z)] = cos(dist_to_plane(v, a)) * sin(dist_to_plane(v, b))
							+ cos(dist_to_plane(v, b)) * sin(dist_to_plane(v, c))
							+ cos(dist_to_plane(v, c)) * sin(dist_to_plane(v, d))
							+ cos(dist_to_plane(v, d)) * sin(dist_to_plane(v, a))
							+ sin(dist_to_plane(v, a)) * sin(dist_to_plane(v, b))
							* sin(dist_to_plane(v, c)) * sin(dist_to_plane(v, d));
							break;
							case T:
							dataset[index_of_cube(x, y, z)] = sin(dist_to_plane(v, a)) * cos(dist_to_plane(v, b))
							+ sin(dist_to_plane(v, b)) * cos(dist_to_plane(v, c))
							+ sin(dist_to_plane(v, c)) * cos(dist_to_plane(v, d))
							+ sin(dist_to_plane(v, d)) * cos(dist_to_plane(v, a))
							+ sin(dist_to_plane(v, a)) * sin(dist_to_plane(v, b))
							* sin(dist_to_plane(v, c)) * sin(dist_to_plane(v, d));
							break;
							*/
						}
					}
				}
	}

	free(t1);
	free(t2);
	free(t3);
	free(t4);

	free(v);
}

void wirteOFF(SURFACEMESH *surface, char *name) {
	FILE *fp;
	fp = fopen(name, "w+");

	fprintf(fp, "OFF\n");
	fprintf(fp, "%d %d 0\n", surface->nv, surface->nf);

	int n;
	for (n = 0; n < surface->nv; n++)
		fprintf(fp, "%f %f %f\n", surface->vertice[n].x, surface->vertice[n].y, surface->vertice[n].z);

	for (n = 0; n < surface->nf; n++)
		fprintf(fp, "3 %d %d %d\n", surface->triangles[n].a, surface->triangles[n].b, surface->triangles[n].c);

	fclose(fp);
}

SURFACEMESH* surfaceMesh_marchingCube(float* dataset, float isovalue) {
	int tempt_x, tempt_y, tempt_z;
	int i, j, k, ii;
	int v_num, t_num;
	float den1, den2, ratio;
	POINT *vertex;
	TRIANGLE *triangle, *mc_edge;
	int cellVerts[12];
	unsigned char cellIndex;
	unsigned char *mc_sign;
	SURFACEMESH *surfmesh;

	vertex = (POINT*)malloc(sizeof(POINT) * DIM * DIM * DIM);
	triangle = (TRIANGLE*)malloc(sizeof(TRIANGLE) * DIM * DIM * DIM);
	v_num = 0;
	t_num = 0;

	mc_edge = (TRIANGLE*)malloc(sizeof(TRIANGLE) * DIM * DIM * DIM);
	mc_sign = (unsigned char*)malloc(sizeof(unsigned char) * DIM * DIM * DIM);

	for (k = 0; k < DIM; k++)
		for (j = 0; j < DIM; j++)
			for (i = 0; i < DIM; i++) {
				if (dataset[index_of_cube(i, j, k)] > isovalue - 0.0001 &&
					dataset[index_of_cube(i, j, k)] < isovalue + 0.0001)
					dataset[index_of_cube(i, j, k)] = isovalue + 0.0001;

				if (dataset[index_of_cube(i, j, k)] >= OUTSIDE - 10)
					mc_sign[index_of_cube(i, j, k)] = 50;
				else if (dataset[index_of_cube(i, j, k)] >= isovalue)
					mc_sign[index_of_cube(i, j, k)] = 1;
				else
					mc_sign[index_of_cube(i, j, k)] = 255;

				mc_edge[index_of_cube(i, j, k)].a = -1;
				mc_edge[index_of_cube(i, j, k)].b = -1;
				mc_edge[index_of_cube(i, j, k)].c = -1;
			}

	int inside = 0;
	int total = 0;

	for (tempt_z = 0; tempt_z < DIM - 1; tempt_z++)
		for (tempt_y = 0; tempt_y < DIM - 1; tempt_y++)
			for (tempt_x = 0; tempt_x < DIM - 1; tempt_x++) {

				total++;

				if (mc_sign[index_of_cube(tempt_x, tempt_y, tempt_z)] == 255)
					inside++;

				for (ii = 0; ii < 12; ii++)
					cellVerts[ii] = -1;

				cellIndex = 0;

				if (mc_sign[index_of_cube(tempt_x, tempt_y, tempt_z)] == 50 || mc_sign[index_of_cube(tempt_x, tempt_y + 1, tempt_z)] == 50
					|| mc_sign[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)] == 50 || mc_sign[index_of_cube(tempt_x + 1, tempt_y, tempt_z)] == 50
					|| mc_sign[index_of_cube(tempt_x, tempt_y, tempt_z + 1)] == 50 || mc_sign[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)] == 50
					|| mc_sign[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z + 1)] == 50 || mc_sign[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)] == 50)
					continue;

				if (mc_sign[index_of_cube(tempt_x, tempt_y, tempt_z)] == 255)
					cellIndex |= 1;

				if (mc_sign[index_of_cube(tempt_x, tempt_y + 1, tempt_z)] == 255)
					cellIndex |= 2;

				if (mc_sign[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)] == 255)
					cellIndex |= 4;

				if (mc_sign[index_of_cube(tempt_x + 1, tempt_y, tempt_z)] == 255)
					cellIndex |= 8;

				if (mc_sign[index_of_cube(tempt_x, tempt_y, tempt_z + 1)] == 255)
					cellIndex |= 16;

				if (mc_sign[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)] == 255)
					cellIndex |= 32;

				if (mc_sign[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z + 1)] == 255)
					cellIndex |= 64;

				if (mc_sign[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)] == 255)
					cellIndex |= 128;

				if (edgeTable[cellIndex] & 1) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].b == -1) {
						den1 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x;
						vertex[v_num].y = tempt_y + ratio;
						vertex[v_num].z = tempt_z;
						cellVerts[0] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].b = v_num;
						v_num++;
					}
					else
						cellVerts[0] = mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].b;
				}

				if (edgeTable[cellIndex] & 2) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].a == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + ratio;
						vertex[v_num].y = tempt_y + 1;
						vertex[v_num].z = tempt_z;
						cellVerts[1] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].a = v_num;
						v_num++;
					}
					else
						cellVerts[1] = mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].a;
				}

				if (edgeTable[cellIndex] & 4) {
					if (mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].b == -1) {

						den1 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + 1;
						vertex[v_num].y = tempt_y + ratio;
						vertex[v_num].z = tempt_z;
						cellVerts[2] = v_num;
						mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].b = v_num;
						v_num++;
					}
					else
						cellVerts[2] = mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].b;
				}

				if (edgeTable[cellIndex] & 8) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].a == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + ratio;
						vertex[v_num].y = tempt_y;
						vertex[v_num].z = tempt_z;
						cellVerts[3] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].a = v_num;
						v_num++;
					}
					else
						cellVerts[3] = mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].a;
				}

				if (edgeTable[cellIndex] & 16) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].b == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z + 1)];
						den2 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x;
						vertex[v_num].y = tempt_y + ratio;
						vertex[v_num].z = tempt_z + 1;
						cellVerts[4] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].b = v_num;
						v_num++;
					}
					else
						cellVerts[4] = mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].b;
				}

				if (edgeTable[cellIndex] & 32) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)].a == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + ratio;
						vertex[v_num].y = tempt_y + 1;
						vertex[v_num].z = tempt_z + 1;
						cellVerts[5] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)].a = v_num;
						v_num++;
					}
					else
						cellVerts[5] = mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)].a;
				}

				if (edgeTable[cellIndex] & 64) {
					if (mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)].b == -1) {

						den1 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + 1;
						vertex[v_num].y = tempt_y + ratio;
						vertex[v_num].z = tempt_z + 1;
						cellVerts[6] = v_num;
						mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)].b = v_num;
						v_num++;
					}
					else
						cellVerts[6] = mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)].b;
				}

				if (edgeTable[cellIndex] & 128) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].a == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z + 1)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + ratio;
						vertex[v_num].y = tempt_y;
						vertex[v_num].z = tempt_z + 1;
						cellVerts[7] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].a = v_num;
						v_num++;
					}
					else
						cellVerts[7] = mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z + 1)].a;
				}

				if (edgeTable[cellIndex] & 256) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].c == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x, tempt_y, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x;
						vertex[v_num].y = tempt_y;
						vertex[v_num].z = tempt_z + ratio;
						cellVerts[8] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].c = v_num;
						v_num++;
					}
					else
						cellVerts[8] = mc_edge[index_of_cube(tempt_x, tempt_y, tempt_z)].c;
				}

				if (edgeTable[cellIndex] & 512) {
					if (mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].c == -1) {

						den1 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x, tempt_y + 1, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x;
						vertex[v_num].y = tempt_y + 1;
						vertex[v_num].z = tempt_z + ratio;
						cellVerts[9] = v_num;
						mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].c = v_num;
						v_num++;
					}
					else
						cellVerts[9] = mc_edge[index_of_cube(tempt_x, tempt_y + 1, tempt_z)].c;
				}

				if (edgeTable[cellIndex] & 1024) {
					if (mc_edge[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)].c == -1) {

						den1 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + 1;
						vertex[v_num].y = tempt_y + 1;
						vertex[v_num].z = tempt_z + ratio;
						cellVerts[10] = v_num;
						mc_edge[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)].c = v_num;
						v_num++;
					}
					else
						cellVerts[10] = mc_edge[index_of_cube(tempt_x + 1, tempt_y + 1, tempt_z)].c;
				}

				if (edgeTable[cellIndex] & 2048) {
					if (mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].c == -1) {

						den1 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z)];
						den2 = dataset[index_of_cube(tempt_x + 1, tempt_y, tempt_z + 1)];

						if (den1 != den2)
							ratio = (isovalue - den1) / (den2 - den1);
						else
							ratio = 0;

						vertex[v_num].x = tempt_x + 1;
						vertex[v_num].y = tempt_y;
						vertex[v_num].z = tempt_z + ratio;
						cellVerts[11] = v_num;
						mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].c = v_num;
						v_num++;
					}
					else
						cellVerts[11] = mc_edge[index_of_cube(tempt_x + 1, tempt_y, tempt_z)].c;
				}

				ii = 0;
				while (triTable[cellIndex][ii] != -1) {
					triangle[t_num].a = cellVerts[triTable[cellIndex][ii++]];
					triangle[t_num].b = cellVerts[triTable[cellIndex][ii++]];
					triangle[t_num].c = cellVerts[triTable[cellIndex][ii++]];
					t_num++;
				}
			}

	float porosity = inside / total;

	// Allocate memory
	surfmesh = (SURFACEMESH *)malloc(sizeof(SURFACEMESH));

	surfmesh->nv = v_num;
	surfmesh->nf = t_num;
	surfmesh->vertice = vertex;
	surfmesh->triangles = triangle;

	free(mc_edge);
	free(mc_sign);

	// Return created SurfaceMesh
	return surfmesh;
}

void writeRawiv(float* dataset, char* name) {
	FILE *fp;
	fp = fopen(name, "wb");

	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * DIM * DIM * DIM);

	for (int i = 0; i < DIM * DIM * DIM; i++) {
		data[i] = (unsigned char)dataset[i];
	}

	write_rawiv_short(DIM, DIM, DIM, data, fp);
	//writeRawivText(data, fp);

	fclose(fp);
}

void scaleDataset(float *dataset) {
	float min = FLT_MAX, max = FLT_MIN;

	for (int i = 0; i < DIM * DIM * DIM; i++) {
		if (dataset[i] > OUTSIDE - 10)
			continue;
		if (dataset[i] < min)
			min = dataset[i];
		if (dataset[i] > max)
			max = dataset[i];
	}

	float length = max - min;

	for (int i = 0; i < DIM * DIM * DIM; i++) {
		if (dataset[i] > OUTSIDE - 10)
			dataset[i] = 255;
		else
			dataset[i] = (dataset[i] - min) / length * 255;
	}
}