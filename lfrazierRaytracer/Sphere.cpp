#include "Primitive.h"

using namespace glm;

#define PI 3.14159265

Sphere::Sphere(glm::mat4 trans) {
	name = "sphere";
	this->transform = trans;
	
	int numFacets = 20; // resolution of cylinder. Number of "panels". MUST DIVIDE EVENLY INTO 360
	float r1 = 1;
	float step = 360 / numFacets;
	//float step_phi = step / 2;
	float theta = 0;
	float phi = -90;
	// Top point
	vec3 P1 = vec3(0, r1, 0);
	this->points.push_back(P1);

	// Iterate over phi, theta. Surface of rotation: create arc from top to bottom, rotate around Y.
	while (phi < 90) {
		while (theta < 360) {
			float theta_radians = theta * (PI/180);
			float phi_radians = phi * (PI/180);
			float x = r1 * cos(theta_radians) * cos(phi_radians);
			float y = (r1 * sin(phi_radians));
			float z = r1 * sin(theta_radians) * cos(phi_radians);
			this->points.push_back(vec3(x, y, z));
			theta += step;
		}
		theta = 0;
		phi += step;
	}

	// Bottom point
	vec3 P2 = vec3(0, -1 * r1, 0);
	this->points.push_back(P2);

	// Push back all the triangles. Is drawing upside down!!!, so must switch top and bottom points.
	int offset = numFacets;
	for (int i = 1; i <= numFacets; i++) {
		int j = (i != numFacets) ? (i + 1) : 1; // Ususally just i+1, unless we are at the end of the circle, then make it 1 again
		pushTriangle(points[i], points.back(), points[j], false);
		int bottomLevel = (numFacets/2) - 1;
		for (int f = 0; f < bottomLevel; f++) {
			pushTriangle(points[i + (f * numFacets)], points[j + (f * numFacets)], points[i + ((f+1) * numFacets)], false);
		}
		pushTriangle(points[i + (bottomLevel * numFacets)], points.front(), points[j + (bottomLevel * numFacets)], true);
		for (int g = bottomLevel; g > 0; g--) {
			pushTriangle(points[j + (g * numFacets)], points[i + (g * numFacets)], points[j + ((g-1) * numFacets)], false);
		}
	}

	// Transfer data into arrays for vbo, nbo, cbo
	numIndices = vertVec.size();
	std::vector<float> vertCoords;
	for (int i = 0; i < vertVec.size(); i++) {
		vertCoords.push_back(vertVec[i][0]);
		vertCoords.push_back(vertVec[i][1]);
		vertCoords.push_back(vertVec[i][2]);
		vertCoords.push_back(1);
	}
	numVertices = vertCoords.size();
	vertices = new float[numVertices];
	for (int i = 0; i < numVertices; i++) {
		vertices[i] = vertCoords[i];
	}

	std::vector<float> normCoords;
	for(int i = 0; i < normVec.size(); i++) {
		normCoords.push_back(normVec[i][0]);
		normCoords.push_back(normVec[i][1]);
		normCoords.push_back(normVec[i][2]);
		normCoords.push_back(0);
	}
	numNormals = normCoords.size();
	normals = new float[numNormals];
	for (int i = 0; i < numNormals; i++) {
		normals[i] = normCoords[i];
	}
}

// Pushes vertices, normals, colors in the order they will be sent to the buffer. Also takes an option for "backwards" normal.
void Sphere::pushTriangle(vec3 point0, vec3 point1, vec3 point2, bool backwards) {
		vertVec.push_back(point0);
		vertVec.push_back(point1);
		vertVec.push_back(point2);

		for (int i = 0; i < 3; i++) {
			normVec.push_back(getNormal(point0, point1, point2, backwards));
		}
}

// Takes 3 points, and an option to flip the normal backwards before returning it.
vec3 Sphere::getNormal(vec3 Q0, vec3 Q1, vec3 Q2, bool backwards) {
	vec3 normal;
	normal.x = ((Q0.y - Q1.y) * (Q0.z + Q1.z)) + ((Q1.y - Q2.y) * (Q1.z + Q2.z)) + ((Q2.y - Q0.y) * (Q2.z + Q0.z));
	normal.y = ((Q0.z - Q1.z) * (Q0.x + Q1.x)) + ((Q1.z - Q2.z) * (Q1.x + Q2.x)) + ((Q2.z - Q0.z) * (Q2.x + Q0.x));
	normal.z = ((Q0.x - Q1.x) * (Q0.y + Q1.y)) + ((Q1.x - Q2.x) * (Q1.y + Q2.y)) + ((Q2.x - Q0.x) * (Q2.y + Q0.y));

	if (backwards) {
		normal.x = normal.x * -1;
		normal.y = normal.y * -1;
		normal.z = normal.z * -1;
	}
	return normal;
}