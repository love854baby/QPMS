/*
* File:   main.c
* Author: yeguo
*
* Created on April 5, 2015, 4:28 PM
*/

#include "QPMS.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>

int main() {

	int num_Tetra = 20;

	POINT *vertice = (POINT *)malloc(sizeof(POINT) * 4 * num_Tetra);

	float t = (1 + sqrt(5)) / 2;
	float t2 = 1 / sqrt(1 + t * t) * 100;

	POINT v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;

	v0.x = t * t2;
	v0.y = t2;
	v0.z = 0;

	v1.x = -t * t2;
	v1.y = t2;
	v1.z = 0;

	v2.x = t2 * t;
	v2.y = -t2;
	v2.z = 0;

	v3.x = -t2 * t;
	v3.y = -t2;
	v3.z = 0;

	v4.x = t2;
	v4.y = 0;
	v4.z = t * t2;

	v5.x = t2;
	v5.y = 0;
	v5.z = -t * t2;

	v6.x = -t2;
	v6.y = 0;
	v6.z = t2 * t;

	v7.x = -t2;
	v7.y = 0;
	v7.z = -t2 * t;

	v8.x = 0;
	v8.y = t2 * t;
	v8.z = t2;

	v9.x = 0;
	v9.y = -t2 * t;
	v9.z = t2;

	v10.x = 0;
	v10.y = t2 * t;
	v10.z = -t2;

	v11.x = 0;
	v11.y = -t2 * t;
	v11.z = -t2;

	v12.x = 0;
	v12.y = 0;
	v12.z = 0;

	vertice[0] = v0;
	vertice[1] = v8;
	vertice[2] = v4;
	vertice[3] = v12;

	vertice[4] = v0;
	vertice[5] = v5;
	vertice[6] = v10;
	vertice[7] = v12;

	vertice[8] = v2;
	vertice[9] = v4;
	vertice[10] = v9;
	vertice[11] = v12;

	vertice[12] = v2;
	vertice[13] = v11;
	vertice[14] = v5;
	vertice[15] = v12;

	vertice[16] = v1;
	vertice[17] = v6;
	vertice[18] = v8;
	vertice[19] = v12;

	vertice[20] = v1;
	vertice[21] = v10;
	vertice[22] = v7;
	vertice[23] = v12;

	vertice[24] = v3;
	vertice[25] = v9;
	vertice[26] = v6;
	vertice[27] = v12;

	vertice[28] = v3;
	vertice[29] = v7;
	vertice[30] = v11;
	vertice[31] = v12;

	vertice[32] = v0;
	vertice[33] = v10;
	vertice[34] = v8;
	vertice[35] = v12;

	vertice[36] = v1;
	vertice[37] = v8;
	vertice[38] = v10;
	vertice[39] = v12;

	vertice[40] = v2;
	vertice[41] = v9;
	vertice[42] = v11;
	vertice[43] = v12;

	vertice[44] = v3;
	vertice[45] = v9;
	vertice[46] = v11;
	vertice[47] = v12;

	vertice[48] = v4;
	vertice[49] = v2;
	vertice[50] = v0;
	vertice[51] = v12;

	vertice[52] = v5;
	vertice[53] = v0;
	vertice[54] = v2;
	vertice[55] = v12;

	vertice[56] = v6;
	vertice[57] = v1;
	vertice[58] = v3;
	vertice[59] = v12;

	vertice[60] = v7;
	vertice[61] = v3;
	vertice[62] = v1;
	vertice[63] = v12;

	vertice[64] = v8;
	vertice[65] = v6;
	vertice[66] = v4;
	vertice[67] = v12;

	vertice[68] = v9;
	vertice[69] = v4;
	vertice[70] = v6;
	vertice[71] = v12;

	vertice[72] = v10;
	vertice[73] = v5;
	vertice[74] = v7;
	vertice[75] = v12;

	vertice[76] = v11;
	vertice[77] = v7;
	vertice[78] = v5;
	vertice[79] = v12;

	/*
	vertice[0].x = 1;
	vertice[0].y = 0;
	vertice[0].z = 0;

	vertice[1].x = -1;
	vertice[1].y = 0;
	vertice[1].z = 0;

	vertice[2].x = 0;
	vertice[2].y = SQRT_TREEE;
	vertice[2].z = 0;

	vertice[3].x = 0;
	vertice[3].y = SQRT_TREEE / 3;
	vertice[3].z = 2 * SQRT_TWO / SQRT_TREEE;

	vertice[4].x = 1;
	vertice[4].y = 0;
	vertice[4].z = 0;

	vertice[5].x = -1;
	vertice[5].y = 0;
	vertice[5].z = 0;

	vertice[6].x = 0;
	vertice[6].y = SQRT_TREEE;
	vertice[6].z = 0;

	vertice[7].x = 0;
	vertice[7].y = SQRT_TREEE / 3;
	vertice[7].z = -2 * SQRT_TWO / SQRT_TREEE;
	*/

	/*
	vertice[0].x = 1;
	vertice[0].y = 1;
	vertice[0].z = 1;

	vertice[1].x = -1;
	vertice[1].y = 1;
	vertice[1].z = -1;

	vertice[2].x = 1;
	vertice[2].y = -1;
	vertice[2].z = -1;

	vertice[3].x = -1;
	vertice[3].y = -1;
	vertice[3].z = 1;
	*/

	/*
	//Top tetrahedron's fourth point
	vertice[0].x = sqrt(6) * PI / 2 * 2;
	vertice[0].y = sqrt(2) * PI / 2 * 2;
	vertice[0].z = 2 * PI;

	vertice[1].x = 0;
	vertice[1].y = 0;
	vertice[1].z = 0;

	vertice[2].x = sqrt(6) * PI;
	vertice[2].y = 0;
	vertice[2].z = 0;

	vertice[3].x = sqrt(6) * PI / 2;
	vertice[3].y = sqrt(2) * 3 * PI / 2;
	vertice[3].z = 0;

	vertice[4].x = 0;
	vertice[4].y = 0;
	vertice[4].z = 0;

	vertice[5].x = sqrt(6) * PI;
	vertice[5].y = 0;
	vertice[5].z = 0;

	vertice[6].x = sqrt(6) * PI / 2;
	vertice[6].y = sqrt(2) * 3 * PI / 2;
	vertice[6].z = 0;

	//Bottom tetrahedron's fourth point
	vertice[7].x = sqrt(6) * PI / 2 * 2;
	vertice[7].y = sqrt(2) * PI / 2 * 2;
	vertice[7].z = -2 * PI;
	*/

	float *array = (float *)malloc(sizeof(float)* DIM * DIM * DIM);

	//Set name of output file here
	char *name = "test2.off";
	//char* name = "pMesh2.off";

	//char *rawiv_name = "test.rawiv";

	//Set surface type here
	SURFACE_TYPE type = P;

	TETRAHEDRON *tetra = (TETRAHEDRON *)malloc(sizeof(TETRAHEDRON) * num_Tetra);

	initialize(vertice, tetra, num_Tetra);

	generate_dataset(array, tetra, num_Tetra, type);

	float min = FLT_MAX, max = FLT_MIN;

	for (int i = 0; i < DIM * DIM * DIM; i++) {
		if (array[i] >= OUTSIDE - 10)
			continue;
		if (array[i] < min)
			min = array[i];
		if (array[i] > max)
			max = array[i];
	}

	//Set threshold here
	int thres_in_rawiv = 88;

	float thres = min + (max - min) * thres_in_rawiv / 255;
	
	//thres = 0.1;

	clock_t start = clock(), diff;

	//Set threshold value here
	SURFACEMESH *surfmesh = (SURFACEMESH *)surfaceMesh_marchingCube(array, thres);

	diff = clock() - start;

	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);

	printf("Number of triangle: %d\n", surfmesh->nf);

	wirteOFF(surfmesh, name);

	//scaleDataset(array);

	//writeRawiv(array, rawiv_name);

	free(surfmesh->vertice);
	free(surfmesh->triangles);
	free(surfmesh);
	free(array);

	return 0;
}