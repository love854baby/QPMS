#pragma once

#define PI 3.14159265358979323846
#define SQRT_TWO 1.41421356237
#define SQRT_TREEE 1.73205080757

#define DIM 200 // Number of the cells on each dimension
#define PD 1  // Max periods from three dimensions

typedef enum {
	P, D, G
} SURFACE_TYPE;

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

void initWithVers(POINT *vertice, TETRAHEDRON *tetra, int size);
void init(TETRAHEDRON *tetra, int size);
float* generateDataset(TETRAHEDRON *tetra, SURFACE_TYPE type, int size);
SURFACEMESH* marchingCube(float* dataset, float isovalue);
void wirteOFF(SURFACEMESH *surface, char *name);
void writeRawiv(float *dataset, char* name);
float convertThres(float *dataset, int thres);
TETRAHEDRON* readTet(char *name, int *size);
