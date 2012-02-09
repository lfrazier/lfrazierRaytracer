#include "Camera.h"

Camera::Camera(int RESOx, int RESOy, vec3 posVec, vec3 dirVec, vec3 upVec, float fov)
{
	this->RESOx = RESOx;
	this->RESOy = RESOy;
	this->position = posVec;
    this->forward = dirVec;
    this->up = upVec;
    this->fovy = degreesToRadians(fov);
    this->aspect = float(this->RESOx)/ float(this->RESOy);
    this->fovx = atanf(tanf(this->fovy)*aspect);

	this->A = cross(forward, up);
	this->B = cross(A, forward);
	this->M = position + forward;
	float forLength =vecLength(forward);
	this->H = A * vecLength(forward) * tanf(fovx) / vecLength(A);
	this->V = B * vecLength(forward) * tanf(fovy) / vecLength(B);

}

Ray Camera::getWorldRayForPixel(int x, int y){
    float sx = float(x)/(this->RESOx - 1);
    float sy = float(y)/(this->RESOy - 1);
    vec3 P = M + (H * (2*sx - 1)) + (V * (2*sy - 1));
    vec3 ray_dir = P - position;
	ray_dir = glm::normalize(ray_dir);
    return Ray(position, ray_dir);
}

float Camera::degreesToRadians(float degrees){
    return degrees * (3.141592654/float(180));
}