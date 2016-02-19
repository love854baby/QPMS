#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _LITTLE_ENDIAN   1

#define IndexVect(i,j,k) ((k)*xdim*ydim + (j)*xdim + (i))

float minext[3], maxext[3];
int nverts, ncells;
unsigned int dim[3];
float orig[3], span[3];
float maxraw;
float minraw;


/*
* ***************************************************************************
* Routine:  swap_buffer    < ... >
*
* Purpose:  swap the bytes when LITTLE_ENDIAN is enabled
* ***************************************************************************
*/
void swap_buffer(char *buffer, int count, int typesize)
{
	char sbuf[4];
	int i;
	int temp = 1;
	unsigned char* chartempf = (unsigned char*)&temp;
	if (chartempf[0] > '\0') {

		// swapping isn't necessary on single byte data
		if (typesize == 1)
			return;


		for (i = 0; i < count; i++)
		{
			memcpy(sbuf, buffer + (i*typesize), typesize);

			switch (typesize)
			{
			case 2:
			{
				buffer[i*typesize] = sbuf[1];
				buffer[i*typesize + 1] = sbuf[0];
				break;
			}
			case 4:
			{
				buffer[i*typesize] = sbuf[3];
				buffer[i*typesize + 1] = sbuf[2];
				buffer[i*typesize + 2] = sbuf[1];
				buffer[i*typesize + 3] = sbuf[0];
				break;
			}
			default:
				break;
			}
		}

	}
}

// to write rawiv as float, please change c_short to c_float and make it as a "float" type
void write_rawiv_short(int xdim, int ydim, int zdim, unsigned char *result, FILE* fp)
{
	int i, j, k;
	unsigned char c_short;

	dim[0] = xdim;
	dim[1] = ydim;
	dim[2] = zdim;

	minext[0] = 0;
	minext[1] = 0;
	minext[2] = 0;

	maxext[0] = xdim - 1;
	maxext[1] = ydim - 1;
	maxext[2] = zdim - 1;

	nverts = xdim * ydim * zdim;

	ncells = (xdim - 1) * (ydim - 1) * (zdim - 1);

	orig[0] = 0;
	orig[1] = 0;
	orig[2] = 0;

	span[0] = 1;
	span[1] = 1;
	span[2] = 1;

#ifdef _LITTLE_ENDIAN
	swap_buffer((char *)minext, 3, sizeof(float));
	swap_buffer((char *)maxext, 3, sizeof(float));
	swap_buffer((char *)&nverts, 1, sizeof(int));
	swap_buffer((char *)&ncells, 1, sizeof(int));
	swap_buffer((char *)dim, 3, sizeof(unsigned int));
	swap_buffer((char *)orig, 3, sizeof(float));
	swap_buffer((char *)span, 3, sizeof(float));
#endif 

	fwrite(minext, sizeof(float), 3, fp);
	fwrite(maxext, sizeof(float), 3, fp);
	fwrite(&nverts, sizeof(int), 1, fp);
	fwrite(&ncells, sizeof(int), 1, fp);
	fwrite(dim, sizeof(unsigned int), 3, fp);
	fwrite(orig, sizeof(float), 3, fp);
	fwrite(span, sizeof(float), 3, fp);

#ifdef _LITTLE_ENDIAN
	swap_buffer((char *)minext, 3, sizeof(float));
	swap_buffer((char *)maxext, 3, sizeof(float));
	swap_buffer((char *)&nverts, 1, sizeof(int));
	swap_buffer((char *)&ncells, 1, sizeof(int));
	swap_buffer((char *)dim, 3, sizeof(unsigned int));
	swap_buffer((char *)orig, 3, sizeof(float));
	swap_buffer((char *)span, 3, sizeof(float));
#endif 

	for (k = 0; k < zdim; k++)
		for (j = 0; j < ydim; j++)
			for (i = 0; i < xdim; i++) {
				c_short = result[IndexVect(i, j, k)];

				fwrite(&c_short, sizeof(unsigned char), 1, fp);

			}

}