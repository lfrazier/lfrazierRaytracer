#include "Furniture.h"

class Furniture;

class Renderer {
public:
	Renderer();
	void render(Furniture *furn, glm::mat4 transform);
};