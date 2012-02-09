//
//  intersection.cpp
//  raytracerTests
//
//  Created by Lauren Frazier on 11/22/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "intersection.h"
#include <vector>

using namespace glm;
using namespace std;


const float EPSILON = 1e-06f;
const float	LIL_EPSILON = EPSILON;
const float BIG_EPSILON = 1e-03f;
const double TEN_KAZILLION = 1e26;

vec3 transformP0(vec3 const& P0,  mat4 const& Tinv)
{
	vec4 Ps4 = Tinv * vec4(P0.x, P0.y, P0.z, 1);
	vec3 Ps = vec3(Ps4.x, Ps4.y, Ps4.z);
	return Ps;
}

vec3 transformV0(vec3 const& V0,  mat4 const& Tinv)
{
	vec4 V04(V0.x, V0.y, V0.z, 0);
	vec4 Vs4 = Tinv * V04;
	vec3 Vs = vec3(Vs4.x, Vs4.y, Vs4.z);
	return Vs;
}

mat3 setColumn(mat3 matrix, int index, vec3 vector)
{
    matrix[index].x = vector.x;
    matrix[index].y = vector.y;
    matrix[index].z = vector.z;
    return matrix;
}

// Tries to find the intersection of a ray and a sphere.
// P0 is the position from which the ray emanates; V0 is the
// direction of the ray.
// matrix is the transformation matrix of the sphere.
// This function should return the smallest positive t-value of the intersection
// (a point such that P0+t*V0 intersects the sphere), or -1 if there is no
// intersection.
double RaySphereIntersect(vec3 const& P0, vec3 const& V0,
						  mat4 const& Tinv)
{
	vec3 Ps = transformP0(P0, Tinv);
	vec3 Vs = transformV0(V0, Tinv);
	
	float a = dot(Vs, Vs);
	float b = dot(2.0f * Vs, Ps);
	float c = dot(Ps, Ps) - 1;
	
	if (fabs(a) < EPSILON) {
		return -1;
	}
	double discriminant = b*b - 4*a*c;
	if (discriminant < 0){
		return -1;
	}
	double t_plus = (-b + sqrt(discriminant))/(2*a);
	double t_minus = (-b - sqrt(discriminant))/(2*a);
	if((t_plus < 0) && (t_minus < 0)){
		return -1;
	} else if ((t_plus < 0) && (t_minus >=0)) {
		return t_minus;
	} else if ((t_plus >= 0) && (t_minus < 0)) {
		return t_plus;
	}
	return std::min(t_plus, t_minus);
}

bool ls2DpointInTri(const vec2& p1_p,const vec2& p2_p,const vec2& p3_p, const vec2& point_p){
	vec3 p1(p1_p.x, p1_p.y, 1);
	vec3 p2(p2_p.x, p2_p.y, 1);
	vec3 p3(p3_p.x, p3_p.y, 1);
	vec3 point(point_p.x, point_p.y, 1);
	mat3 matA = mat3(1.0f);
    matA = setColumn(matA, 0, p1);
	matA = setColumn(matA, 1, p2);
	matA = setColumn(matA, 2, p3);
	mat3 invA = inverse(matA);
	vec3 vecB = point;
	vec3 barys = invA * vecB;
	  
	for (int i = 0; i<3; i++){
		if (barys[i] < 0){
			return false;
		}
	}
	return true;
	
}

// Tries to find the intersection of a ray and a triangle.
// This is just like the above function, but it intersects the ray with a
// triangle instead. The parameters p1, p2, and p3 specify the three
// points of the triangle, in object space.
double RayPolyIntersect(
                        vec3 const& P0, vec3 const& V0,
                        vec3 const& p1, vec3 const& p2, vec3 const& p3,
                        mat4 const& Tinv)
{
	vec3 Ps = transformP0(P0, Tinv);
	vec3 Vs = transformV0(V0, Tinv);
	vec3 v12 = p2-p1;
	vec3 v23 = p3-p2;
	vec3 norm = cross(v12,v23);
	norm = normalize(norm);
	float parallel = dot(norm, Vs);
	if (fabs(parallel) < LIL_EPSILON) {
		return -1;
	}
	float t = (dot(norm, p1) - dot(norm, Ps))/parallel;
	vec3 pt = Ps+(t*Vs);
	if (fabs(norm.y) > BIG_EPSILON) {
		if ( !ls2DpointInTri(vec2(p1.x, p1.z), vec2(p2.x, p2.z), vec2(p3.x, p3.z), vec2(pt.x, pt.z)) ) {
			return -1;
		}
	} else {
		if ( !ls2DpointInTri(vec2(p1.x, p1.y), vec2(p2.x, p2.y), vec2(p3.x, p3.y), vec2(pt.x, pt.y)) ) {
			return -1;
		}
	}
	return t;
}

// This is just like Test_RaySphereIntersect, but with a unit cube instead of a
// sphere. A unit cube extends from -0.5 to 0.5 in all axes.
double RayCubeIntersect(
                        vec3 const& P0, vec3 const& V0,
                        mat4 const& Tinv)
{	
	vec3 Ps = transformP0(P0, Tinv);
	vec3 Vs = transformV0(V0, Tinv);
	vec3 mins;
	vec3 maxs;
	const float THICKNESS = 0.5f;
	//P0+t*V0
	for (int i = 0; i<3; i++) {
		float pos, neg;
		if (fabs(Vs[i]) < LIL_EPSILON) {
			if (Ps[i] > -THICKNESS && Ps[i] < THICKNESS) {
				neg = -TEN_KAZILLION;
				pos = TEN_KAZILLION;
			}
			else {
				neg = TEN_KAZILLION;
				pos = TEN_KAZILLION;
			}
		} else {
			pos= (THICKNESS-Ps[i]) / Vs[i];
			neg = (-THICKNESS - Ps[i])/Vs[i];
		}
		mins[i] = std::min(pos,neg);
		maxs[i] = std::max(pos,neg);
	}
	float entry = -TEN_KAZILLION;
	float exit = TEN_KAZILLION;
	for (int i = 0; i < 3; i++) {
		entry = std::max(entry, mins[i]);
		exit = std::min(exit, maxs[i]);
	}
	if (entry > exit) {
		return -1;
	}
	if (entry < 0) {
		if (exit < 0) {
			return -1;
		}else {
			return exit;
		}
        
	}
	return entry;
}

// This is also like Test_RaySphereIntersect, but with a unit cylinder.  A unit
// cylinder is 1 unit tall, 1 unit in diameter, with the bottom base at (0,-0.5,0)
double RayCylinderIntersect(vec3 const& P0, vec3 const& V0, mat4 const& Tinv)
{
    vec3 Ps = transformP0(P0, Tinv);
	vec3 Vs = transformV0(V0, Tinv);
    float RADIUS = 0.5;
    vector<float> possible_t;
    // Test Side
    // (Vx^2 + Vz^2)t^2 + (4VxVzPxPz)t + (Px^2 + Pz^2 - r^2)
    float a = ((Vs.x * Vs.x) + (Vs.z * Vs.z));
    float b = (2 * Vs.x * Ps.x) + (2 * Vs.z * Ps.z);
    float c = (Ps.x * Ps.x) + (Ps.z * Ps.z) - (RADIUS * RADIUS);
    
    double discriminant = b*b - 4*a*c;
	if (discriminant >= 0){
        double t_plus = (-b + sqrt(discriminant))/(2*a);
        double t_minus = (-b - sqrt(discriminant))/(2*a);
        /*if((t_plus < 0) && (t_minus < 0)){
            t_side = -1;
        } else if ((t_plus < 0) && (t_minus >=0)) {
            t_side = t_minus;
        } else if ((t_plus >= 0) && (t_minus < 0)) {
            t_side = t_plus;
        } else {
            t_side = std::min(t_plus, t_minus);
        }*/
        if (t_minus > 0 && (Ps.y + (Vs.y * t_minus)) >= -0.5  && (Ps.y + (Vs.y * t_minus)) <= 0.5) {
            possible_t.push_back(t_minus);
        }
        if (t_plus > 0 && (Ps.y + (Vs.y * t_plus)) >= -0.5  && (Ps.y + (Vs.y * t_plus)) <= 0.5) {
            possible_t.push_back(t_plus);
        }
    }
    
    // Test Caps
    // Ax + By + Cz = D
    // 0 + (Py + Vy*t) + 0 = +/- 0.5
    float t_top = (0.5 - Ps.y) / Vs.y;
    if (t_top > 0 && pow(Ps.x + (Vs.x * t_top), 2) + pow(Ps.z + (Vs.z * t_top), 2) <= pow(RADIUS, 2) ) {
        possible_t.push_back(t_top);
    }
    float t_bottom = (-0.5 - Ps.y) / Vs.y;
    if (t_bottom > 0 && pow(Ps.x + (Vs.x * t_bottom), 2) + pow(Ps.z + (Vs.z * t_bottom), 2) <= pow(RADIUS, 2) ) {
        possible_t.push_back(t_bottom);
    }
    
    // If no t satisfied the equations, we missed the whole cylinder; return -1.
    if (possible_t.empty()) {
        return -1;
    }
    
    float final_t = TEN_KAZILLION;
    for (int i = 0; i < possible_t.size(); i++) {
        if (possible_t[i] < final_t) {
            final_t = possible_t[i];
        }
    }
    return final_t;
}
