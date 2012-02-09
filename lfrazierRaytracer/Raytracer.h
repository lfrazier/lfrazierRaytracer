#ifndef Raytracer_h
#define Raytracer_h

#include "Camera.h"
#include "EasyBMP.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Node.h"

using namespace std;
using namespace glm;

class Raytracer {
public:
	// Properties from config file
	char FILE[30];
	int RESOx;
	int RESOy;
	vec3 EYEP;
	vec3 VDIR;
	vec3 UVEC;
	float FOVY;
	vec3 LPOS;
	vec3 LCOL;
	vec3 ACOL;
	vector<float> MAT1, MAT2, MAT3;

	// Camera and bitmap
    Camera *cam;
    BMP output;

	int depth;
    
	Raytracer(char *configFilename);
	vector<Node *> preorderList;

	// Functions
    void beginRender(vector<Node *> preorderList);
    vec3 march(Ray ray, int x, int y, int bouncesLeft);
	bool inShadow(Ray ray);
};


#endif