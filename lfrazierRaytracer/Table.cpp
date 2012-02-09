#include "Furniture.h"

Table::Table() {
	glm::mat4 identity = glm::mat4(1);
	
	glm::mat4 transform = glm::scale(glm::translate(identity, glm::vec3(0, 0.13, 0)), glm::vec3(1, 0.1, 1));
	components.push_back(new Cube(transform));

	transform = glm::scale( glm::translate(identity, glm::vec3(0.346, -0.35, -0.372)), glm::vec3(0.139, 0.848, 0.139));
	components.push_back(new Cube(transform));

	transform = glm::scale( glm::translate(identity, glm::vec3(-0.346, -0.35, -0.372)), glm::vec3(0.139, 0.848, 0.139));
	components.push_back(new Cube(transform));

	transform =  glm::scale(glm::translate(identity, glm::vec3(0.346, -0.35, 0.372)), glm::vec3(0.139, 0.848, 0.139));
	components.push_back(new Cube(transform));

	transform =  glm::scale(glm::translate(identity, glm::vec3(-0.346, -0.35, 0.372)), glm::vec3(0.139, 0.848, 0.139));
	components.push_back(new Cube(transform));
}
