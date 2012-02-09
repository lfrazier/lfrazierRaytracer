// HW 5: Raytracer

#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include "SceneGraph.h"
#include <string>
#include <cstring>
#include "Mesh.h"
#include "Raytracer.h"

using namespace glm;
using namespace std;

/////////////////////////////////////////////////////////
SceneGraph *sgraph;
Raytracer *raytracer;
float matNums[] = {0.7, 0.7, 0.2, 0, 0, 0, 0, 0};
vector<float> standardMAT = vector<float>(matNums, matNums+8);;
/////////////////////////////////////////////////////////

//vertex arrays needed for drawing
unsigned int vbo;
unsigned int cbo;
unsigned int nbo;
unsigned int ibo;

//attributes
unsigned int positionLocation = 0;
unsigned int colorLocation = 1;
unsigned int normalLocation = 2;

//uniforms
unsigned int u_modelMatrixLocation;
unsigned int u_projMatrixLocation;
	
//needed to compile and link and use the shaders
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

//Animation/transformation stuff
clock_t old;
float rotation = 0.0f;
float transX = 0.0f;
float transY = 0;
float transZ = 0;
float scaleX = 1;
float scaleY = 1;
float scaleZ = 1;

Cabinet *cabinet = new Cabinet();
Chair *chair = new Chair();
Lamp *lamp = new Lamp();
Table *table = new Table();

//helper function to read shader source and put it in a char array
//thanks to Swiftless
char* textFileRead(const char*);

//some other helper functions from CIS 565
void printLinkInfoLog(int);
void printShaderInfoLog(int);

//standard glut-based program functions
void init(void);
void resize(int, int);
void display(void);
void keypress(unsigned char, int, int);
void cleanup(void);

void createRedSquare(mat4);
void createBlueSquare(mat4);

char *configFilename;
char *raytracerConfigFilename;
void drawSceneGraph();
void createSceneGraph();
Node *selectedNode;
glm::mat4 nextTransform;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Lauren Frazier");

	//Call GLEW only _after_ you get the window
	//I had to tell the author of your textbook that ;-)  -Cory
	glewInit();

	configFilename = argv[1];
	raytracerConfigFilename = argv[2];
	raytracer = new Raytracer(raytracerConfigFilename);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keypress);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}

void init() {
	//Create the VBOs and IBO we'll be using to render images in OpenGL
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);
	
	//Everybody does this
	glClearColor(0.5, 0.5, 0.5, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	//here is stuff for setting up our shaders
	const char* fragFile = "diffuseFrag.frag";
	const char* vertFile = "diffuseVert.vert";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	
	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	} 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	//to ask OpenGL for them
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");

	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);

	resize(1040, 768);
	old = clock();

	// Create scene Graph
	createSceneGraph();
}

void createSceneGraph() {
	if (configFilename == NULL)
		return;
	ifstream file(configFilename);
	//ifstream file("HW4 scene 2.txt");
	string line;
	if (file.is_open()){
		getline(file, line);
		char *pEnd;
		int xSize, zSize, numItems;
		xSize = strtol(line.c_str(), &pEnd, 10);
		zSize = strtol(pEnd, &pEnd, 10);
		numItems = strtol(pEnd, NULL, 10);

		sgraph = new SceneGraph(xSize, zSize, numItems);

		for (int i = 0; i < numItems; i++) {
			getline(file, line);
			getline(file, line);
			string furnType = line;

			int matIndex;
			float rotation, xScale, yScale, zScale;
			int xIndex, zIndex;
			string filenameDAT;
			string filenameBMP;

			getline(file, line);

			if(furnType.compare("mesh") == 0){
				filenameDAT = line;
				getline(file, line);
			}

			matIndex = strtol(line.c_str(), &pEnd, 10);

			getline(file, line);
			xIndex = strtol(line.c_str(), &pEnd, 10);
			zIndex = strtol(pEnd, &pEnd, 10);

			getline(file, line);
			rotation = strtod(line.c_str(), NULL);

			getline(file, line);
			xScale = strtod(line.c_str(), &pEnd);
			yScale = strtod(pEnd, &pEnd);
			zScale = strtod(pEnd, &pEnd);

			Furniture *furn = NULL;
			Mesh *mesh = NULL;
			if (furnType.compare("table") == 0) {
				furn = table;
				mesh = NULL;
			} else if (furnType.compare("chair") == 0) {
				furn = chair;
				mesh = NULL;
			} else if (furnType.compare("cabinet") == 0) {
				furn = cabinet;
				mesh = NULL;
			} else if (furnType.compare("lamp") == 0) {
				furn = lamp;
				mesh = NULL;
			} else if (furnType.compare("mesh") == 0) {
				furn = NULL;
				mesh = new Mesh(filenameDAT, filenameBMP);
			}
			//vector<float> MAT;
			Node *node;
			if(matIndex == 1) {
				//MAT = raytracer->MAT1;
				node = new Node(furn, mesh, xIndex, zIndex, rotation, xScale, yScale, zScale, raytracer->MAT1);
			} else if (matIndex == 2) {
				//MAT = raytracer->MAT2;
				node = new Node(furn, mesh, xIndex, zIndex, rotation, xScale, yScale, zScale, raytracer->MAT2);
			} else if (matIndex == 3) {
				//MAT = raytracer->MAT3;
				node = new Node(furn, mesh, xIndex, zIndex, rotation, xScale, yScale, zScale, raytracer->MAT3);
			} else {
				//MAT = standardMAT;
				node = new Node(furn, mesh, xIndex, zIndex, rotation, xScale, yScale, zScale, standardMAT);
			}
			//Node *node = new Node(furn, mesh, xIndex, zIndex, rotation, xScale, yScale, zScale, MAT);
			sgraph->addNode(node);
		}

		file.close();
	}
	else {
		printf("Error reading config file\n");
	}
	selectedNode = sgraph->root;
	sgraph->root->selected = true;
}

void cleanup() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	//Tear down the shader program in reverse of building it
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

void keypress(unsigned char key, int x, int y) {
	switch(key) {
	case 'p':
		printf("RAYTRACE!\n");
		raytracer->beginRender(sgraph->preorder);
		printf("DONE!\n");
		break;
	case 'q':
		cleanup();
		exit(0);
		break;
	case 'n':
		//selectedNode = selectedNode
		selectedNode->selected = false;
		int i;
		for (i = 0; i < sgraph->preorder.size(); i++) {
			if (sgraph->preorder[i] == selectedNode) {
				break;
			}
		}
		if (i == sgraph->preorder.size() - 1){
			selectedNode = sgraph->root;
		} else {
			selectedNode = sgraph->preorder[i+1];
		}
		selectedNode->selected = true;
		break;
	case 'a':
		transX -= 0.5;
		selectedNode->transform = glm::translate(selectedNode->transform, glm::vec3(transX, 0, transZ));
		break;
	case 'd':
		transX += 0.5;
		selectedNode->transform = glm::translate(selectedNode->transform, glm::vec3(transX, 0, transZ));
		break;
	case 'w':
		transZ += 0.5;
		selectedNode->transform = glm::translate(selectedNode->transform, glm::vec3(transX, 0, transZ));
		break;
	case 's':
		transZ -= 0.5;
		selectedNode->transform = glm::translate(selectedNode->transform, glm::vec3(transX, 0, transZ));
		break;
	case 'x':
		scaleX += 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'X':
		scaleX -= 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'y':
		scaleY += 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'Y':
		scaleY -= 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'z':
		scaleZ += 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'Z':
		scaleZ -= 0.5;
		selectedNode->transform = glm::scale(selectedNode->transform, glm::vec3(scaleX, scaleY, scaleZ));
		break;
	case 'r':
		rotation += 10;
		selectedNode->transform = glm::rotate(selectedNode->transform, rotation, glm::vec3(0,1,0));
		break;
	case 'R':
		rotation -= 10;
		selectedNode->transform = glm::rotate(selectedNode->transform, rotation, glm::vec3(0,1,0));
		break;
	case 'e':
		selectedNode = sgraph->removeNode(selectedNode);
		selectedNode->selected = true;
		break;
	}

	glutPostRedisplay();
}


void resetParams() {
	transX = 0;
	transY = 0;
	transZ = 0;
	rotation = 0;
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
}

void drawSceneGraph() {
	sgraph->updatePreorder();
	sgraph->root->traverse(glm::mat4(1), vbo, cbo, nbo, ibo, positionLocation, colorLocation, normalLocation, u_modelMatrixLocation);
	resetParams();
}

void display() {
	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	clock_t newTime = clock();

	//part of the animation
	//rotation += 150 * (static_cast<float>(newTime - old) / static_cast<float>(CLOCKS_PER_SEC));

	//create an identity matrix for the modelview matrix
	glm::mat4 modelView = glm::mat4(1.0);

	//Draw the two components of our scene separately, for your scenegraphs it will help
	//your sanity to do separate draws for each type of primitive geometry, otherwise your VBOs will
	//get very, very complicated fast
	//createRedSquare(modelView);	//PASS MODELVIEW BY COPY to get the nice scoping for hierarchical (i'm sure i spelled that wrong) transformations
								//like Norm mentioned in his Scene Graph lecture
	//createBlueSquare(modelView);
	drawSceneGraph();

	glutSwapBuffers();
	old = newTime;
}

void createRedSquare(mat4 modelView) {
	//set up a transformation matrix, in this case we're doing rotation only
	modelView = glm::rotate(modelView, rotation, glm::vec3(0, 1, 0));

	//these four points define where the quad would be BEFORE transformations
	//this is referred to as object-space and it's best to center geometry at the origin for easier transformations
	float* vertices = new float[16];
	vertices[0] = -4.0f; vertices[1] = 4.0f; vertices[2] = -1.0f; vertices[3] = 1.0f;
	vertices[4] = -4.0f; vertices[5] = -4.0f; vertices[6] = -1.0f; vertices[7] = 1.0f;
	vertices[8] = 4.0f; vertices[9] = -4.0f; vertices[10] = -1.0f; vertices[11] = 1.0f;
	vertices[12] = 4.0f; vertices[13] = 4.0f; vertices[14] = -1.0f; vertices[15] = 1.0f;

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
																				 //while the program runs

	//once the data is loaded, we can delete the float arrays, the data is safely stored with OpenGL
	delete [] vertices;

	//again with colors
	float* colors = new float[12];
	
	colors[0] = 1; colors[1] = 0; colors[2] = 0;
	colors[3] = 1; colors[4] = 0; colors[5] = 0;
	colors[6] = 1; colors[7] = 0; colors[8] = 0;
	colors[9] = 1; colors[10] = 0; colors[11] = 0;

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
																				//as it bounces between squares
	delete [] colors;

	//once more, this time with normals
	float* normals = new float[16];
	
	normals[0] = 0; normals[1] = 0; normals[2] = 1; normals[3] = 0;
	normals[4] = 0; normals[5] = 0; normals[6] = 1; normals[7] = 0;
	normals[8] = 0; normals[9] = 0; normals[10] = 1; normals[11] = 0;
	normals[12] = 0; normals[13] = 0; normals[14] = 1; normals[15] = 0;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
																				 //while the program runs
	delete [] normals;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[6];

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 0; indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void createBlueSquare(mat4 modelView) {
	//two transformations put into the modelview matrix
	modelView = glm::translate(modelView, glm::vec3(-2.0f, 2.0f, -0.1f));
	modelView = glm::rotate(modelView, -rotation, glm::vec3(0, 1, 0));

	//the only difference between a red square and a blue square is the color, so we can leave the other VBOs as they are
	float* colors = new float[12];
	
	colors[0] = 0; colors[1] = 0; colors[2] = 1;
	colors[3] = 0; colors[4] = 0; colors[5] = 1;
	colors[6] = 0; colors[7] = 0; colors[8] = 1;
	colors[9] = 0; colors[10] = 0; colors[11] = 1;

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);
	
	delete [] colors;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//bind again
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//set the modelview matrix again since it changed
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw again, even the indices from before are good
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void resize(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);

	//
	glm::mat4 projection = glm::perspective(60.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 30.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0, 2, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = projection * camera;

	//set the projection matrix here, it only needs to be changed if the screen is resized otherwise it can stay the same
	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &projection[0][0]);

	glutPostRedisplay();
}

//from swiftless.com
char* textFileRead(const char* fileName) {
    char* text;
    
    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
            }
            fclose(file);
        }
    }
    return text;
}

void printLinkInfoLog(int prog) 
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}
}

void printShaderInfoLog(int shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}