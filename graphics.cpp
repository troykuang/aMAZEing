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
#include <cmath>
#include "logic.h"
#include "Maze.h"
#include "Cell.h"
#include "Arrow.h"
HeightMap map;
int size = 10;
Maze *m = new Maze(size);
Arrow *a = new Arrow();
float rotX;
int w2;
int wallH = 2;
int colourFlag = 0;//0 = M1, 1 = M2, 2 = M3

//for the camera
float camPos[3] = {25,18,8};
float angleX = 0.0f;
float angleY = 0.0f;

//to do the lighting for Lighting 1
float light_pos0[] =  {map.xSize + 1700,  map.ySize/2 ,  2900,  1.0};
float amb0[4]  = {0.2, 0.2, 0.2, 1}; 
float diff0[4] = {0.8, 0.8, 0.8, 1}; 
float spec0[4] = {0.5, 0.5, 0.5, 1}; 

//to do lighting for the second light
float light_pos1[] =  {-1700.0,  map.ySize/2 ,  2900 ,  1.0};
float amb1[4]  = {0.2, 0.2, 0.2, 1}; 
float diff1[4] = {0.8, 0.8, 0.8, 1}; 
float spec1[4] = {0.5, 0.5, 0.5, 1}; 



//atom material center sphere
float atom_amb[] = {0,0,0,1};
float atom_diff[] = {5,0,0,1};
float atom_spec[] = {0.7,0.6,0.6,1};
float atom_shiny = 0.25;

//atom branch material
float atom_br_amb[] = {0.02, 0.02, 0.02, 1};
float atom_br_diff[] = { 0.01, 0.01, 0.01, 1};
float atom_br_spec[] = {0.4,0.4,0.4,1};
float atom_br_shiny = 0.078125;


//material for ball: M1
float m_ambS[] = {0.458, 0.002, 0.040, 1}; 
float m_diffS[] = {1.000, 0.271, 0.000, 1}; 
float m_specS[] = {0.486, 0.988, 0.600, 1}; 
float shinyS = 27; 

//M2
float ambS1[] ={ 0.105882f, 0.058824f, 0.113725f, 1.0f };
float diffS1[] ={0.427451f, 0.470588f, 0.541176f, 1.0f };
float specS1[] ={0.333333f, 0.333333f, 0.521569f, 1.0f };
float shinyS1 = 9.84615f;

//M3
//Emerald
float ambS2[] ={ 0.0215f, 0.1745f, 0.0215f, 0.55f };
float diffS2[] ={0.07568f, 0.61424f, 0.07568f, 0.55f };
float specS2[] ={0.633f, 0.727811f, 0.633f, 0.55f };
float shinyS2 = 76.8f;
//to use for the normals
float norm[3] = {0,0,1};	//init to the default
float norm2[3] = {0,0,1};	//for doing the norms of quads
float v1 [3] = {0,0,0};
float v2 [3] = {0,0,0};
float v3 [3] = {0,0,0,};

//states for the display
int state[3] ={1,1,1};
int lightState = 1;


float ballPos[3]={0,0,0};
float ballRotate[] = {1,0,0};
float degree = 30;
int balli,ballj;
char dir = 'N'; //inital direction: North

GLubyte* snow;
GLubyte* ice;
GLubyte* grass;
GLubyte* brain;
GLubyte* dirt;
GLubyte* sand;
GLubyte* wood;
GLubyte* candy;
GLubyte* cells;
GLubyte* drake;
int width, height, Tmax;
GLuint myTex[10];
bool cheat;


GLubyte* LoadPPM(char* file, int* width, int* height, int* Tmax)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    /* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    
    /* next, skip past the comments - any line starting with #*/
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    
    /* now get the dimensions and max colour value from the snow */
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    /* calculate number of pixels and allocate storage for this */
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;
    
    /* for every pixel, grab the read green and blue values, storing them in the snow data array */
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    /* finally, set the "return parameters" (width, height, max) and return the snow array */
    *width = n;
    *height = m;
    *Tmax = k;
    
    return img;
}

void initTexture(void){
	glGenTextures(9, myTex);
    
	//textures
    glBindTexture(GL_TEXTURE_2D, myTex[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    

    snow = LoadPPM("snow.ppm",&width, &height, &Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, snow);
    
    
    //ice texture
    glBindTexture(GL_TEXTURE_2D, myTex[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    ice = LoadPPM("ice.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, ice);

    //grass texture
    glBindTexture(GL_TEXTURE_2D, myTex[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    grass = LoadPPM("grass.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, grass);

    //stone texture
    glBindTexture(GL_TEXTURE_2D, myTex[3]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    brain = LoadPPM("brain.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, brain);

    //dirt texture
    glBindTexture(GL_TEXTURE_2D, myTex[4]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    dirt = LoadPPM("dirt.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, dirt);

    //sand
    glBindTexture(GL_TEXTURE_2D, myTex[5]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    sand = LoadPPM("sand.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, sand);

    //wood
    glBindTexture(GL_TEXTURE_2D, myTex[6]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    wood = LoadPPM("wood.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, wood);

    //candy
    glBindTexture(GL_TEXTURE_2D, myTex[7]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    candy = LoadPPM("candy.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, candy);
    
	//cells
    glBindTexture(GL_TEXTURE_2D, myTex[8]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    cells = LoadPPM("cells.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, cells);

    //drake
    glBindTexture(GL_TEXTURE_2D, myTex[9]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    

    drake = LoadPPM("drake.ppm",&width, &height,&Tmax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, drake);
    


    
    
    
    
    
    glMatrixMode(GL_TEXTURE);
    glScalef(1,-1,-1);



}

void updateBallMapPos(){
	ballPos[0] = 25 + 4*balli +1;
	ballPos[1] = 25 + 4*(ballj+1) +1;
	ballPos[2] = map.terrainMap[balli*4+27][(ballj+1)*4+27] + 1;
}

void init(void)
{
	glClearColor(0.118, 0.565, 1.000, 1);
	glColor3f(1, 1, 1);

	cheat = false;


	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);


	//ENABLING THE LIGHTING YAY
	glEnable(GL_LIGHTING); 
	
	//initially lightstate 1
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 

	//add components
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	//add components for the second light
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1); 
    
}


void FPS(int val){
		glutPostRedisplay();
		glutTimerFunc(10,FPS,0); //1 sec
		rotX += 2;
}

void normalFormTri(){
	//assign to global for current normal form
	//do things
	float U [3]= {v3[0]-v2[0], v3[1]-v2[1], v3[2]-v2[2]}; //v2 - v1
	float V [3]= {v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]}; //v3 - v1

	norm[0]=((U[1]*V[2])-(U[3]*V[1]))/1;
	norm[1]= ((U[2]*V[0])-(U[0]*V[2]))/1;
	norm[2]= ((U[0]*V[1])-(U[1]*V[0]))/1;
}



void normalFormQuad(int i, int j){
	//assign v1, v2, v3 
	v1[0] = i;
	v1[1] = j;
	v1[2] = map.terrainMap[i][j];
	v3 [0] = i;
	v3 [1] = j +1;
	v3 [2] = map.terrainMap[i][j+1];
	v2[0] = i+1;
	v2[1] = j;
	v2[2] = map.terrainMap[i+1][j];
	normalFormTri();
	norm2[0] = norm[0];
	norm2[1] = norm[1];
	norm2[2] = norm[2];

	//then the second tri
	v1[0]= v2[0];
	v1[1]= v2[1];
	v1[2]= v2[2];

	v2[0] = i+1;
	v2[1] = j+1;
	v2[2] = map.terrainMap[i+1][j+1];
	normalFormTri();

	norm[0] = (norm[0]+norm2[0])/2;
	norm[1] = (norm[1]+norm2[1])/2;
	norm[2] = (norm[2]+norm2[2])/2;

}

void getNormal(int v0x,int v0y,int v0z,int v1x,int v1y,int v1z,int v2x,int v2y,int v2z,int v3x,int v3y,int v3z){
	v1[0] = v1x;
	v1[1] = v1y;
	v1[2] = v1z;
	v3 [0] = v0x;
	v3 [1] = v0y;
	v3 [2] = v0z;
	v2[0] = v2x;
	v2[1] = v2y;
	v2[2] = v2z;
	normalFormTri();
	norm2[0] = norm[0];
	norm2[1] = norm[1];
	norm2[2] = norm[2];

	//then the second tri
	v1[0]= v3[0];
	v1[1]= v3[1];
	v1[2]= v3[2];

	v3[0] = v3x;
	v3[1] = v3y;
	v3[2] = v3z;
	normalFormTri();

	norm[0] = (norm[0]+norm2[0])/2;
	norm[1] = (norm[1]+norm2[1])/2;
	norm[2] = (norm[2]+norm2[2])/2;
}


void drawTerrainQuad(){	//for drawing quads
	switch(size){
		case 10:
			glBindTexture(GL_TEXTURE_2D, myTex[0]);
			break;
		case 20:
			glBindTexture(GL_TEXTURE_2D, myTex[4]);
			break;
		case 30:
			glBindTexture(GL_TEXTURE_2D, myTex[5]);
			break;
		case 40:
			glBindTexture(GL_TEXTURE_2D, myTex[0]);
			break;
		case 50:
			glBindTexture(GL_TEXTURE_2D, myTex[8]);
			break;
	}
	//glBindTexture(GL_TEXTURE_2D, myTex[0]);

		for(int i =0; i< map.xSize; i++){
			glBegin(GL_QUAD_STRIP);
			
			for(int j = 0; j< map.ySize; j++ ){
					if(j % 2 == 0){
						glTexCoord2f(0.0, 0.0);
					}
					else{
						glTexCoord2f(1.0, 1.0);
					}
					glVertex3f(i,j,map.terrainMap[i][j]);
					normalFormQuad(i,j);
					glNormal3fv(norm);
					if(j % 2 == 0){
						glTexCoord2f(0.0, 1.0);
					}
					else{
						glTexCoord2f(1.0, 0.0);
					}
					glVertex3f(i+1,j,map.terrainMap[i+1][j]);

				
			}
		glEnd();	
		}//end of both for loops
}

void fenceHorizontal(int p, int q){
		switch(size){
		case 10:
			glBindTexture(GL_TEXTURE_2D, myTex[1]);
			break;
		case 20:
			glBindTexture(GL_TEXTURE_2D, myTex[2]);
			break;
		case 30:
			glBindTexture(GL_TEXTURE_2D, myTex[6]);
			break;
		case 40:
			glBindTexture(GL_TEXTURE_2D, myTex[7]);
			break;
		case 50:
			glBindTexture(GL_TEXTURE_2D, myTex[3]);
			break;
	}
	if(cheat){
		glBindTexture(GL_TEXTURE_2D, myTex[9]);
	}
	int x = p*4;
	int y = q*4;
	float verts [8][3] = {{x-2,y+2,map.terrainMap[x-2+25][y+2+25]},
							{x-2,y+2,map.terrainMap[x-2+25][y+2+25]+wallH},
							{x+2,y+2,map.terrainMap[x+2+25][y+2+25]+wallH},			
							{x+2,y+2,map.terrainMap[x+2+25][y+2+25]},
							{x-2,y+3,map.terrainMap[x-2+25][y+3+25]},
							{x-2,y+3,map.terrainMap[x-2+25][y+3+25]+wallH},
							{x+2,y+3,map.terrainMap[x+2+25][y+3+25]+wallH},
							{x+2,y+3,map.terrainMap[x+2+25][y+3+25]} };


	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[3][0],verts[3][1],verts[3][2],verts[2][0],verts[2][1],verts[2][2],verts[1][0],verts[1][1],verts[1][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[3]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[2]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[1]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[3][0],verts[3][1],verts[3][2],verts[7][0],verts[7][1],verts[7][2],verts[6][0],verts[6][1],verts[6][2],verts[2][0],verts[2][1],verts[2][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[3]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[7]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[2]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[5][0],verts[5][1],verts[5][2],verts[6][0],verts[6][1],verts[6][2],verts[7][0],verts[7][1],verts[7][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[5]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[7]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[0][0],verts[0][1],verts[0][2],verts[1][0],verts[1][1],verts[1][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[1][0],verts[1][1],verts[1][2],verts[2][0],verts[2][1],verts[2][2],verts[6][0],verts[6][1],verts[6][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[1]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[4][0],verts[4][1],verts[4][2],verts[7][0],verts[7][1],verts[7][2],verts[3][0],verts[3][1],verts[3][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[7]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[3]);
	glEnd();

}

void fenceVertical(int p, int q){
	switch(size){
		case 10:
			glBindTexture(GL_TEXTURE_2D, myTex[1]);
			break;
		case 20:
			glBindTexture(GL_TEXTURE_2D, myTex[2]);
			break;
		case 30:
			glBindTexture(GL_TEXTURE_2D, myTex[6]);
			break;
		case 40:
			glBindTexture(GL_TEXTURE_2D, myTex[7]);
			break;
		case 50:
			glBindTexture(GL_TEXTURE_2D, myTex[3]);
			break;
	}
	if(cheat){
		glBindTexture(GL_TEXTURE_2D, myTex[9]);
	}
	int x = p*4;
	int y = q*4;
	float verts [8][3] = {{x+2,y+3,map.terrainMap[x+2+25][y+3+25]},
							{x+2,y+3,map.terrainMap[x+2+25][y+3+25]+wallH},
							{x+2,y-2,map.terrainMap[x+2+25][y-2+25]+wallH},			
							{x+2,y-2,map.terrainMap[x+2+25][y-2+25]},
							{x+3,y+3,map.terrainMap[x+3+25][y+3+25]},
							{x+3,y+3,map.terrainMap[x+3+25][y+3+25]+wallH},
							{x+3,y-2,map.terrainMap[x+3+25][y-2+25]+wallH},
							{x+3,y-2,map.terrainMap[x+3+25][y-2+25]} };

	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[3][0],verts[3][1],verts[3][2],verts[2][0],verts[2][1],verts[2][2],verts[1][0],verts[1][1],verts[1][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[3]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[2]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[1]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[3][0],verts[3][1],verts[3][2],verts[7][0],verts[7][1],verts[7][2],verts[6][0],verts[6][1],verts[6][2],verts[2][0],verts[2][1],verts[2][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[3]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[7]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[2]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[5][0],verts[5][1],verts[5][2],verts[6][0],verts[6][1],verts[6][2],verts[7][0],verts[7][1],verts[7][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[5]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[7]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[0][0],verts[0][1],verts[0][2],verts[1][0],verts[1][1],verts[1][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[1][0],verts[1][1],verts[1][2],verts[2][0],verts[2][1],verts[2][2],verts[6][0],verts[6][1],verts[6][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[1]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[6]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[4][0],verts[4][1],verts[4][2],verts[7][0],verts[7][1],verts[7][2],verts[3][0],verts[3][1],verts[3][2]);
		glNormal3fv(norm);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(verts[0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(verts[4]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(verts[7]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(verts[3]);
	glEnd();

}

void drawAtom(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1,0,0);

	
	glPushMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, atom_amb ); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, atom_diff); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, atom_spec ); 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  atom_shiny); 

	
	glTranslatef(m->endX*4 +1, (m->endY+1)*4 +1, map.terrainMap[m->endX*4+26][m->endY*4+26]+2); 
	glRotatef(rotX,0,0,1);
	glScalef(1+(cos(rotX)/15),1+(cos(rotX)/15),1+(cos(rotX)/15));



	glPushMatrix();
		glTranslatef(cos(rotX)/11,sin(rotX)/11,sin(rotX)/11);
		glutSolidSphere(0.5,50,50);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, atom_br_amb ); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, atom_br_diff); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, atom_br_spec ); 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  atom_br_shiny); 
	
	glRotatef(45,0,1,0);

	glPushMatrix();
		glRotatef(2*cos(rotX),0,1,0);
		glutSolidTorus(0.10,1,100,100);
	glPopMatrix();

	
	glRotatef(45,0,1,0);

	glPushMatrix();
		glutSolidTorus(0.10,1,100,100);
	glPopMatrix();

	glRotatef(45,0,1,0);
	
	glPushMatrix();
		glRotatef(2*cos(rotX),0,1,0);
		glutSolidTorus(0.10,1,100,100);
	glPopMatrix();

	
	glPopMatrix();

}

void drawWalls(){

	glPushMatrix();

	for (int i = 0; i < m->getSize(); i++) {
		for (int j = 0; j < m->getSize(); j++) {
			if (m->getUp(i,j)) {
				fenceHorizontal(i,j);
			}
		} 
	
		for (int j = 0; j < m->getSize(); j++) {
			if (m->getLeft(i,j)){ 
				fenceVertical(i-1,j+1);
			}
					
			if (m->getRight(i,j)){ 
				fenceVertical(i,j+1);
			}
		}
		
		for (int j = 0; j < m->getSize(); j++) {
			if (m->getDown(i,j)){
				fenceHorizontal(i,j+1);

			}
		}
		
	} 

	glPopMatrix();
}
void drawSphere(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,0.3);
	switch(colourFlag) {
		case 0:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambS); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffS); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specS); 
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinyS);
			break;
		case 1:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ambS1); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffS1); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specS1); 
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinyS1);
			break;
		case 2:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambS2); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffS2); 
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specS2); 
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinyS2);
			break;
	}
	
	glPushMatrix();
	
	glTranslatef(ballPos[0],ballPos[1],ballPos[2]);
	glRotatef(degree,ballRotate[0], ballRotate[1], ballRotate[2]);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
	glDisable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);

}

void updateCamPos(){
	switch (dir){
		case 'N':
			camPos[0]= ballPos[0];
			camPos[1]= ballPos[1]-8;
			camPos[2]= ballPos[2]+5;
			break;
		case 'S':
			camPos[0]= ballPos[0];
			camPos[1]= ballPos[1]+8;
			camPos[2]= ballPos[2]+5;
			break;
		case 'W':
			camPos[0]= ballPos[0]+8;
			camPos[1]= ballPos[1];
			camPos[2]= ballPos[2]+5;
			break;
		case 'E':
			camPos[0]= ballPos[0]-8;
			camPos[1]= ballPos[1];
			camPos[2]= ballPos[2]+5;
			break;			

	}
}

bool checkWin(){
	bool result = ( (balli == m->endX) && (ballj == m->endY));
	if (result) {
		printf("%s\n", "You reach the end point! WIN!");
	}
	return result;
}

void display(void)
{
	glClearDepth(1);
    glClearColor (0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, 1, 1, 500);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 

	updateBallMapPos();
	updateCamPos();

	if (!checkWin()){
		gluLookAt(camPos[0], camPos[1], camPos[2], ballPos[0],ballPos[1],ballPos[2]+1.5, 0,0,1);
	}
	else{
		gluLookAt(map.xSize/4, map.ySize/4, 20*map.heightMax, ballPos[0],ballPos[1],0, 0,1,0);
	}



	
	glDisable(GL_COLOR_MATERIAL);
	drawSphere();
	glEnable(GL_COLOR_MATERIAL);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0); 
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1); 

	glPushMatrix();
		glRotatef(angleX, 1, 0, 0);
		glRotatef(angleY, 0, 1, 0);
	    
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL

		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		drawTerrainQuad();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);


		glPushMatrix();
			glTranslatef(25,25,0);
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_TEXTURE_2D);
			
			drawWalls();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR_MATERIAL);
			drawAtom();
		glPopMatrix();
		
	glPopMatrix();


	glFlush();
	a->drawArrow(dir);	
	glutSwapBuffers();
	
	
}

void display_2(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(0,300,0,300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(10,10,0);
	glColor3f(1,0,0);
	int side = 280/size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (m->getUp(i,j)) {
				glBegin(GL_LINES);
					glVertex2f(i*side,j*side);
					glVertex2f(i*(side) + side,j*side);
				glEnd();
			}
		} 
	
		for (int j = 0; j < size; j++) {
			if (m->getLeft(i,j)){ 
				glBegin(GL_LINES);
					glVertex2f(i*side,j*side);
					glVertex2f(i*side,j*(side) + side);
				glEnd();
			}
			
			if (m->getRight(i,j)){ 
				glBegin(GL_LINES);
					glVertex2f(i*(side) + side,j*side);
					glVertex2f(i*(side) + side,j*(side) + side);
				glEnd();
			}
			glPointSize(5);
			if (i == m->startX && j == m->startY) {
				glColor3f(1,0,0);
				glBegin(GL_POINTS);
					glVertex2f(i*side + side/2 - 0.2,j*side + side/2 - 0.2);
					
				glEnd();
			}
			
			if (i == m->endX && j == m->endY) {
				glColor3f(0,1,0);
				glBegin(GL_POINTS);
					glVertex2f(i*side + side/2 - 0.2,j*side + side/2 - 0.2);
					
				glEnd();
				glColor3f(1,0,0);
			}
			if (i == balli && j== ballj){
				glColor3f(0,0,1);
				glBegin(GL_POINTS);
					glVertex2f(i*side + side/2 - 0.2,j*side + side/2 - 0.2);
					
				glEnd();
				glColor3f(1,0,0);
			}
		}
		
		for (int j = 0; j < size; j++) {
			if (m->getDown(i,j)){
				glBegin(GL_LINES);
					glVertex2f(i*side,j*(side) + side);
					glVertex2f(i*(side) + side,j*(side)+side);
				glEnd();
			}
		}
		
	} 
	glFlush();
}

//add create menu to main()
void menuProc(int value){
	switch(value) {
		case 1:
			printf("%s\n", "Restart the game with current size");
			m = new Maze(size);
			break;
		case 2:
			printf("%s\n", "Exit!");
			exit(0);
			break;
		case 3:
			printf("%s\n", "Change size to 10");
			size = 10;
			m = new Maze(size);
			break;
		case 4:
			printf("%s\n", "Change size to 20");
			size = 20;
			m = new Maze(size);
			break;
		case 5:
			printf("%s\n", "Change size to 30");
			size = 30;
			m = new Maze(size);
			break;
		case 6:
			printf("%s\n", "Change size to 40");
			size = 40;
			m = new Maze(size);
			break;
		case 7:
			printf("%s\n", "Change size to 50");
			size = 50;
			m = new Maze(size);
			break;
		case 8:
			printf("%s\n", "Switch to M1 ball");
			colourFlag = 0;
			break;
		case 9:
			printf("%s\n", "Switch to M2 ball");
			colourFlag = 1;
			break;
		case 10:
			printf("%s\n", "Switch to M3 ball");
			colourFlag = 2;
			break;
	}
	map.xSize = (size*4)+50;
	map.ySize = (size*4)+50;
	map.reset();
	map.constructCircleAlg();
	glutPostRedisplay();
	glutSetWindow(w2);
	glutPostRedisplay();
}

void createOurMenu(){
	int subMenu_id1 = glutCreateMenu(menuProc);
	glutAddMenuEntry("10", 3);
	glutAddMenuEntry("20", 4);
	glutAddMenuEntry("30", 5);
	glutAddMenuEntry("40", 6);
	glutAddMenuEntry("50", 7);

	
	int main_id = glutCreateMenu(menuProc);
	glutAddSubMenu("Size", subMenu_id1);
	glutAddMenuEntry("Restart with current size", 1);
	glutAddMenuEntry("Quit", 2);
	glutAddMenuEntry("Skin 1", 8);
	glutAddMenuEntry("Skin 2", 9);
	glutAddMenuEntry("Skin 3", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 27:
			exit (0);
			break;
		case 'r':
		case 'R':
			map.reset();
			map.constructCircleAlg();
			glutPostRedisplay();
			break;
		case 'f':
		case 'F':
			map.reset();
			glutPostRedisplay();
			break;
		case 'D':
		case 'd':
			cheat = !cheat;
			glutPostRedisplay();
			break;
		case 'l':
		case 'L':
			switch(lightState){
				case 1:	//light are all on
					glDisable(GL_LIGHT0);
					lightState = 2;
					glutPostRedisplay();
					break;
				case 2:
					glDisable(GL_LIGHT1);
					lightState = 3;
					glutPostRedisplay();
					break;
				case 3:	//they are both disables
					glEnable(GL_LIGHT0);
					lightState = 4;
					glutPostRedisplay();
					break;
				case 4:
					glEnable(GL_LIGHT1);
					lightState = 1;
					glutPostRedisplay();
					break;


		}


	}
	glutSetWindow(w2);
	glutPostRedisplay();
}


void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	int mod = glutGetModifiers();

	switch(key)
	{
		case GLUT_KEY_LEFT:
			if (mod == GLUT_ACTIVE_SHIFT){
				camPos[0] -= 2;
				break;
			}
			else{
				switch(dir)
				{
					case 'N':
						dir = 'W';
						break;
					case 'S':
						dir = 'E';
						break;
					case 'E':
						dir = 'N';
						break;
					case 'W':
						dir = 'S';
						break;
				}	
				break;
			}
			

		case GLUT_KEY_RIGHT:
			if (mod == GLUT_ACTIVE_SHIFT){
				camPos[0] += 2;
				break;
			}
			else{
				switch(dir)
				{
					case 'N':
						dir = 'E';
						break;
					case 'S':
						dir = 'W';
						break;
					case 'E':
						dir = 'S';
						break;
					case 'W':
						dir = 'N';
						break;
				}
				break;
			}

		case GLUT_KEY_UP:
			if (mod == GLUT_ACTIVE_SHIFT){
				camPos[1] += 2;
				break;
			}
			else {
				if (mod == GLUT_ACTIVE_ALT){
					ballPos[2] += 1;
					break;

				}
				else{
					switch(dir)
				{
					case 'S':
						degree+=60;
						if (!m->getUp(balli,ballj)){
							ballj -= 1;
							break;
						}
						break;
						
					case 'N':
						degree+=60;
						if (!m->getDown(balli,ballj)){
							ballj += 1;
							break;
						}
						break;
						
					case 'E':
						degree+=60;
						if(!m->getRight(balli,ballj)){
							balli += 1;
							break;
						}
						break;
					case 'W':
						degree+=60;
						if (!m->getLeft(balli,ballj)){
							balli -= 1;
							break;
						}
						break;
						
				}
				break;		
				}
				break;
			}


		case GLUT_KEY_DOWN:
			if (mod == GLUT_ACTIVE_SHIFT){
				camPos[1] -= 2;
				break;
			}
			else {
				if (mod == GLUT_ACTIVE_ALT){
					ballPos[2] -= 1;
					break;

				}
				else{
					switch(dir)
				{
					case 'N':
						dir = 'S';
						break;
					case 'S':
						dir = 'N';
						break;
					case 'E':
						dir = 'W';
						break;
					case 'W':
						dir = 'E';
						break;
				}
				break;
				}
			}
		case GLUT_KEY_HOME:
			camPos[1]-=0.5;
			break;

		case GLUT_KEY_END:
			camPos[1]+=0.5;
			
			break;
		case GLUT_KEY_F1:
			light_pos0[1] -= 5;
			light_pos1[1] -= 5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F2:
			light_pos0[1] += 5;
			light_pos1[1] += 5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F3:
			light_pos0[2] += 5;
			light_pos1[2] += 5;
			glutPostRedisplay();
		break;
		case GLUT_KEY_F4:
			light_pos0[2] -= 5;
			light_pos1[2] -= 5;
			glutPostRedisplay();
		break;
		case GLUT_KEY_F5:
			camPos[2]-=2;
			break;

		case GLUT_KEY_F6:
			camPos[2]+=2;
			
			break;

	}
	glutPostRedisplay();
	glutSetWindow(w2);
	glutPostRedisplay();
}







int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	map.constructCircleAlg();
	map.xSize = (size*4)+50;
	map.ySize = (size*4)+50;
	rotX = 0;
	
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);

	balli =m->startX;
	ballj =m->startY;
	updateBallMapPos();

	glutCreateWindow("3GC3_Project");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	createOurMenu();
	//glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutTimerFunc(0 , FPS, 0);

	initTexture();
	init();


	//second window for the overview
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(900, 200);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	w2 = glutCreateWindow(argv[0]);
	glutSetWindowTitle("Labyrinth Map");
	
	
	glutDisplayFunc(display_2);
	


	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
