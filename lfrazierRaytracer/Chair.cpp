#include "Furniture.h"

Chair::Chair() {
	glm::mat4 identity = glm::mat4(1);
	
	glm::mat4 transform = glm::translate(identity, glm::vec3(0, -0.063, 0)) * glm::scale(identity, glm::vec3(0.492, 0.05, 0.492));
	components.push_back(new Cube(transform));

	transform = glm::translate(identity, glm::vec3(0.17, -0.248, 0.2)) * glm::scale(identity, glm::vec3(0.068, 0.417, 0.068));
	components.push_back(new Cube(transform));

	transform = glm::translate(identity, glm::vec3(-0.17, -0.248, 0.2)) * glm::scale(identity, glm::vec3(0.068, 0.417, 0.068));
	components.push_back(new Cube(transform));

	transform = glm::translate(identity, glm::vec3(0.17, -0.248, -0.2)) * glm::scale(identity, glm::vec3(0.068, 0.417, 0.068));
	components.push_back(new Cube(transform));

	transform = glm::translate(identity, glm::vec3(-0.17, -0.248, -0.2)) * glm::scale(identity, glm::vec3(0.068, 0.417, 0.068));
	components.push_back(new Cube(transform));

	float rot = 12.056;
	transform = glm::translate(identity, glm::vec3(-0.267, 0.206, 0)) * glm::scale(identity, glm::vec3(0.045, 0.54, 0.48)) * glm::rotate(identity, rot, glm::vec3(0,0,1));
	components.push_back(new Cube(transform));
}
