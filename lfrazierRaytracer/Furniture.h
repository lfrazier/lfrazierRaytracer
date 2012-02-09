#ifndef FURNITURE_H
#define FURNITURE_H

#include "Primitive.h"

class Node;

using namespace std;

class Furniture {
public:
	Node *node;

	Furniture(){
	}
	vector<Primitive *> components;
	//virtual void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation, float red, float green, float blue);
};


//Attribute List, in case I need it: float tX, float tZ, float rY, float sX, float sY, float sZ, float r, float g, float b
class Lamp: public Furniture {
public:
	Lamp();
	//void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
};

class Cabinet: public Furniture {
public:
	Cabinet();
	//void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
};

class Chair : public Furniture {
public:
	Chair();
	//void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
};

class Table: public Furniture {
public:
	Table();
	//void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
};

class Floor: public Furniture {
public:
	Floor(int x, int z);
	//void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
};

#endif