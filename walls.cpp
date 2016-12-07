#include "walls.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>

using namespace std;

cell::cell(){
	
	south = false;
	north = false;
	east = false;
	west = false;

	
}


cell::~cell(){

}


WallBoard::WallBoard(){
	max = 7;

	cout<<"constructed";
	size = 50;
	srand (static_cast <unsigned> (time(0)));
	construct();

}

WallBoard::~WallBoard(){
	//destroyed

}

void WallBoard::construct(){
	int x;
	for(int i =0; i< size; i++){
		for(int j =0; j<size; j++){
			x = rand () % 4;



		switch(x){
		case 0:
			cells[i][j].east = true;
			break;
		case 1:
			cells[i][j].north = true;
			break;
		case 2:
			cells[i][j].west = true;
			break;
		case 3:
			cells[i][j].south = true;
			break;
			} //end switch

		}
	

	}//end ifs
}

/*int main(){
	WallBoard w;
	cout<<endl;
	cout<< w.cells[0][1].north <<endl;
	cout<< w.cells[0][1].south <<endl;
}

*/