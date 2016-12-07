#include "logic.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>

using namespace std;

HeightMap::HeightMap(){
	xSize = 250;
	ySize = 250;
	heightMax = 10;
	for(int i=0; i<xSize; i++){
		for(int j=0; j<ySize; j++){
			terrainMap[i][j] = 0.0;
		}
	}
	cout<<"obj made"<<endl;


	srand (static_cast <unsigned> (time(0)));

	numOfCircles = 15;
	constructCircleAlg();

}

HeightMap::~HeightMap(){
	//destroyer

}

void HeightMap::createCrater(int x, int y){
	int circlex, circley, circlesize;



	for (int p = 0; p < numOfCircles; p++ ){	//go through the circles to create the terrain
	circlesize = -(rand() % (((xSize+ySize)/2)/4) + 6); //if the circles are too small its just going to look flat with bumps
	circlex = x;
	circley = y;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}

		}


	}//end of the constuction
}


void HeightMap::createCircle(int x, int y){
	int circlex, circley, circlesize;



		//go through the circles to create the terrain
	circlesize = rand() % (((xSize+ySize)/2)/4) + 25; //if the circles are too small its just going to look flat with bumps
	circlex = x;
	circley = y;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}


	}//end of the constuction
}





void HeightMap::constructCircleAlg(){

	int circlex, circley, circlesize;



	for (int p = 0; p < numOfCircles; p++ ){	//go through the circles to create the terrain
	circlesize = rand() % (((xSize+ySize)/2)/3) + 60; //if the circles are too small its just going to look flat with bumps
	circlex = rand () % xSize;
	circley = rand () % ySize;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}

		}


	}//end of the constuction
	cout<<"end of construction"<<endl;
}

void HeightMap::print(){
		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){

				cout << terrainMap[i][j];
				cout << ", ";
		
		}	
		cout << endl;
	}
}



void HeightMap::reset(){
	for(int i=0; i< xSize; i ++){
		for(int j=0; j < ySize; j ++){
			terrainMap[i][j] = 0.0;
		}
	}

}
/*
void HeightMap::printInstructions(){
	cout<<"Controls:"<<endl;
	cout<<"right/left	Arrows move the Camera about the Y axis"<<endl;
	cout<<"up/down		Arrows move the Camera about the X axis"<<endl;
	cout<<"HOME/END		Arrows move the Camera about the Z axis"<<endl;
	cout<<"F1/F2		Moves the cameras along the x axis (F1 in the negative direction, F2 positive)"<<endl;
	cout<<"F3/F4		Moves the cameras up and down (z axis)"<<endl;
	cout<<"'W' 		Switches between drawn planes (default), wireframe and both wire and filled polygons"<<endl;
	cout<<"'T'		Switches to triangular mesh (default)"<<endl;
	cout<<"'Y'		Switches to quad mesh"<<endl;
	cout<<"'L'		Switches the lights on and off (default both on)"<<endl;
	cout<<"'S' 		Switches between shading modes (default Flat Shading)"<<endl;
	cout<<"'F'		Switches to using the Fault Algorithm to make the terrain"<<endl;

}*/

/*
int main(){
	HeightMap n;

	//n.print();
	return 1;
}*/