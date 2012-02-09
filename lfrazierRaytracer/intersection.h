//
//  intersection.h
//  raytracerTests
//
//  Created by Lauren Frazier on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef raytracerTests_intersection_h
#define raytracerTests_intersection_h

#include "../glm/glm.hpp"


using namespace glm;

// Tries to find the intersection of a ray and a sphere.
// P0 is the position from which the ray emanates; V0 is the
// direction of the ray.
// matrix is the transformation matrix of the sphere.
// This function should return the smallest positive t-value of the intersection
// (a point such that P0+t*V0 intersects the sphere), or -1 if there is no
// intersection.
double RaySphereIntersect(vec3 const& P0, vec3 const& V0,
						  mat4 const& Tinv);

// Tries to find the intersection of a ray and a triangle.
// This is just like the above function, but it intersects the ray with a
// triangle instead. The parameters p1, p2, and p3 specify the three
// points of the triangle, in object space.
double RayPolyIntersect(
						vec3 const& P0, vec3 const& V0,
						vec3 const& p1, vec3 const& p2, vec3 const& p3,
						mat4 const& Tinv);

// This is just like Test_RaySphereIntersect, but with a unit cube instead of a
// sphere. A unit cube extends from -0.5 to 0.5 in all axes.
double RayCubeIntersect(
						vec3 const& P0, vec3 const& V0,
						mat4 const& Tinv);

// This is also like Test_RaySphereIntersect, but with a unit cylinder.  A unit
// cylinder is 1 unit tall, 1 unit in diameter, with the bottom base at (0,-0.5,0)
double RayCylinderIntersect(vec3 const& P0, vec3 const& V0, mat4 const& Tinv);

// Helper functions
vec3 transformP0(vec3 const& P0,  mat4 const& Tinv);
vec3 transformV0(vec3 const& V0,  mat4 const& Tinv);
bool ls2DpointInTri(const vec2& p1_p,const vec2& p2_p,const vec2& p3_p, const vec2& point_p);
mat3 setColumn(mat3 matrix, int index, vec3 vector);

#endif
