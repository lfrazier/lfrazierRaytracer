#include "Node.h"

using namespace glm;

Node::Node(Furniture *furn, Mesh *mesh, float tX, float tZ, float rY, float sX, float sY, float sZ, vector<float> mat) {
	selected = false;

	this->transX = tX;
	this->transZ = tZ;
	this->rotY = rY;
	this->scaleX = sX;
	this->scaleY = sY;
	this->scaleZ = sZ;
	this->MAT = mat;

	this->transform = glm::mat4(1);
	this->transform = scale(this->transform, vec3(this->scaleX, this->scaleY, this->scaleZ));
	this->transform = rotate(this->transform, this->rotY, vec3(0,1,0));
	this->transform = translate(this->transform, vec3(this->transX, 0, this->transZ));

	/*if (furnType.compare("table") == 0) {
		this->furn = new Table(transX, transZ, rotY, scaleX, scaleY, scaleZ, red, green, blue);
	} else if (furnType.compare("chair") == 0) {
		this->furn = new Chair(transX, transZ, rotY, scaleX, scaleY, scaleZ, red, green, blue);
	} else if (furnType.compare("cabinet") == 0) {
		this->furn = new Cabinet(transX, transZ, rotY, scaleX, scaleY, scaleZ, red, green, blue);
	} else if (furnType.compare("lamp") == 0) {
		this->furn = new Lamp(transX, transZ, rotY, scaleX, scaleY, scaleZ, red, green, blue);
	}*/

	this->furn = furn;
	if(this->furn != NULL) {
		this->furn->node = this;
	}

	this->mesh = mesh;
	if(this->mesh != NULL) {
		this->mesh->node = this;
	}
}

void Node::traverse(glm::mat4 transform, 
	unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, 
	unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation) {
	
		
		create(transform * this->transform, vbo, cbo, nbo, ibo, positionLocation, colorLocation, normalLocation, u_modelMatrixLocation); 
	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->traverse(transform * this->transform, vbo, cbo, nbo, ibo, positionLocation, colorLocation, normalLocation, u_modelMatrixLocation);
	}
}

void Node::addChild(Node *node) {
	for (int i = 0; i < (int)children.size(); i++) {
		if (children[i]->transX == node->transX && children[i]->transZ == node->transZ) {
			children[i]->addChild(node);
			return;
		}
	}
	children.push_back(node);
	node->parent = this;
	node->yDisp = 0;
	node->yDisp = this->yDisp + 1;
	node->transform = translate(node->transform, vec3(0, node->yDisp, 0));
	//node->transform = translate(node->transform, vec3(0, node->yDisp, 0))  * this->transform ;
}

vector<Node *> Node::preorderList(vector<Node *> currentList) {
	currentList.push_back(this);
	for(unsigned int i = 0; i < children.size(); i++) {
		currentList = children[i]->preorderList(currentList);
	}
	return currentList;
}

void Node::create(glm::mat4 transform, 
	unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, 
	unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation) {
		if (mesh != NULL){
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER,  mesh->numVertices * sizeof(float),  mesh->vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
					
				glBindBuffer(GL_ARRAY_BUFFER, cbo);
				int numColorCoords = (mesh->numVertices / 4.0) * 3.0;
				if (this->selected) {
					float *selectedColors = new float[numColorCoords];
					for (int i = 0; i < numColorCoords; i++) {
						selectedColors[i] = 1;
					}
					glBufferData(GL_ARRAY_BUFFER,  numColorCoords * sizeof(float),  selectedColors, GL_STREAM_DRAW);
				} else {
						float *colors = new float[numColorCoords];
						for (int i = 0; i < numColorCoords; i++) {
							if (i%3 == 0){
								colors[i] = MAT[0];
							} else if (i%3 == 1) {
								colors[i] = MAT[1];
							} else {
								colors[i] = MAT[2];
							}
						}	
					glBufferData(GL_ARRAY_BUFFER,  numColorCoords * sizeof(float),  colors, GL_STREAM_DRAW);	//the color is going to change every frame
				}
			
				glBindBuffer(GL_ARRAY_BUFFER, nbo);
				glBufferData(GL_ARRAY_BUFFER,  mesh->numNormals * sizeof(float),  mesh->normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,

				//activate our three kinds of information
				glEnableVertexAttribArray(positionLocation);
				glEnableVertexAttribArray(colorLocation);
				glEnableVertexAttribArray(normalLocation);
	
				//we're using the vertex data first
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
				glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
				//now use color data, remember we're not using 4 at a time anymore
				glBindBuffer(GL_ARRAY_BUFFER, cbo);
				glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
				//one more time with the normals
				glBindBuffer(GL_ARRAY_BUFFER, nbo);
				glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

				//set the modelview uniform
				glm::mat4 newTrans = transform *  mesh->transform;
				glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &newTrans[0][0]);

				//draw the elements
				glDrawArrays(GL_TRIANGLES, 0, mesh->numIndices);
	
				//shut off the information since we're done drawing
				glDisableVertexAttribArray(positionLocation);
				glDisableVertexAttribArray(colorLocation);
				glDisableVertexAttribArray(normalLocation);
		} else {
			for(unsigned int i = 0; i < furn->components.size(); i++) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER,  furn->components[i]->numVertices * sizeof(float),  furn->components[i]->vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
					
				glBindBuffer(GL_ARRAY_BUFFER, cbo);
				//printf("%d\n", (furn->components[i]->numVertices));
				int numColorCoords = (int)((furn->components[i]->numVertices / 4.0) * 3);
				//printf("%d\n", numColorCoords);
				if (this->selected) {
					float *selectedColors = new float[numColorCoords];
					for (int i = 0; i < numColorCoords; i++) {
						selectedColors[i] = 1;
					}
					glBufferData(GL_ARRAY_BUFFER,  numColorCoords * sizeof(float),  selectedColors, GL_STREAM_DRAW);
				} else {
						//printf("%d\n", numColorCoords);
						float *colors = new float[numColorCoords];
						for (int i = 0; i < numColorCoords; i++) {
							if (i%3 == 0){
								float first = MAT[0];
								colors[i] = MAT[0];
							} else if (i%3 == 1) {
								colors[i] = MAT[1];
							} else {
								colors[i] = MAT[2];
							}
						}	
					glBufferData(GL_ARRAY_BUFFER,  numColorCoords * sizeof(float),  colors, GL_STREAM_DRAW);	//the color is going to change every frame
					delete colors;
				}
			
				glBindBuffer(GL_ARRAY_BUFFER, nbo);
				glBufferData(GL_ARRAY_BUFFER,  furn->components[i]->numNormals * sizeof(float),  furn->components[i]->normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,

				//activate our three kinds of information
				glEnableVertexAttribArray(positionLocation);
				glEnableVertexAttribArray(colorLocation);
				glEnableVertexAttribArray(normalLocation);
	
				//we're using the vertex data first
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
				glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
				//now use color data, remember we're not using 4 at a time anymore
				glBindBuffer(GL_ARRAY_BUFFER, cbo);
				glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
				//one more time with the normals
				glBindBuffer(GL_ARRAY_BUFFER, nbo);
				glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
				// index data
				Cube *c1 = dynamic_cast<Cube *>(furn->components[i]);
				if (c1 != NULL) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER,  furn->components[i]->numIndices * sizeof(unsigned short),  furn->components[i]->indices, GL_STATIC_DRAW);
				}

				//set the modelview uniform
				glm::mat4 newTrans = transform *  furn->components[i]->transform;
				glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &newTrans[0][0]);

				//draw the elements
				if (c1 != NULL) {
					glDrawElements(GL_TRIANGLES,  furn->components[i]->numIndices, GL_UNSIGNED_SHORT, 0);
				} else {
					glDrawArrays(GL_TRIANGLES, 0, furn->components[i]->numIndices);
				}
	
				//shut off the information since we're done drawing
				glDisableVertexAttribArray(positionLocation);
				glDisableVertexAttribArray(colorLocation);
				glDisableVertexAttribArray(normalLocation);
			}		
		}
}

void Node::removeNode(Node *node) {
	if (this == node) {
		this->parent->removeMe(this);
	} else {
		for(unsigned int i = 0; i < children.size(); i++) {
			children[i]->removeNode(node);
		}
	}
}

void Node::removeMe(Node *child){
	/*for(unsigned int i = 0; i < child->children.size(); i++) {
		removeMe(child->children[i]);
	}
	for(unsigned int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + 1);
			child->parent = NULL;
			delete child;
		}
	}*/
	int index = 0;
	for (index = 0; index < children.size(); index++) {
		if (children[index] == child) {
			break;
		}
	}
	children.erase(children.begin() + index);
}

void Node::cleanup(){
	for(unsigned int i = 0; i < children.size(); i++) {
		children[i]->cleanup();
	}
	delete parent;
	delete furn;
	delete this;
}
