#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>

#ifndef _MESH_H
#define _MESH_H

#include "Primitive.h"

class Node;

using namespace glm;
using namespace std;

class Triangle {
public:
	vec3 pt1;
	vec3 pt2;
	vec3 pt3;
	vec3 normal;

	Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 norm) {
		pt1 = point1;
		pt2 = point2;
		pt3 = point3;
		normal = norm;
	}

	Triangle() {
		pt1 = vec3();
		pt2 = vec3();
		pt3 = vec3();
		normal = vec3();
	}
};

class Mesh {
public:
	Node *node;

	glm::mat4 transform;

	float *vertices;
	float *colors;
	float *normals;
	unsigned short *indices;
	
	int numVertices;
	int numColors;
	int numNormals;
	int numIndices;

	char *filenameDAT;
	char *filenameBMP;

	Sphere *boundingSphere;
	Cube *boundingCube;

	std::vector<glm::vec3> points;	
	std::vector<vec3> vertVec;
	std::vector<vec3> normVec;
	std::vector<vec3> indexVec;
	std::vector<Triangle> triangles;
	void pushTriangle(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2,  bool backwards);
	glm::vec3 getNormal(glm::vec3 Q0, glm::vec3 Q1, glm::vec3 Q2,  bool backwards);

	Mesh(string fDAT, string fBMP);
	void prepareVBO();
	void computeBoundingSphere();
};

#endif