#include "Furniture.h"

/*void Furniture::create(glm::mat4 transform, 
	unsigned int vbo, unsigned int cbo, unsigned int nbo, unsigned int ibo, 
	unsigned int positionLocation, unsigned int colorLocation, unsigned int normalLocation, unsigned int u_modelMatrixLocation, float red, float green, float blue) {
		for(unsigned int i = 0; i < components.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, components[i]->numVertices * sizeof(float), components[i]->vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,

			glBindBuffer(GL_ARRAY_BUFFER, cbo);
			glBufferData(GL_ARRAY_BUFFER, components[i]->numColors * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
			
			glBindBuffer(GL_ARRAY_BUFFER, nbo);
			glBufferData(GL_ARRAY_BUFFER, components[i]->numNormals * sizeof(float), components[i]->normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,

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
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, components[i]->numIndices * sizeof(unsigned short), components[i]->indices, GL_STATIC_DRAW);


			//set the modelview uniform
			glm::mat4 newTrans = transform * components[i]->transform;
			glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &newTrans[0][0]);

			//draw the elements
			glDrawElements(GL_TRIANGLES, components[i]->numIndices, GL_UNSIGNED_SHORT, 0);
	
			//shut off the information since we're done drawing
			glDisableVertexAttribArray(positionLocation);
			glDisableVertexAttribArray(colorLocation);
			glDisableVertexAttribArray(normalLocation);
		}		
}

void Furniture::setAttributes(float transX, float transZ, float rotY, float scaleX, float scaleY, float scaleZ) {
	this->transX = transX;
	this->transZ =  transZ;
	this->rotY = rotY;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleZ = scaleZ;
}
*/