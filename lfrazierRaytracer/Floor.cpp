#include "Furniture.h"

Floor::Floor(int x, int z) {
	glm::mat4 identity = glm::mat4(1);
	//glm::mat4 floor_transform =  glm::translate(identity, glm::vec3(0, -0.7, 0)) * glm::scale(identity, glm::vec3(x, 0.05, z));
	glm::mat4 floor_transform =  glm::translate(identity, glm::vec3(0, -0.7, 0)) * glm::scale(identity, glm::vec3(x, 0.25, z));
	components.push_back(new Cube(floor_transform));
}
