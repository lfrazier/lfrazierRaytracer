#ifndef NODE_H
#define NODE_H

#include "Furniture.h"
#include "Mesh.h"

class Furniture;
class Mesh;

class Node {
public:
	Node *parent;
	vector<Node *> children;
	Furniture *furn;
	Mesh *mesh;
	vector<float> MAT;

	float transX;
	float transZ;
	float rotY;
	float scaleX;
	float scaleY;
	float scaleZ;

	float yDisp;
	glm::mat4 transform;

	bool selected;

	Node() {
		float matNums[] = {0.2, 0.2, 0, 4, 0, 0, 0, 0};
		MAT = vector<float>(matNums, matNums+8);
	}
	//Node(Furniture *furn, Mesh *mesh, float tX, float tZ, float rY, float sX, float sY, float sZ, float r, float g, float b);
	Node(Furniture *furn, Mesh *mesh, float tX, float tZ, float rY, float sX, float sY, float sZ, vector<float> MAT);
	void traverse(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
	void addChild(Node *node);
	vector<Node *> preorderList(vector<Node *> nodes);
	void create(glm::mat4 transform, unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation);
	void removeNode(Node *node);
	void removeMe(Node *child);
	void cleanup();
};

#endif