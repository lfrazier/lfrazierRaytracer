#ifndef Ray_h
#define Ray_h

#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"

using namespace glm;

class Ray{
public:
    vec3 origin;
    vec3 direction;
    Ray(vec3 orig, vec3 dir) {
        origin = orig;
        direction = dir;
    }
};

#endif