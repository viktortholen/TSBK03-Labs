all :  lab1-1

lab1-1: lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/LittleOBJLoader.c ../common/Linux/MicroGlut.c
	gcc -Wall -o lab1-1 -DGL_GLEXT_PROTOTYPES lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/LittleOBJLoader.c ../common/Linux/MicroGlut.c -I../common -I../common/Linux -lX11 -lm -lGL

clean :
	rm lab1-1

