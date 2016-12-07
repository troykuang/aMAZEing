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
#include "logic.h"
#include "walls.h"
#include "Maze.h"
#include "Cell.h"

WallBoard w;
HeightMap map;
int size = 10;
Maze *m = new Maze(size);


//for the camera
float camPos[] = {map.xSize/4, map.ySize/4, 20*map.heightMax};
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


//The material.... want matte so high diffuse, low specular
float m_amb[] = {0.000, 0.392, 0.000, 1}; 
float m_diff[] = {0.133, 0.545, 0.133, 1}; 
float m_spec[] = {0.486, 0.988, 0.000, 1}; 
float shiny = 0.078125; 

//material for ball
float m_ambS[] = {0.458, 0.002, 0.040, 1}; 
float m_diffS[] = {1.000, 0.271, 0.000, 1}; 
float m_specS[] = {0.486, 0.988, 0.600, 1}; 
float shinyS = 27; 

//to use for the normals
float norm[3] = {0,0,1};	//init to the default
float norm2[3] = {0,0,1};	//for doing the norms of quads
float v1 [3] = {0,0,0};
float v2 [3] = {0,0,0};
float v3 [3] = {0,0,0,};

//states for the display
int state[3] ={1,1,1};
int lightState = 1;




void init(void)
{
	glClearColor(0.118, 0.565, 1.000, 1);
	glColor3f(1, 1, 1);


	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, 1, 1, 500);


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
		glutTimerFunc(5,FPS,0); //1 sec
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
	glShadeModel(GL_FLAT);

		for(int i =0; i< map.xSize; i++){
			glBegin(GL_QUAD_STRIP);
			
			for(int j = 0; j< map.ySize; j++ ){

					glVertex3f(i,j,map.terrainMap[i][j]);
					normalFormQuad(i,j);
					glNormal3fv(norm);
					glVertex3f(i+1,j,map.terrainMap[i+1][j]);

				
			}
		glEnd();	
		}//end of both for loops
}

void fenceHorizontal(int p, int q){
	int x = p*4;
	int y = q*4;
	float verts [8][3] = {{x-2,y+2,map.terrainMap[x-2+25][y+2+25]},
							{x-2,y+2,map.terrainMap[x-2+25][y+2+25]+10},
							{x+2,y+2,map.terrainMap[x+2+25][y+2+25]+10},			
							{x+2,y+2,map.terrainMap[x+2+25][y+2+25]},
							{x-2,y+3,map.terrainMap[x-2+25][y+3+25]},
							{x-2,y+3,map.terrainMap[x-2+25][y+3+25]+10},
							{x+2,y+3,map.terrainMap[x+2+25][y+3+25]+10},
							{x+2,y+3,map.terrainMap[x+2+25][y+3+25]} };


	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[3][0],verts[3][1],verts[3][2],verts[2][0],verts[2][1],verts[2][2],verts[1][0],verts[1][1],verts[1][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[3]);
		glVertex3fv(verts[2]);
		glVertex3fv(verts[1]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[3][0],verts[3][1],verts[3][2],verts[7][0],verts[7][1],verts[7][2],verts[6][0],verts[6][1],verts[6][2],verts[2][0],verts[2][1],verts[2][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[3]);
		glVertex3fv(verts[7]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[2]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[5][0],verts[5][1],verts[5][2],verts[6][0],verts[6][1],verts[6][2],verts[7][0],verts[7][1],verts[7][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[5]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[7]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[0][0],verts[0][1],verts[0][2],verts[1][0],verts[1][1],verts[1][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[1]);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[1][0],verts[1][1],verts[1][2],verts[2][0],verts[2][1],verts[2][2],verts[6][0],verts[6][1],verts[6][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[1]);
		glVertex3fv(verts[2]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[4][0],verts[4][1],verts[4][2],verts[7][0],verts[7][1],verts[7][2],verts[3][0],verts[3][1],verts[3][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[7]);
		glVertex3fv(verts[3]);
	glEnd();

}

void fenceVertical(int p, int q){
	int x = p*4;
	int y = q*4;
	float verts [8][3] = {{x+2,y+2,map.terrainMap[x+2+25][y+2+25]},
							{x+2,y+2,map.terrainMap[x+2+25][y+2+25]+10},
							{x+2,y-2,map.terrainMap[x+2+25][y-2+25]+10},			
							{x+2,y-2,map.terrainMap[x+2+25][y-2+25]},
							{x+3,y+2,map.terrainMap[x+3+25][y+2+25]},
							{x+3,y+2,map.terrainMap[x+3+25][y+2+25]+10},
							{x+3,y-2,map.terrainMap[x+3+25][y-2+25]+10},
							{x+3,y-2,map.terrainMap[x+3+25][y-2+25]} };

	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[3][0],verts[3][1],verts[3][2],verts[2][0],verts[2][1],verts[2][2],verts[1][0],verts[1][1],verts[1][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[3]);
		glVertex3fv(verts[2]);
		glVertex3fv(verts[1]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[3][0],verts[3][1],verts[3][2],verts[7][0],verts[7][1],verts[7][2],verts[6][0],verts[6][1],verts[6][2],verts[2][0],verts[2][1],verts[2][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[3]);
		glVertex3fv(verts[7]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[2]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[5][0],verts[5][1],verts[5][2],verts[6][0],verts[6][1],verts[6][2],verts[7][0],verts[7][1],verts[7][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[5]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[7]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[4][0],verts[4][1],verts[4][2],verts[0][0],verts[0][1],verts[0][2],verts[1][0],verts[1][1],verts[1][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[1]);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[1][0],verts[1][1],verts[1][2],verts[2][0],verts[2][1],verts[2][2],verts[6][0],verts[6][1],verts[6][2],verts[5][0],verts[5][1],verts[5][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[1]);
		glVertex3fv(verts[2]);
		glVertex3fv(verts[6]);
		glVertex3fv(verts[5]);
	glEnd();
	glBegin(GL_POLYGON);
		getNormal(verts[0][0],verts[0][1],verts[0][2],verts[4][0],verts[4][1],verts[4][2],verts[7][0],verts[7][1],verts[7][2],verts[3][0],verts[3][1],verts[3][2]);
		glNormal3fv(norm);
		glVertex3fv(verts[0]);
		glVertex3fv(verts[4]);
		glVertex3fv(verts[7]);
		glVertex3fv(verts[3]);
	glEnd();

}

void drawWalls(){
	glPushMatrix();
	glTranslatef(25,25,0);

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


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0); 
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1); 


	glPushMatrix();
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	



	gluLookAt(camPos[0], camPos[1], camPos[2], map.xSize/4,map.ySize/4,2*map.heightMax, 0,1,0);
    
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL

							//applying the material 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 

	drawTerrainQuad();
	drawWalls();
	
			
	glutSwapBuffers();
	
	glPopMatrix();
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
	}
	map.xSize = (size*4)+50;
	map.ySize = (size*4)+50;
	map.constructCircleAlg();
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
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 'q':
		case 27:
			exit (0);
			break;

		case'r':
			map.reset();
			//map.constructCircleAlg();
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
}


void special(int key, int x, int y)
{
	/* arrow key presses move the camera */

	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0] -= 0.5;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0] += 0.5;
			break;

		case GLUT_KEY_UP:
			camPos[2] += 0.5;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] -= 0.5;
			break;
		
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

	}
	glutPostRedisplay();
}






int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	map.constructCircleAlg();
	map.xSize = (size*4)+50;
	map.ySize = (size*4)+50;






	
	
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("3GC3_Project");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	createOurMenu();
	//glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutTimerFunc(0 , FPS, 0);

	glEnable(GL_DEPTH_TEST);
	init();


	//second window for the overview


	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}