#include "Ray.h"

#ifndef Camera_h
#define Camera_h

using namespace std;
using namespace glm;

class Camera{
public:
	int RESOx;
	int RESOy;
	vec3 position;
	vec3 forward;
	vec3 up;
    float fovy;
    float fovx;
    float aspect;

	vec3 A, B, M, H, V;

    Camera(int RESOx, int RESOy, vec3 posVec, vec3 dirVec, vec3 upVec, float fov);
    Ray getWorldRayForPixel(int x, int y);
    float degreesToRadians(float degrees);
	float vecLength (vec3 vec) { return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z ); }
};

#endif