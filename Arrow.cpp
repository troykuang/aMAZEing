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
#include "Arrow.h"
Arrow::Arrow() {
    loadArrow();
}

Arrow::~Arrow(){}

//load the ppm file into the program
GLubyte* Arrow::LoadPPM(char* file, int* width, int* height, int* max)
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
    
    /* now get the dimensions and max colour value from the image */
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    /* calculate number of pixels and allocate storage for this */
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;
    
    /* for every pixel, grab the read green and blue values, storing them in the image data array */
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    /* finally, set the "return parameters" (width, height, max) and return the image array */
    *width = n;
    *height = m;
    *max = k;
    
    return img;
}


void Arrow::loadArrow() {
    glEnable(GL_TEXTURE_2D);   
    
    /*Get and save image*/
    image1 = LoadPPM("north.ppm",&width, &height,&max);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image1);
    
    image2 = LoadPPM("south.ppm",&width, &height,&max);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image2);

    image3 = LoadPPM("east.ppm",&width, &height,&max);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image3);
    image4 = LoadPPM("west.ppm",&width, &height,&max);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image4);
}

void Arrow::drawArrow(char dir){//display different texture according to the direction
    
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0, 700, 0, 700); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    glRasterPos2i(700, 700-height); 
    glPixelZoom(-1, 1); 
    switch(dir) {
        case 'N':            
            glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image1);
            break;
        case 'W':
            glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image4);
            break;
        case 'S':
            glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image2);
            break;
        case 'E':
            glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image3);
            break;
    }
    glFlush();//draw the 2d scene
}
