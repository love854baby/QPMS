/*
* File:   QPMS.h
* Author: yeguo
*
* Created on April 5, 2015, 4:29 PM
*/

#include <stdio.h>

#define PI 3.14159265358979323846
#define SQRT_TWO 1.41421356237
#define SQRT_TREEE 1.73205080757

#define DIM 500 // Number of the cells on each dimension
#define OUTSIDE 1000

typedef enum {
	P, D, G, I_WP, N, W, P_W, T, T_1, T_2, T_3
} SURFACE_TYPE;

extern const int edgeTable[256];
extern const int triTable[256][16];

//Data structure POINT_COORDINATE
typedef struct {
	float x; //x-coordinate
	float y; //y-coordinate
	float z; //z-coordinate
} POINT;

typedef struct {
	int a;
	int b;
	int c;
} TRIANGLE;

//Plane equation in format of ax + by + cz + d = 0
typedef struct {
	float a; //parameter of variable x
	float b; //parameter of variable y
	float c; //parameter of variable z
	float d; //constant d
} PLANE;

typedef struct {
	POINT *p1, *p2, *p3, *p4;
	PLANE *a, *b, *c, *d;
} TETRAHEDRON;

//Data structure SurfaceMesh (for surface meshes)
typedef struct {
	int nv; //number of vertices
	int nf; //number of triangles
	POINT* vertice; //pointer to the vertices
	TRIANGLE* triangles; //pointer to the triangles
} SURFACEMESH;

int index_of_cube(int i, int j, int k);
void initialize(POINT *vertice, TETRAHEDRON *tetra, int num_tetra);
void make_plane(const POINT *p1, const POINT *p2, const POINT *p3, const POINT *p4, PLANE *plane);
float dist_to_plane(const POINT *v, const PLANE *p);
void generate_dataset(float *dataset, TETRAHEDRON *tetra, int num_tetra, SURFACE_TYPE type);
SURFACEMESH* surfaceMesh_marchingCube(float* dataset, float isovalue);
void wirteOFF(SURFACEMESH *surface, char *name);
float dist_between_points(POINT *p1, POINT *p2);
float volumn_tetra(TETRAHEDRON *tetra);
void writeRawiv(float *dataset, char* name);
void scaleDataset(float *dataset);