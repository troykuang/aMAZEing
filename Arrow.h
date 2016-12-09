#ifndef Arrow_H
#define Arrow_H

#include <stdio.h>
class Arrow
{
public:
	Arrow();
	~Arrow();
	void drawArrow(char dir);

private: 
	GLubyte* image1;
	GLubyte* image2;
	GLubyte* image3;
	GLubyte* image4;
	int width, height, max;
	GLubyte* LoadPPM(char* file, int* width, int* height, int* max1);
	void loadArrow();
};
#endif