#include "Furniture.h"

Cabinet::Cabinet() {
	glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 transform = glm::scale(glm::translate(identity, vec3(0, 1.4, 0)) , glm::vec3(3, 4, 0.5));
	components.push_back(new Cube(transform));
}
