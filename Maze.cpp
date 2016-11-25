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
#include "Maze.h"
#include <iostream>
#include <ctime>

Maze::Maze() {
	size = 50;
	srand(time(NULL));
	startX = randMaze();
	startY = randMaze();
	do {
		endX = randMaze();
		endY = randMaze();
	} while (endX == startX && endY == startY);
	initGrid();
	path();
}

Maze::Maze(int input) {
	size = input;
	srand(time(NULL));
	startX = randMaze();
	startY = randMaze();

	do {
		endX = randMaze();
		endY = randMaze();
	} while (endX == startX && endY == startY);
	initGrid();
	path();

}

Maze::~Maze(){
	delete this;
}

int Maze::getSize(){
	return size;
}

void Maze::path() {
	stack<Cell*> s;
	grid[startX][startY]->changeVisit(true);
	s.push(grid[startX][startY]);
	walk(startX, startY, s);
}

void Maze::walk(int x, int y, stack<Cell*> s) {
	int neighbor[4][2] = {{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}};
	bool check = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (!grid[i][j]->getVisit()){
				check = false;
			}
		}
	}
	if (check) return;
	else {	
		int* index;
		index = getValidCell(x, y);	
		if (index != NULL) {
			
			

			for (int i = 0; i < 4; i++) {
				if (index[0] == neighbor[i][0] && index[1] == neighbor[i][1]) {
					if (i == 0) {
						grid[x][y]->changeRight(false);
						grid[x+1][y]->changeLeft(false);
					} else if (i == 1) {
						grid[x][y]->changeLeft(false);
						grid[x-1][y]->changeRight(false);
					} else if (i == 2) {
						grid[x][y]->changeDown(false);
						grid[x][y+1]->changeUp(false);
					} else if (i == 3) {
						grid[x][y]->changeUp(false);
						grid[x][y-1]->changeDown(false);
					}
					
				}
			}
			s.push(grid[index[0]][index[1]]);
			grid[index[0]][index[1]]->changeVisit(true);
			/*printf("%i", x);
			printf(" ");
			printf("%i\n", y);
			printf("%i", index[0]);
			printf(" ");
			printf("%i\n\n", index[1]);*/
			//toString();
			walk(index[0], index[1],s);
		} else {
			Cell* temp = s.top();
			s.pop();
			//printf("%s\n", temp->getX());
			walk(temp->getX(), temp->getY(), s);
		}
	}
}

int* Maze::getValidCell(int x, int y) {
	int* result = new int[2];
	int neighbor[4][2] = {{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}};
	
	while (!checkNull(neighbor)) {
		int i = rand() % 4;//0-3
		
		if (isValid(neighbor[i][0], neighbor[i][1])) {
			//printf("%i\n", i);
			result[0] = neighbor[i][0];
			result[1] = neighbor[i][1];
			return result;
		} else {
			neighbor[i][0] = -1;
			neighbor[i][1] = -1;
		}
	}
	return NULL;
	
}

bool Maze::checkNull(int in[4][2]) {
	bool result = true;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++){
			if(in[i][j] != -1) {
				result = false;
			}
		}
	}
	return result;
}

bool Maze::isValid(int x, int y) {
	if (inRange(x,y) && !grid[x][y]->getVisit()) {
		return true;
	} else {
		return false;
	}
}

bool Maze::inRange(int x, int y) {
	if (0 <= x && x < size && 0 <= y && y < size){
		return true;
	} else {
		return false;
	}

}
void Maze::toString(){
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (grid[j][i]->getUp()) printf("%s", "---");
			else printf("%s", "  ");

		} 

		printf("\n"); 


	
		for (int j = 0; j < size; j++) {
			if (grid[j][i]->getLeft()) printf("%s", "|");
			else printf("%s", " ");
			
			if (j == startX && i == startY) {
				printf("%s", "s");
			} else if (j == endX && i == endY) {
				printf("%s", "e");
			} else {
				printf("%s", " ");
			}
			if (grid[j][i]->getRight()) printf("%s", "|");
			else printf("%s", " ");

		}
		printf("\n");
		for (int j = 0; j < size; j++) {
			if (grid[j][i]->getDown()) printf("%s", "---");
			else printf("%s", "  ");

		}
		printf("\n");
	} 
}


 
void Maze::initGrid(){

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			grid[i][j] = new Cell(i, j);
		}
	}
}

int Maze::randMaze(){
	
	return rand() % size;//from 0-size-1
}

int main(int argc, char const *argv[])
{
	Maze *m = new Maze(10);
	m->toString();
	return 0;
}

