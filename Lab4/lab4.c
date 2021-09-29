// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <time.h>
#include <stdlib.h>
// L�gg till egna globaler h�r efter behov.
float kMaxDistance = 1000;
float kAlignmentWeight = 0.0001;
float kCohesionWeight = 0.0001;
float kAvoidanceWeight = 0.009;
float kFoodWeight = 0.0001;
TextureData* foodFace;

FPoint CalcAvoidance(SpritePtr i, SpritePtr j, float distance){

	FPoint avoidance;
	avoidance.h = i->position.h - j->position.h;
	avoidance.v = i->position.v - j->position.v;
	float falloff = (1-(distance / sqrt(kMaxDistance)))/distance;
	avoidance.h *= falloff;
	avoidance.v *= falloff;
	return avoidance;

}
void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.
SpritePtr i = gSpriteRoot;
SpritePtr food = gFoodSpriteRoot;
int count;
while(i != NULL){
	count = 0;
	i->speedDiff = (FPoint){0,0};
	i->averagePosition = (FPoint){0,0};
	i->avoidanceVector = (FPoint){0,0};

	SpritePtr j = gSpriteRoot;
	while(j != NULL){
		if(j != i ){
			FPoint between;
			//horizontal (h) and vertical (v)
			between.v = j->position.v - i->position.v;
			between.h = j->position.h - i->position.h;

			float distance = sqrt(pow(between.v, 2) + pow(between.h, 2));
			
			if(distance < kMaxDistance){
				i->speedDiff.h += j->speed.h - i->speed.h;
				i->speedDiff.v += j->speed.v - i->speed.v;

				i->averagePosition.h += j->position.h - i->position.h;
				i->averagePosition.v += j->position.v - i->position.v;
			
				FPoint avoidance = CalcAvoidance(i, j, distance);
				i->avoidanceVector.h = avoidance.h;
				i->avoidanceVector.v = avoidance.v;
				//TODO: avoidance
				count += 1;
			}
		}
		j = j->next;
	}
	if(count > 0){
		i->speedDiff.h /= count;
		i->speedDiff.v /= count;

		i->averagePosition.h /= count;
		i->averagePosition.v /= count;

 		i->avoidanceVector.h /= count;
        i->avoidanceVector.v /= count;
		//TODO: avoidance
	}

	
	i = i->next;
}
//Second loop
i = gSpriteRoot;
while(i != NULL){
	if(food != NULL){
		FPoint foodVec;
		foodVec.h = food->position.h - i->position.h;
		foodVec.v = food->position.v - i->position.v;

		float distanceToFood = sqrt(pow(foodVec.v, 2) + pow(foodVec.h, 2));
		if(distanceToFood < 50){
			gFoodSpriteRoot = NULL;
		}
		i->speed.h += foodVec.h * kFoodWeight;
		i->speed.v += foodVec.v * kFoodWeight;
	}
	else{
		i->speed.h += i->speedDiff.h * kAlignmentWeight + i->averagePosition.h * kCohesionWeight + i->avoidanceVector.h * kAvoidanceWeight;
		i->speed.v += i->speedDiff.v * kAlignmentWeight + i->averagePosition.v * kCohesionWeight + i->avoidanceVector.v * kAvoidanceWeight;
	}
	

	//black sheeps
	if(i->anomaly){
		i->speed.h += (float)rand()/(float)(RAND_MAX)/10;
		i->speed.v += (float)rand()/(float)(RAND_MAX)/10;
	}
	

	i->position.h += i->speed.h;
	i->position.v += i->speed.v;

	i = i->next;
}
}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	//food
	sp = gFoodSpriteRoot;
	if(sp != NULL){
		DrawSprite(sp);
	}
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	srandom(time(NULL));
	
	for (size_t i = 0; i < 20; i++)
	{
		// printf("%f \n", (float)rand()/(float)(RAND_MAX/6));
		NewSprite(sheepFace, random() % 800 + 1, random() % 600 + 1, random() % 1 - 1, random() % 1 - 1, false);
	}
	for (size_t i = 0; i < 3; i++)
	{
		NewSprite(blackFace, random() % 800 + 1, random() % 600 + 1, random() % 1 - 1, random() % 1 - 1, true);
	}

	
}
int prevx = 0, prevy = 0;

void mouseUpDown(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
		NewFoodSprite(foodFace, prevx, 600-prevy);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutMouseFunc(mouseUpDown);
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
