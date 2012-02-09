#include "Primitive.h"

using namespace std;
using namespace glm;

#define PI 3.14159265

Cylinder::Cylinder(glm::mat4 trans) {
	name = "cylinder";
	this->transform = trans;

	int numFacets = 360; // resolution of cylinder. Number of "panels". MUST DIVIDE EVENLY INTO 360
	float r1 = 1;
	float r2 = r1; // Change this for cone
	float step = 360 / numFacets;
	float theta = 0;
	// Top circle
	vec3 P1 = vec3(0, 0.5, 0);
	this->points.push_back(P1);

	while (theta < 360) {
		float theta_radians = theta * (PI/180);
		this->points.push_back(vec3(r1 * cos(theta_radians), 0.5, r1 * sin(theta_radians)));
		theta += step;
	}
	// Bottom circle
	vec3 P2 = vec3(0, -0.5, 0);
	this->points.push_back(P2);
	theta = 0;
	while (theta < 360) {
		float theta_radians = theta * (PI/180);
		this->points.push_back(glm::vec3(r2 * cos(theta_radians), -0.5, r2 * sin(theta_radians)));
		theta += step;
	}

	// Push back all the triangles
	int offset = numFacets + 1;
	for (int i = 1; i <= numFacets; i++) {
		int j = (i != numFacets) ? (i + 1) : 1; // Ususally just i+1, unless we are at the end of the circle, then make it 1 again
		
		pushTriangle(points[i], points[0], points[j], false);
		pushTriangle(points[j], points[i], points[i + offset], true);
		pushTriangle(points[i + offset], points[offset], points[j + offset], false);
		pushTriangle(points[j + offset], points[i + offset], points[j], false);
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

void Cylinder::pushTriangle(vec3 point0, vec3 point1, vec3 point2, bool backwards) {
		vertVec.push_back(point0);
		vertVec.push_back(point1);
		vertVec.push_back(point2);

		for (int i = 0; i < 3; i++) {
			normVec.push_back(getNormal(point0, point1, point2, backwards));
		}
}

vec3 Cylinder::getNormal(vec3 Q0, vec3 Q1, vec3 Q2, bool backwards) {
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

/*Cylinder::Cylinder(glm::mat4 trans, float r, float g, float b) {
	// Two circles both alike in dignity...

	int numFacets = 20; // resolution of cylinder

	std::vector<vec3> vertVec;
	std::vector<vec3> normVec;
	std::vector<vec3> indexVec;
	std::vector<vec3> colorVec;

	vec3 color = vec3(r, g, b);

	vec3 P1 = vec3(0, 0.5, 0);
	vec3 P2 = vec3(0, -0.5,0);
	vec3 A = vec3(1, 0, 0);
	vec3 B = cross(P2 - P1, A) ;
	//vec3 B = vec3(0,0,1);
	float r1 = 1;
	float r2 = r1; // Change this for cone
	float step = 360 / numFacets;
	float theta1 = 0;
	float theta2 = theta1 + step;
	vec3 Q0, Q1, Q2, Q3;
	while (theta1 < 360) {
		// Sides
		Q0 = P1 + r1 * cos(theta1) * A + r1 * sin(theta1) * B;
		Q1 = P2 + r2 * cos(theta1) * A + r2 * sin(theta1) * B;
		Q2 = P2 + r2 * cos(theta2) * A + r2 * sin(theta2) * B;
		Q3 = P1 + r1 * cos(theta2) * A + r1 * sin(theta2) * B;

		int first = vertVec.size(); // index that Q0 will have in the vertex list

		vertVec.push_back(Q0);
		vertVec.push_back(Q1);
		vertVec.push_back(Q2);
		vertVec.push_back(Q3);

		//indexVec.push_back(vec3(first, first + 2, first + 1)); // first triangle is 0, 2, 1
		//indexVec.push_back(vec3(first, first + 2, first + 3)); // other triangle is 0, 2, 3
		

		vec3 normal;
		normal.x = ((Q0.y - Q1.y) * (Q0.z + Q1.z)) + ((Q1.y - Q2.y) * (Q1.z + Q2.z)) + ((Q2.y - Q0.y) * (Q2.z + Q0.z));
		normal.y = ((Q0.z - Q1.z) * (Q0.x + Q1.x)) + ((Q1.z - Q2.z) * (Q1.x + Q2.x)) + ((Q2.z - Q0.z) * (Q2.x + Q0.x));
		normal.z = ((Q0.x - Q1.x) * (Q0.y + Q1.y)) + ((Q1.x - Q2.x) * (Q1.y + Q2.y)) + ((Q2.x - Q0.x) * (Q2.y + Q0.y));

		for (int i = 0; i < 4; i++) {
			normVec.push_back(normal);
			colorVec.push_back(color);
		}

		// Top Cap
		int  topCap = vertVec.size();

		vertVec.push_back(P1);
		vertVec.push_back(Q0);
		vertVec.push_back(Q3);

		//indexVec.push_back(vec3(topCap, topCap + 1, topCap + 2));

		for (int i = 0; i < 3; i++) {
			normVec.push_back(vec3(0,1,0));
			colorVec.push_back(color);
		}

		// Bottom Cap
		int bottomCap = vertVec.size();

		vertVec.push_back(P2);
		vertVec.push_back(Q1);
		vertVec.push_back(Q2);

		//indexVec.push_back(vec3(bottomCap, bottomCap + 1, bottomCap + 2));

		for (int i = 0; i < 3; i++) {
			normVec.push_back(vec3(0,-1,0));
			colorVec.push_back(color);
		}

		indexVec.push_back(vec3(first, first + 2, first + 1));
		indexVec.push_back(vec3(bottomCap + 1, bottomCap, bottomCap + 2));
		indexVec.push_back(vec3(first + 2, first + 3, first));
		indexVec.push_back(vec3(topCap + 1, topCap, topCap + 2));


		printf("Theta: %f\n", theta1);
		theta1 += step;
		theta2 += step;
	}

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

	std::vector<float> colorCoords;
	for (int i = 0; i < colorVec.size(); i++) {
		colorCoords.push_back(colorVec[i][0]);
		colorCoords.push_back(colorVec[i][1]);
		colorCoords.push_back(colorVec[i][2]);
	}
	numColors = colorCoords.size();
	colors = new float[numColors];
	for (int i = 0; i < numColors; i++) {
		colors[i] = colorCoords[i];
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

	std::vector<unsigned short> indexCoords;
	for (int i = 0; i < indexVec.size(); i++) {
		indexCoords.push_back(indexVec[i][0]);
		indexCoords.push_back(indexVec[i][1]);
		indexCoords.push_back(indexVec[i][2]);
	}
	numIndices = indexCoords.size();
	indices = new unsigned short[numIndices];
	for (int i = 0; i < numIndices; i++) {
		indices[i] = indexCoords[i];
	}
}*/