// Revised 2019 with a bit better variable names.

// This worked 2015: 
// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux
// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

//constants
const int initWidth=512, initHeight=512;

// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 bunnyToWorld = {{ 	1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0}};
mat4 teapotToWorld = {{ 	1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0}};
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 worldToView;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

// Globals
// * Model(s)
Model *bunny;
Model *teapot;
// * Reference(s) to shader phongShader(s)
GLuint phongShader, texShader;
// * Texture(s)
GLuint texture;

void init(void)
{
	dumpInfo();
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits"); // This is merely a vague indication of where something might be wrong

	projectionMatrix = perspective(90, 1.0, 0.1, 1000);
	worldToView = lookAt(0,0,1.5, 0,0,0, 0,1,0);

	// Load and compile shader
	phongShader = loadShaders("lab0.vert", "lab0.frag");
	texShader = loadShaders("tex.vert", "tex.frag");

	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/stanford-bunny.obj");
	teapot = LoadModelPlus("objects/teapot.obj");
	bunnyToWorld = Mult(bunnyToWorld, T(0.5,0.0,0.0));
	teapotToWorld = Mult(teapotToWorld, T(-0.5,0.0,0.0));
	teapotToWorld = Mult(teapotToWorld, S(0.5,0.5,0.5));
	printError("load models");
	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&texture);
	printError("load textures");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the phongShader, and set its variables
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	bunnyToWorld = Mult(bunnyToWorld, Ry(0.01));
	mat4 modelToWorldToView = Mult(worldToView, bunnyToWorld); // Combine to one matrix
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
	DrawModel(bunny, phongShader, "in_Position", "in_Normal", NULL);
	
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	teapotToWorld = Mult(teapotToWorld, Ry(-0.06));
	modelToWorldToView = Mult(worldToView, teapotToWorld); // Combine to one matrix
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
	DrawModel(teapot, texShader, "in_Position", "in_Normal","in_TexCoord");
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.2+ Introduction");
	glutDisplayFunc(display); 
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}

