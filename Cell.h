#ifndef Cell_H
#define Cell_H

#include <stdio.h>
#include <string>
using namespace std;

class Cell
{
public:
	Cell(int a, int b);
	~Cell();
	void changeVisit(bool input);
	void changeUp(bool input);
	void changeDown(bool input);
	void changeLeft(bool input);
	void changeRight(bool input);
	int getX();
	int getY();
	bool getVisit();
	bool getUp();
	bool getDown();
	bool getLeft();
	bool getRight();
	string toString();


private: 
	int x, y;
	bool up, down, right, left, visit;
};
#endif 