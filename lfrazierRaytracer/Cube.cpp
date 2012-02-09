#include "Primitive.h"


using namespace glm;

Cube::Cube(glm::mat4 trans) {
	name = "cube";
	this->transform = trans;

	vec4 leftTopFront = vec4(-0.5, 0.5, 0.5, 1);
	vec4 rightTopFront = vec4(0.5,0.5, 0.5, 1);
	vec4 rightBottomFront = vec4(0.5, -0.5, 0.5, 1);
	vec4 leftBottomFront = vec4(-0.5, -0.5, 0.5, 1);
	vec4 leftTopBack = vec4(-0.5, 0.5, -0.5, 1);
	vec4 rightTopBack = vec4(0.5, 0.5, -0.5, 1);
	vec4 rightBottomBack = vec4(0.5, -0.5, -0.5, 1);
	vec4 leftBottomBack = vec4(-0.5, -0.5, -0.5, 1);

	std::vector<vec4> points;
	// Front
	points.push_back(leftTopFront); // 0
	points.push_back(rightTopFront);
	points.push_back(rightBottomFront);
	points.push_back(leftBottomFront);
	// Left
	points.push_back(leftTopFront); // 4
	points.push_back(leftTopBack);
	points.push_back(leftBottomBack);
	points.push_back(leftBottomFront);
	// Top
	points.push_back(leftTopFront); // 8
	points.push_back(leftTopBack);
	points.push_back(rightTopBack);
	points.push_back(rightTopFront);
	// Back
	points.push_back(leftTopBack); // 12
	points.push_back(leftBottomBack);
	points.push_back(rightBottomBack);
	points.push_back(rightTopBack);
	// Right
	points.push_back(rightTopFront); // 16
	points.push_back(rightTopBack);
	points.push_back(rightBottomBack);
	points.push_back(rightBottomFront);
	// Bottom
	points.push_back(leftBottomFront); // 20
	points.push_back(leftBottomBack);
	points.push_back(rightBottomBack);
	points.push_back(rightBottomFront);
	std::vector<float> coords;
	for (unsigned int i = 0; i < points.size(); i++) {
		coords.push_back(points[i][0]);
		coords.push_back(points[i][1]);
		coords.push_back(points[i][2]);
		coords.push_back(points[i][3]);
	}
	numVertices = (int)coords.size();
	vertices = new float[numVertices];
	for (unsigned int i = 0; i < coords.size(); i++) {
		vertices[i] = coords[i];
	}

	vec4 forward = vec4(0,0,1,0);
	vec4 backward = vec4(0,0,-1,0);
	vec4 up = vec4(0,1,0,0);
	vec4 down = vec4(0,-1,0,0);
	vec4 left = vec4(-1,0,0,0);
	vec4 right = vec4(1,0,0,0);
	std::vector<vec4> normalVector;
	for (int i = 0; i < 24; i++) {
		if (i < 4)
			normalVector.push_back(forward);
		else if (i < 8)
			normalVector.push_back(left);
		else if (i < 12)
			normalVector.push_back(up);
		else if (i < 16)
			normalVector.push_back(backward);
		else if (i < 20)
			normalVector.push_back(right);
		else if (i < 24)
			normalVector.push_back(down);
	}
	std::vector<float> normalCoords;
	for (unsigned int i = 0; i < normalVector.size(); i++){
		normalCoords.push_back(normalVector[i][0]);
		normalCoords.push_back(normalVector[i][1]);
		normalCoords.push_back(normalVector[i][2]);
		normalCoords.push_back(normalVector[i][3]);
	}
	numNormals = (int)normalCoords.size();
	normals = new float[numNormals];
	for (int i = 0; i < normalCoords.size(); i++) {
		normals[i] = normalCoords[i];
	}

	std::vector<vec3> triangles;
	triangles.push_back(vec3(3, 2, 0));
	triangles.push_back(vec3(0, 1, 2));

	triangles.push_back(vec3(19, 16, 17));
	triangles.push_back(vec3(17, 19, 18));

	triangles.push_back(vec3(14, 15, 12));
	triangles.push_back(vec3(12, 14, 13));

	triangles.push_back(vec3(21, 20, 23));
	triangles.push_back(vec3(23, 22, 21));

	triangles.push_back(vec3(6, 5, 4));
	triangles.push_back(vec3(7, 6, 4));

	triangles.push_back(vec3(8, 11, 9));
	triangles.push_back(vec3(9, 10, 11));

	std::vector<unsigned short> indexVector;

	for (unsigned int i = 0; i < triangles.size(); i++) {
		indexVector.push_back(triangles[i][0]);
		indexVector.push_back(triangles[i][1]);
		indexVector.push_back(triangles[i][2]);
	}
	numIndices = (int)indexVector.size();
	indices = new unsigned short[numIndices];
	for (unsigned int i = 0; i < indexVector.size(); i++) {
		indices[i] = indexVector[i];
	}

	
	/*
	vertices[0] = -0.5f; vertices[1] = 0.5f; vertices[2] = 0.5f; vertices[3] = 1; // left top front
	vertices[4] = 0.5f; vertices[5] = 0.5f; vertices[6] = 0.5f; vertices[7] = 1; // right top front
	vertices[8] = 0.5f; vertices[9] = -0.5f; vertices[10] = 0.5f; vertices[11] = 1; // right bottom front
	vertices[12] = -0.5f; vertices[13] = -0.5f; vertices[14] = 0.5f; vertices[15] = 1; // left bottom front
	vertices[16] = -0.5f; vertices[17] = 0.5f; vertices[18] = -0.5f; vertices[19] = 1; // left top back
	vertices[20] = 0.5f; vertices[21] = 0.5f; vertices[22] = -0.5f; vertices[23] = 1; // right top back
	vertices[24] = 0.5f; vertices[25] = -0.5f; vertices[26] = -0.5f; vertices[27] = 1; // right bottom back
	vertices[28] = -0.5f; vertices[29] = -0.5f; vertices[30] = -0.5f; vertices[31] = 1; // left bottom back
	*/

}
