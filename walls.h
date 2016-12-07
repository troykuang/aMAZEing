#ifndef WALLS_H
#define WALLS_H


class cell{
	public:
		cell();
		~cell();
		bool north;
		bool south;
		bool west;
		bool east;
	private:

};


class WallBoard{

	public:
		WallBoard();
		~WallBoard();
		cell cells [50][50];
		double max;
		int size;
		void construct();

	private:

};





#endif