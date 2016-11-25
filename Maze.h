#ifndef Maze_H
#define Maze_H
#include "Cell.h"
#include <stdio.h>
#include "time.h"
#include <stack>

//using namespace std;

class Maze
{
public:
	Maze();
	Maze(int input);
	~Maze();
	int getSize();
	void toString();
	int startX, startY, endX, endY;

private: 
	int size = 0;
	bool checkNull(int in[4][2]);
	int randMaze();
	Cell* grid[500][500];
	void initGrid();
	void path();
	void walk(int x, int y, stack<Cell*> s);
	int* getValidCell(int x, int y);
	bool isValid(int x, int y);
	bool inRange(int x, int y);
};
#endif