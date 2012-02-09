#include "Furniture.h"

Lamp::Lamp() {
	glm::mat4 identity = glm::mat4(1);
	
	//glm::mat4 transform = glm::translate(identity, glm::vec3(0, 0.15, 0)) * glm::scale(identity, glm::vec3(0.181, 0.181, 0.181));
	glm::mat4 transform = identity;
	
	//components.push_back(new Sphere(glm::translate(identity, vec3(0, 2, 0))));
	//components.push_back(new Cylinder(glm::scale(identity, vec3(0.3, 3, 0.3))));

	// Just a sphere
	components.push_back(new Sphere(glm::translate(identity, vec3(0, 1, 0))));
}
