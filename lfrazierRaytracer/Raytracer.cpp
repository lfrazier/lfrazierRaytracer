#include "Raytracer.h"
#include "intersection.h"

float EPSILON = 0.01;

Raytracer::Raytracer(char *configFilename)
{
	ifstream file(configFilename);
	if (file.is_open()){
		string line;
		char *pEnd;
		// FILE
		getline(file, line);
		char lineCopy[140];
        strcpy(lineCopy, line.c_str());
        char *filename = strtok(lineCopy, " ");
		filename = strtok(NULL, " ");
		strcpy(this->FILE, filename);

		// RESO
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        char *token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->RESOx = atoi(token);
		token = strtok(NULL, " ");
		this->RESOy = atoi(token);

		// EYEP
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->EYEP.x = atof(token);
		token = strtok(NULL, " ");
		this->EYEP.y = atof(token);
		token = strtok(NULL, " ");
		this->EYEP.z = atof(token);

		// VDIR
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->VDIR.x = atof(token);
		token = strtok(NULL, " ");
		this->VDIR.y = atof(token);
		token = strtok(NULL, " ");
		this->VDIR.z = atof(token);

		// UVEC
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->UVEC.x = atof(token);
		token = strtok(NULL, " ");
		this->UVEC.y = atof(token);
		token = strtok(NULL, " ");
		this->UVEC.z = atof(token);

		// FOVY
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->FOVY = atof(token);

		// LPOS
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->LPOS.x = atof(token);
		token = strtok(NULL, " ");
		this->LPOS.y = atof(token);
		token = strtok(NULL, " ");
		this->LPOS.z = atof(token);

		// LCOL
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->LCOL.x = atof(token);
		token = strtok(NULL, " ");
		this->LCOL.y = atof(token);
		token = strtok(NULL, " ");
		this->LCOL.z = atof(token);

		// ACOL
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		token = strtok(NULL, " ");
		this->ACOL.x = atof(token);
		token = strtok(NULL, " ");
		this->ACOL.y = atof(token);
		token = strtok(NULL, " ");
		this->ACOL.z = atof(token);

		// MAT1
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		for(int i = 0; i < 8; i++) {
			token = strtok(NULL, " ");
			this->MAT1.push_back(atof(token));
		}

		// MAT2
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		for(int i = 0; i < 8; i++) {
			token = strtok(NULL, " ");
			this->MAT2.push_back(atof(token));
		}

		// MAT3
		getline(file, line);
        strcpy(lineCopy, line.c_str());
        token = strtok(lineCopy, " ");
		for(int i = 0; i < 8; i++) {
			token = strtok(NULL, " ");
			this->MAT3.push_back(atof(token));
		}
	}

	this->cam = new Camera(this->RESOx, this->RESOy, this->EYEP, this->VDIR, this->UVEC, this->FOVY);
	this->depth = 5;
}

void Raytracer::beginRender(vector<Node *> pList)
{
	this->preorderList = pList;

	output.SetSize(this->RESOx, this->RESOy);
	output.SetBitDepth(24);

	for (int y = 0; y < RESOy; y++) {
		if (y%50 == 0) { cout << "Y: " << y << endl; } // Progress bar
		int flippedY = RESOy - y - 1;
		for (int x = 0; x < RESOx; x++) {
			Ray ray = cam->getWorldRayForPixel(x, y);

			// RAY DIRECTION IMAGE
			/*output(x,flippedY)->Red = fabs((ray.direction.x) * 255);
            output(x,flippedY)->Green = fabs((ray.direction.y) * 255);
            output(x,flippedY)->Blue = fabs((ray.direction.z) * 255);
			*/

			vec3 finalColor = march(ray, x, y, depth-1);
			output(x,flippedY)->Red = glm::clamp(finalColor.x * 255, 0.0f, 255.0f);
            output(x,flippedY)->Green = glm::clamp(finalColor.y * 255, 0.0f, 255.0f);
            output(x,flippedY)->Blue = glm::clamp(finalColor.z * 255, 0.0f, 255.0f);
		}
	}
	output.WriteToFile(this->FILE);
}

vec3 Raytracer::march(Ray ray, int x, int y, int bouncesLeft)
{
	vec3 C = vec3(0,0,0);

	double minT = 99999999;
	Node *frontNode = NULL;
	Primitive *frontComponent = NULL;
	vec3 normal;
	for (int i = 0; i < preorderList.size(); i++) {
		Node *node = preorderList[i];
		if (node->furn != NULL) {
			for (int j = 0; j < node->furn->components.size(); j++) {
				mat4 invT = glm::inverse(node->transform * node->furn->components[j]->transform);
				//mat4 invT = glm::inverse(node->furn->components[j]->transform * node->transform);
				string compName = node->furn->components[j]->name;
				double t = 0;
				if (compName.compare("cube") == 0) {
					t = RayCubeIntersect(ray.origin, ray.direction, invT);
				} else if (compName.compare("cylinder") == 0) {
					t = RayCylinderIntersect(ray.origin, ray.direction, invT);
				} else if (compName.compare("sphere") == 0) {
					t = RaySphereIntersect(ray.origin, ray.direction, invT);
				}

				if (t < minT && t != -1 ) {
					minT = t;
					frontComponent = node->furn->components[j];
					frontNode = node;
				}
			}
		} else if (node->mesh != NULL) {
			double boundingT = RaySphereIntersect(ray.origin, ray.direction, glm::inverse(node->transform * node->mesh->boundingSphere->transform));
			if (boundingT == -1) {
				continue;
			}
			
			Triangle frontTriangle;
			for (int j = 0; j < node->mesh->triangles.size(); j++) {
				mat4 invT = glm::inverse(node->transform * node->mesh->transform);
				double t = RayPolyIntersect(ray.origin, ray.direction, 
					node->mesh->triangles[j].pt1, 
					node->mesh->triangles[j].pt2,
					node->mesh->triangles[j].pt3,
					invT);
				if (t < minT && t != -1 ) {
					minT = t;
					frontTriangle = node->mesh->triangles[j];
					frontNode = node;
					frontComponent = NULL;
					normal = frontTriangle.normal;
				}
			}
		}
	}
	if(frontNode != NULL) {
		C.x = frontNode->MAT[0];
		C.y = frontNode->MAT[1];
		C.z = frontNode->MAT[2];


		//calculate diffuse term and clamp to the range [0, 1]
		vec3 pointPos = ray.origin + ((float)minT * ray.direction);
		vec3 lightVector = LPOS - pointPos; // point to light

		// Shadows
		if(inShadow(Ray(pointPos + (EPSILON * lightVector), lightVector))) {
			return ACOL;
		}

		if (frontComponent != NULL) {
			if (frontComponent->name.compare("cube") == 0) {
				mat4 T = frontNode->transform * frontComponent->transform;
				mat4 invT = glm::inverse(T);
				vec4 point4 = vec4(pointPos.x, pointPos.y, pointPos.z, 1.0f);
				vec4 cubeSpaceVec = invT * point4;
				vec4 normal4;

				/*float maxMagnitudeComponent;
				if(fabs(cubeSpaceVec.x) > fabs(cubeSpaceVec.y)){
					maxMagnitudeComponent = cubeSpaceVec.x;
				} else {
					maxMagnitudeComponent = cubeSpaceVec.y;
				}
				if (fabs(cubeSpaceVec.z) > fabs(maxMagnitudeComponent)) {
					maxMagnitudeComponent = cubeSpaceVec.z;
				}
				if(maxMagnitudeComponent == cubeSpaceVec.x) {
					normal4 = vec4(cubeSpaceVec.x / fabs(cubeSpaceVec.x), 0, 0, 0);
				} else if(maxMagnitudeComponent == cubeSpaceVec.y) {
					normal4 = vec4(0, cubeSpaceVec.y / fabs(cubeSpaceVec.y), 0, 0);
				} else if(maxMagnitudeComponent == cubeSpaceVec.z) {
					normal4 = vec4(0, 0, cubeSpaceVec.z / fabs(cubeSpaceVec.z), 0);
				}*/
				
				if (fabs(cubeSpaceVec.x - 0.5) < EPSILON) {
					normal4 = vec4(1, 0, 0, 0);
					//printf("Right\n");
				} else if (fabs(cubeSpaceVec.x + 0.5) < EPSILON) {
					normal4 = vec4(-1, 0, 0, 0);
					//printf("Left\n");
				} else if (fabs(cubeSpaceVec.y - 0.5) < EPSILON) {
					normal4 = vec4(0, 1, 0, 0);
					//printf("Top\n");
				} else if (fabs(cubeSpaceVec.y + 0.5) < EPSILON) {
					normal4 = vec4(0, -1, 0, 0);
					//printf("Bottom\n");
				} else if (fabs(cubeSpaceVec.z - 0.5) < EPSILON) {
					normal4 = vec4(0, 0, 1, 0);
					//printf("Front\n");
				} else if (fabs(cubeSpaceVec.z + 0.5) < EPSILON) {
					normal4 = vec4(0, 0, -1, 0);
					//printf("Back\n");
				}
				normal4.w = 0;
				normal4 = T * normal4;
				normal.x = normal4.x;
				normal.y = normal4.y;
				normal.z = normal4.z;
			} else if (frontComponent->name.compare("sphere") == 0) {
				mat4 T = frontNode->transform * frontComponent->transform;
				mat4 invT = glm::inverse(T);
				vec4 point4 = vec4(pointPos.x, pointPos.y, pointPos.z, 1.0f);
				vec4 normal4 = invT * point4;
				normal4.w = 0;

				normal4 = T * normal4;
				
				normal.x = normal4.x;
				normal.y = normal4.y;
				normal.z = normal4.z;

				//return normal;
			} else if (frontComponent->name.compare("cylinder") == 0) {
				mat4 T = frontNode->transform * frontComponent->transform;
				mat4 invT = glm::inverse(T);
				vec4 point4 = vec4(pointPos.x, pointPos.y, pointPos.z, 1.0f);
				vec4 normal4 = invT * point4;
				if (fabs(normal4.y - 0.5) < EPSILON) {
					normal4 = T * vec4(0, 1, 0, 0);
				} else if (fabs(normal4.y + 0.5) < EPSILON) {
					normal4 = T * vec4(0, -1, 0, 0);
				} else {
					normal4.y = 0;
					normal4.w = 0;
					normal4 = T * normal4;
				}
				
				normal.x = normal4.x;
				normal.y = normal4.y;
				normal.z = normal4.z;
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////// Debug
		//return normalize(normal);

		float dot = glm::dot(normalize(normal), normalize(lightVector));
		float diffuseTerm = clamp(dot, 0.0f, 1.0f);

		vec3 reflect = glm::reflect(normalize(-1.0f * lightVector), normalize(normal));
		float base = glm::dot(normalize(reflect), normalize(EYEP));
		base = (base > 0.0f) ? base : 0.0f;
		float specular = glm::pow(base, frontNode->MAT[3]);
    
		vec3 blinnPhong = ACOL + (C * diffuseTerm) + (specular * LCOL);

		// Reflection
		vec3 finalColor;
		if (bouncesLeft == 0 || frontNode->MAT[4] == 0) {
			return blinnPhong;
		} else {
			vec3 refRay = glm::reflect(normalize(ray.direction), normalize(normal));
			vec3 reflectedColor = march(Ray(pointPos + (EPSILON * normal), normalize(refRay)), x, y, bouncesLeft - 1);
			return (frontNode->MAT[4] * reflectedColor) + ((1 - frontNode->MAT[4]) * blinnPhong);
		}
	}
	return vec3(0,0,0);
}

bool Raytracer::inShadow(Ray ray) {
	vec3 lightVec = LPOS - ray.origin;
	//ray.direction = normalize(ray.direction);
	for (int i = 0; i < preorderList.size(); i++) {
		Node *node = preorderList[i];
		if (node->furn != NULL) {
			for (int j = 0; j < node->furn->components.size(); j++) {
				mat4 invT = glm::inverse(node->transform * node->furn->components[j]->transform);
				//mat4 invT = glm::inverse(node->furn->components[j]->transform * node->transform);
				string compName = node->furn->components[j]->name;
				double t = 0;
				if (compName.compare("cube") == 0) {
					t = RayCubeIntersect(ray.origin, ray.direction, invT);
				} else if (compName.compare("cylinder") == 0) {
					t = RayCylinderIntersect(ray.origin, ray.direction, invT);
				} else if (compName.compare("sphere") == 0) {
					t = RaySphereIntersect(ray.origin, ray.direction, invT);
				}

				vec3 tRayPoint = (float)t * normalize(ray.direction);
				if (t != -1) { // Fix me! Should make sure to stop at the light. Find length of lightVector, check to see if point to t*ray is longer.
					return true;
				}
			}
		} else if (node->mesh != NULL) {
			double boundingT = RaySphereIntersect(ray.origin, ray.direction, glm::inverse(node->transform * node->mesh->boundingSphere->transform));
			if (boundingT == -1) {
				continue;
			}
			
			for (int j = 0; j < node->mesh->triangles.size(); j++) {
				mat4 invT = glm::inverse(node->transform * node->mesh->transform);
				double t = RayPolyIntersect(ray.origin, ray.direction, 
					node->mesh->triangles[j].pt1, 
					node->mesh->triangles[j].pt2,
					node->mesh->triangles[j].pt3,
					invT);
				//vec3 tRay = (float)t * normalize(ray.direction);
				if (t != -1) { // same as above
					return true;
				}
			}
		}
	}
	return false;
}