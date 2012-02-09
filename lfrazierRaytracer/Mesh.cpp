#include "Mesh.h"

using namespace std;
using namespace glm;

#define PI 3.14159265

Mesh::Mesh(string fDAT, string fBMP) {
	this->filenameDAT = new char[fDAT.size() + 1];
	this->filenameBMP = new char[fBMP.size() + 1];
	strcpy(this->filenameDAT, fDAT.c_str());
	strcpy(this->filenameBMP, fBMP.c_str());

	ifstream file(filenameDAT);
	if (file.is_open()){
		string line;
		getline(file, line);
		char *pEnd;
		if (line.compare("extrusion") == 0) {
			getline(file, line);
			float length = strtod(line.c_str(), NULL);
			getline(file, line);
			int numPoints = strtol(line.c_str(), NULL, 10);

			// Calculate points
			for (int i = 0; i < numPoints; i++) {
				getline(file, line);
				float x = strtod(line.c_str(), &pEnd);
				float z = strtod(pEnd, NULL);
				points.push_back(vec3(x, -1 * (length/2), z)); // Base
			}

			// Stop and determine convexity
			bool convex = true;
			vec3 prevNormal = getNormal(points[numPoints - 2], points[0], points[1], false);
			for (int mid = 0; mid < numPoints - 1; mid++) {
				int left = (mid != 0) ? (mid - 1) : numPoints - 2;
				int right = (mid != numPoints - 2) ? (mid + 1) : 0;
				vec3 newNormal = getNormal(points[left], points[mid], points[right], false);
				if (((newNormal.y > 0) - (newNormal.y < 0)) != ((prevNormal.y > 0) - (prevNormal.y < 0))) { // If signs of y-component are different, then the polygon is non-convex
					convex = false;
					break;
				}
				prevNormal = getNormal(points[left], points[mid], points[right], false);
			}

			// Continue pushing back
			for (int i = 0; i < numPoints; i++) {
				points.push_back(vec3(points[i].x, length /2, points[i].z)); // Top
			}
			if (convex) {
				points.insert(points.begin(), vec3(0,-1 * (length/2), 0)); // Center point
				points.push_back(vec3(0,(length/2), 0)); // Other center point
			}

			// Push back triangles
			int offset = numPoints;
			if(convex) {
				for (int i = 1; i <= numPoints; i++) {
					int j = (i != numPoints) ? (i + 1) : 1; // Ususally just i+1, unless we are at the end of the circle, then make it 1 again
					pushTriangle(points[i], points.front(), points[j], true); // If convex, push a cap triangle
					pushTriangle(points[i], points[i + offset], points[j + offset], false); // Side triangle
					pushTriangle(points[i + offset], points.back(), points[j + offset], false); // If convex, push a cap triangle
					pushTriangle(points[j + offset], points[i], points[j], true); // Other side triangle
				}
			} else {
				for (int i = 0; i < numPoints; i++) {
					int j = (i != numPoints - 1) ? (i + 1) : 0; // Ususally just i+1, unless we are at the end of the circle, then make it 1 again
					pushTriangle(points[i], points[i + offset], points[j + offset], false); // Side triangle
					pushTriangle(points[j + offset], points[i], points[j], true); // Other side triangle
				}
			}
		} else if (line.compare("surfrev") == 0) {
			getline(file, line);
			int numSlices = strtol(line.c_str(), NULL, 10);
			if (numSlices < 3) {
				cout << fDAT << " is invalid: Too few slices in surfrev\n" << endl;
			}
			getline(file, line);
			int numPoints = strtol(line.c_str(), NULL, 10);
			vector<vec3> polyline;
			// Calculate points
			for (int i = 0; i < numPoints; i++) {
				getline(file, line);
				float x = strtod(line.c_str(), &pEnd);
				float y = strtod(pEnd, NULL);
				polyline.push_back(vec3(x, y, 0)); // Base
			}
			float step = 360 / numSlices;
			float theta = 0;
			for (int i = 0; i < numPoints; i++) {
				while (theta < 360) {
					float theta_radians = theta * (PI/180);
					this->points.push_back(vec3(polyline[i].x * cos(theta_radians), polyline[i].y, polyline[i].x * sin(theta_radians)));
					theta += step;
				}
				theta = 0;
			}
			// Push back all the triangles. Is drawing upside down!!!, so must switch top and bottom points.
			int offset = numSlices;
			for (int i = 0; i < numSlices; i++) {
				int j = (i != numSlices - 1) ? (i + 1) : 0; // Ususally just i+1, unless we are at the end of the circle, then make it 0 again
				int bottomLevel = (numPoints- 1);
				for (int f = 0; f < bottomLevel; f++) {
					pushTriangle(points[i + (f * offset)], points[j + (f * offset)], points[i + ((f+1) * offset)], false);
				}
				for (int g = bottomLevel; g > 0; g--) {
					pushTriangle(points[j + (g * offset)], points[i + (g * offset)], points[j + ((g-1) * offset)], false);
				}
			}
		} else {
			cout << fDAT << " is invalid: Not 'extrusion' or 'surfrev'\n" << endl;
		}
	}
	prepareVBO();
	computeBoundingSphere();
}

void Mesh::computeBoundingSphere()
{
	float minX = 99999; 
	float minY = 99999;
	float minZ = 99999;
	float maxX = 0;
	float maxY = 0;
	float maxZ = 0;

	for (int i =0; i < points.size(); i++) {
		if (points[i].x < minX) {
			minX = points[i].x;
		} else if (points[i].y < minY) {
			minY = points[i].y;
		} else if (points[i].z < minZ) {
			minZ = points[i].z;
		}

		if (points[i].x > maxX) {
			maxX = points[i].x;
		} else if (points[i].y > maxY) {
			maxY = points[i].y;
		} else if (points[i].z > maxZ) {
			maxZ = points[i].z;
		}
	}

	float halfXLength = ((maxX - minX)/2);
	float halfYLength = ((maxY - minY)/2);
	float halfZLength = ((maxZ - minZ)/2);
	float centerX = minX + halfXLength;
	float centerY = minY + halfYLength;
	float centerZ = minZ + halfZLength;

	float maxLength;
	if (halfXLength > halfYLength) {
		maxLength = halfXLength;
	} else {
		maxLength = halfYLength;
	}
	if (maxLength < halfZLength) {
		maxLength = halfZLength;
	}
	float radius = maxLength * sqrt(2.0f);

	mat4 transform = translate(glm::mat4(1), vec3(centerX, centerY, centerZ));
	transform = scale(transform, vec3(radius));

	boundingSphere = new Sphere(transform);
}

void Mesh::prepareVBO() {
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
void Mesh::pushTriangle(vec3 point0, vec3 point1, vec3 point2, bool backwards) {
		triangles.push_back(Triangle(point0, point1, point2, getNormal(point0, point1, point2, backwards)));

		vertVec.push_back(point0);
		vertVec.push_back(point1);
		vertVec.push_back(point2);

		for (int i = 0; i < 3; i++) {
			normVec.push_back(getNormal(point0, point1, point2, backwards));
		}
}

// Takes 3 points, and an option to flip the normal backwards before returning it.
vec3 Mesh::getNormal(vec3 Q0, vec3 Q1, vec3 Q2, bool backwards) {
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