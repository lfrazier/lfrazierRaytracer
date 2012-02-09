#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace glm;
using namespace std;

class Primitive {
public:
	float *vertices;
	float *colors;
	float *normals;
	unsigned short *indices;
	
	int numVertices;
	int numColors;
	int numNormals;
	int numIndices;

	string name;

	glm::mat4 transform;
	Primitive(){ // default color = yellow.
	};
	virtual ~Primitive() { }
};

class Cube : public Primitive {
public:
	float width;
	Cube(glm::mat4 trans);
};

class Cylinder : public Primitive {
public:
	std::vector<glm::vec3> points;	
	std::vector<vec3> vertVec;
	std::vector<vec3> normVec;
	std::vector<vec3> indexVec;
	Cylinder(glm::mat4 trans);
	void pushTriangle(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2,  bool backwards);
	glm::vec3 getNormal(glm::vec3 Q0, glm::vec3 Q1, glm::vec3 Q2,  bool backwards);
};

class Sphere : public Primitive {
public:
	std::vector<glm::vec3> points;	
	std::vector<vec3> vertVec;
	std::vector<vec3> normVec;
	std::vector<vec3> indexVec;
	void pushTriangle(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2,  bool backwards);
	glm::vec3 getNormal(glm::vec3 Q0, glm::vec3 Q1, glm::vec3 Q2,  bool backwards);
	Sphere(glm::mat4 trans);
};

#endif
