    //
//  main.cpp
//  CGProject
//
//  Created by Srikant Aggarwal on 27/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#include "main.h"

int width;
int height;
int positionX;
int positionY;

float zNear = 0.01f;
float zFar = 110.1f;

float cameraX = 0.0f;
float cameraY = 14.0f;
float cameraZ = 21.0f;

vector<DeformableObject> objects;
int objectJustCreated = 0;

GLuint textures[2];

clock_t prevTime;

Bitmap sandy, sky;

/***************************************************
 * Method loads the textures for the background
 **************************************************/
void loadTextures()
{
    glGenTextures(2, textures);
    
    sandy = Bitmap();
    sandy.loadBMP("CGProject/textures/sandy.bmp");
    glBindTexture( GL_TEXTURE_2D, textures[0]);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, sandy.width, sandy.height, 0, GL_RGB, GL_UNSIGNED_BYTE, sandy.data);
    
    
    sky = Bitmap();
    sky.loadBMP("CGProject/textures/sky.bmp");
    glBindTexture( GL_TEXTURE_2D, textures[1]);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, 3, sky.width, sky.height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky.data);
}

/***************************************************
 * Method to initialize the scene
 **************************************************/
void initialize()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    glViewport(0, 0, width, height);
    
    loadTextures();
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(90, ((float) width / ((float)height)), zNear, zFar);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY, 0.0, 0, 1, 0);
    
    float ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float position[] = {0.0f, cameraY, cameraZ, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    groundLevel = 0.0f;
}

/***************************************************
 * Callback called when the system is idle
 **************************************************/
void idleFunc()
{
    glutPostRedisplay();
}

/***************************************************
 * Method called for rendering the cube
 **************************************************/
void displayFunc( )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_TEXTURE_2D);
    
    //Loading the sand texture
    glBindTexture( GL_TEXTURE_2D, textures[0]);
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(1, 1);
    glVertex3f(zFar*1.4+2, groundLevel, cameraZ-zFar+0.1f);
    
    glTexCoord2f(1, 0);
    glVertex3f(zFar*1.4+2, groundLevel, cameraZ+10.0f);
    
    glTexCoord2f(0, 0);
    glVertex3f(-(zFar*1.4+2), groundLevel, cameraZ+10.0f);
    
    glTexCoord2f(0, 1);
    glVertex3f(-(zFar*1.4+2), groundLevel, cameraZ-zFar+0.1f);
    
    glEnd();
    
    //Load the cloud texture
    glBindTexture( GL_TEXTURE_2D, textures[1]);
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0, 0);
    glVertex3f(-(zFar*1.4+2), groundLevel, cameraZ-zFar+0.1f);
    
    glTexCoord2f(1, 0);
    glVertex3f(zFar*1.4+2, groundLevel, cameraZ-zFar+0.1f);
    
    glTexCoord2f(1, 1);
    glVertex3f(zFar*1.4+2, cameraY+(zFar*1.4+2), cameraZ-zFar+0.1f);
    
    glTexCoord2f(0, 1);
    glVertex3f(-(zFar*1.4+2), cameraY+(zFar*1.4+2), cameraZ-zFar+0.1f);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    for (int i = 0; i < objects.size(); ++i)
    {
        //If the object was just created, initialize its original position
        if (objectJustCreated)
        {
            objectJustCreated = 0;
            prevTime = clock();
        }
        //If the object was created in or before the previous time step
        else
        {
            clock_t currentTime = clock();
            clock_t elapsedTime = (currentTime-prevTime)/100;
            //Do shape matching for calculation of goal position
            objects[i].matchShape(elapsedTime);
            //Update the position based on goal position
            objects[i].update(elapsedTime);
            prevTime = currentTime;
        }
        //Draw the object
        objects[i].draw();
    }
    
    glFlush();
	glutSwapBuffers();
}

/***************************************************
 * Callback when a menu item is selected
 **************************************************/
void menuCallback(int option)
{
    DeformableObject cube = DeformableObject();
    switch (option) {
        case RIGID:
            mode = RIGID;
            alpha = 1.0f;
            cube.initialize("CGProject/model/cube.obj", 16.00f);
            break;
        case RIGID_ROTATED:
            mode = RIGID;
            alpha = 1.0f;
            cube.initialize("CGProject/model/cube.obj", 16.0f, true);
            break;
        case LINEAR:
            mode = LINEAR;
            alpha = 0.01f;
            beta = 0.1f;
            cube.initialize("CGProject/model/cube.obj", 16.0f);
            break;
        case LINEAR_ROTATED:
            mode = LINEAR;
            alpha = 0.01f;
            beta = 0.1f;
            cube.initialize("CGProject/model/cube.obj", 16.0f, true);
            break;
        case QUADRATIC:
            mode = QUADRATIC;
            alpha = 1.00f;
            beta = 1.0f;
            cube.initialize("CGProject/model/cube.obj", 16.0f);
            break;
    }
    
    objects.clear();
    objectJustCreated = 1;
    elasticity = 0.7f;
    srand(time(NULL));
    cube_color = rand() % 3;
    objects.push_back(cube);
    glutPostRedisplay();
}

/***************************************************
 * Method creates the menu
 **************************************************/
void makeMenu()
{
	int menu, modes;

	modes = glutCreateMenu(menuCallback);
	glutAddMenuEntry("Rigid - Not rotated", RIGID);
    glutAddMenuEntry("Rigid - Rotated", RIGID_ROTATED);
	glutAddMenuEntry("Linear - not rotated", LINEAR);
    glutAddMenuEntry("Linear - Rotated", LINEAR_ROTATED);
//    glutAddMenuEntry("Quadratic", QUADRATIC);
	menu = glutCreateMenu(menuCallback);
    
	glutAddSubMenu("Deformation modes", modes);
    
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

int main(int argc, const char * argv[])
{
    width = 800;
    height = 600;
    
    //Creating window and setting display mode
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(argv[0]);
    
    initialize();
    makeMenu();
    
    //Setting callbacks
    glutIdleFunc(idleFunc);
    glutDisplayFunc(displayFunc);
    glutMainLoop();
    return 0;
}

