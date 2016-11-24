#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "Cell.h"
#include <iostream>
#include <string>
using namespace std;

Cell::Cell(){
	visit = false;//not visit yet
	//true for has wall
	up = true;
	down = true;
	left = true;
	right = true;
}

Cell::~Cell(){
	delete this;
}

void Cell::changeVisit(bool input){
	visit = input;
}

void Cell::changeUp(bool input){
	up = input;
}

void Cell::changeDown(bool input){
	down = input;
}

void Cell::changeLeft(bool input){
	left = input;
}

void Cell::changeRight(bool input){
	right = input;
}

bool Cell::getVisit() {
	return visit;
}

bool Cell::getUp() {
	return up;
}

bool Cell::getDown() {
	return down;
}

bool Cell::getLeft() {
	return left;
}

bool Cell::getRight() {
	return right;
}

//This toString() is only for testing Cell
string Cell::toString() {
	string result = "";
	if (up) result += "++\n";
	if (left) result += "|";
	result += " ";
	if (right) result += "|";
	result += "\n";
	if (down) result += "++";
	return result; 
}

int main(int argc, char const *argv[])
{
	Cell* c = new Cell();
	cout << c->toString();
	//printf("%s\n", c->toString());
	return 0;
}