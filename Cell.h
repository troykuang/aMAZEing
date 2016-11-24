#ifndef Cell_H
#define Cell_H

#include <stdio.h>
#include <string>
using namespace std;

class Cell
{
public:
	Cell();
	~Cell();
	void changeVisit(bool input);
	void changeUp(bool input);
	void changeDown(bool input);
	void changeLeft(bool input);
	void changeRight(bool input);
	bool getVisit();
	bool getUp();
	bool getDown();
	bool getLeft();
	bool getRight();
	string toString();


private: 
	bool up, down, right, left, visit;
};
#endif 