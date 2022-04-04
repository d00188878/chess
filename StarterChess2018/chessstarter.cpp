// Melanie Lent
// Chess animation starter kit.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include "glut.h"
#include "graphics.h"
#include "world.h"
using namespace std;


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = {0.7f, 0.1f, 0.2f, 1.0f};
GLfloat greenMaterial[] = {0.1f, 0.7f, 0.4f, 1.0f};
GLdouble brightGreenMaterial[] = {0.1, 0.9, 0.1, 1.0};
GLfloat blueMaterial[] = {0.1f, 0.2f, 0.7f, 1.0f};
GLfloat whiteMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};

double screen_x = 600;
double screen_y = 500;

World gWorld;


double GetTime()
{
	static clock_t start_time = clock();
	clock_t current_time = clock();
	double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
	return total_time;
}

// Outputs a string of text at the specified location.
void text_output(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}

// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal vector n[0], n[1], n[2].
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];
	
	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1]*v2[2] - v1[2]*v2[1];
	n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n[2] = v1[0]*v2[1] - v1[1]*v2[0];

	double size = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}

// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void DrawPiece(const char filename[])
{
	// Try to open the given file.
	char buffer[200];
	ifstream in(filename);
	if(!in)
	{
		cerr << "Error. Could not open " << filename << endl;
		exit(1);
	}

	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	do
	{
		in.getline(buffer, 200); // get one line (point) from the file.
		int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		done = in.eof();
		if(!done)
		{
			if(count == 3) // if this line had an x,y,z point.
			{
				verts++;
			}
			else // the line was empty. Finish current polygon and start a new one.
			{
				if(verts>=3)
				{
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for(int i=0; i<verts; i++)
					{
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
			}
		}
	}
	while(!done);

	if(verts>0)
	{
		cerr << "Error. Extra vertices in file " << filename << endl;
		exit(1);
	}

}

// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = {4500, 8000, -4000}; // pick a nice vantage point.
double at[3]  = {4500, 0,     4000};
//
// GLUT callback functions
//

// As t goes from t0 to t1, set v between v0 and v1 accordingly.
void Interpolate(double t, double t0, double t1,
	double & v, double v0, double v1)
{
	double ratio = (t - t0) / (t1 - t0);
	if (ratio < 0)
		ratio = 0;
	if (ratio > 1)
		ratio = 1;
	v = v0 + (v1 - v0)*ratio;
}

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	double t = GetTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = {0.8f, 0.9f, 0.5f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);




	double whitePawn1KnockBackX;
	double whitePawn1KnockBackY;
	Interpolate(t, 23.25, 23.55, whitePawn1KnockBackX, 1000, -7000);
	Interpolate(t, 23.25, 23.55, whitePawn1KnockBackY, 2000, -12000);
	glPushMatrix();
	glTranslatef(whitePawn1KnockBackX, 0, whitePawn1KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn2KnockBackX;
	double whitePawn2KnockBackY;
	Interpolate(t, 22.6, 22.85, whitePawn2KnockBackX, 2000, -8000);
	Interpolate(t, 22.6, 22.85, whitePawn2KnockBackY, 2000, -14000);
	glPushMatrix();
	glTranslatef(whitePawn2KnockBackX, 0, whitePawn2KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn3KnockBackX;
	double whitePawn3KnockBackY;
	Interpolate(t, 21.7, 21.9, whitePawn3KnockBackX, 3000, -7000);
	Interpolate(t, 21.7, 21.9, whitePawn3KnockBackY, 2000, -4000);
	glPushMatrix();
	glTranslatef(whitePawn3KnockBackX, 0, whitePawn3KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn4KnockBackX;
	double whitePawn4KnockBackY;
	Interpolate(t, 21.4, 21.65, whitePawn4KnockBackX, 4000, -12000);
	Interpolate(t, 21.4, 21.65, whitePawn4KnockBackY, 2000, -18000);
	glPushMatrix();
	glTranslatef(whitePawn4KnockBackX, 0, whitePawn4KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn5KnockBackX;
	double whitePawn5KnockBackY;
	Interpolate(t, 20.7, 20.9, whitePawn5KnockBackX, 5000, -12000);
	Interpolate(t, 20.7, 20.9, whitePawn5KnockBackY, 2000, -12000);
	glPushMatrix();
	glTranslatef(whitePawn5KnockBackX, 0, whitePawn5KnockBackY);
	glCallList(pawn);
	glPopMatrix();


	double whitePawn6KnockBackX;
	double whitePawn6KnockBackY;
	Interpolate(t, 5.1, 5.35, whitePawn6KnockBackX, 6000, 28000);
	Interpolate(t, 5.1, 5.35, whitePawn6KnockBackY, 2000, -36000);
	glPushMatrix();
	glTranslatef(whitePawn6KnockBackX, 0, whitePawn6KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn7KnockBackX;
	double whitePawn7KnockBackY;
	Interpolate(t, 18.85, 19.1, whitePawn7KnockBackX, 7000, 6000);
	Interpolate(t, 18.85, 19.1, whitePawn7KnockBackY, 2000, -19000);
	glPushMatrix();
	glTranslatef(whitePawn7KnockBackX, 0, whitePawn7KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double whitePawn8KnockBackX;
	double whitePawn8KnockBackY;
	Interpolate(t, 18.75, 19, whitePawn8KnockBackX, 8000, 28000);
	Interpolate(t, 18.75, 19, whitePawn8KnockBackY, 2000, 2500);
	glPushMatrix();
	glTranslatef(whitePawn8KnockBackX, 0, whitePawn8KnockBackY);
	glCallList(pawn);
	glPopMatrix();




	double rightWhiteKnightKnockBackX;
	double rightWhiteKnightKnockBackY;
	Interpolate(t, 21.75, 22, rightWhiteKnightKnockBackX, 2000, -8000);
	Interpolate(t, 21.75, 22, rightWhiteKnightKnockBackY, 1000, -8000);
	glPushMatrix();
	glTranslatef(rightWhiteKnightKnockBackX, 0, rightWhiteKnightKnockBackY);
	glCallList(knight);
	glPopMatrix();

	double leftWhiteKnightKnockBackX;
	double leftWhiteKnightKnockBackY;
	Interpolate(t, 5.15, 5.35, leftWhiteKnightKnockBackX, 7000, 28000);
	Interpolate(t, 5.15, 5.35, leftWhiteKnightKnockBackY, 1000, -36000);
	glPushMatrix();
	glTranslatef(leftWhiteKnightKnockBackX, 0, leftWhiteKnightKnockBackY);
	glCallList(knight);
	glPopMatrix();

	double rightWhiteBishopKnockBackX;
	double rightWhiteBishopKnockBackY;
	Interpolate(t, 22.4, 22.75, rightWhiteBishopKnockBackX, 3000, 28000);
	Interpolate(t, 22.4, 22.75, rightWhiteBishopKnockBackY, 1000, -12000);
	glPushMatrix();
	glTranslatef(rightWhiteBishopKnockBackX, 0, rightWhiteBishopKnockBackY);
	glCallList(bishop);
	glPopMatrix();

	double rightWhiteRookKnockBackX;
	double rightWhiteRookKnockBackY;
	Interpolate(t, 23.8, 24.05, rightWhiteRookKnockBackX, 1000, -6000);
	Interpolate(t, 23.8 , 24.05, rightWhiteRookKnockBackY, 1000, -36000);
	glPushMatrix();
	glTranslatef(rightWhiteRookKnockBackX, 0, rightWhiteRookKnockBackY);
	glCallList(rook);
	glPopMatrix();

	double leftWhiteRookKnockBackX;
	double leftWhiteRookKnockBackY;
	Interpolate(t, 19.1, 19.3, leftWhiteRookKnockBackX, 8000, 28000);
	Interpolate(t, 19.1, 19.3, leftWhiteRookKnockBackY, 1000, -5000);
	glPushMatrix();
	glTranslatef(leftWhiteRookKnockBackX, 0, leftWhiteRookKnockBackY);
	glCallList(rook);
	glPopMatrix();

	double whiteKingKnockBackX;
	double whiteKingKnockBackY;
	Interpolate(t, 20.75, 21, whiteKingKnockBackX, 4000, -12000);
	Interpolate(t, 20.75, 21, whiteKingKnockBackY, 1000, -12000);
	glPushMatrix();
	glTranslatef(whiteKingKnockBackX, 0, whiteKingKnockBackY);
	glCallList(king);
	glPopMatrix();
	
	double z;
	double whiteQueenKnockBackX;
	double whiteQueenKnockBackY;
	Interpolate(t, 1.0, 3.0, z, 1000, 5000);
	Interpolate(t, 5.175, 5.2, whiteQueenKnockBackX, 0, 600);
	Interpolate(t, 5.175, 5.2, whiteQueenKnockBackY, 0, -50);
	glPushMatrix();
	glTranslatef(5000, 0, z);
	glTranslatef(whiteQueenKnockBackX, 0, whiteQueenKnockBackY);
	glCallList(queen);
	glPopMatrix();

	double leftWhiteBishopKnockBackX;
	double leftWhiteBishopKnockBackY;
	Interpolate(t, 19.4, 19.6, leftWhiteBishopKnockBackX, 6000, 9000);
	Interpolate(t, 19.4, 19.6, leftWhiteBishopKnockBackY, 1000, -8000);
	glPushMatrix();
	glTranslatef(leftWhiteBishopKnockBackX, 0, leftWhiteBishopKnockBackY);
	glCallList(bishop);
	glPopMatrix();



	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = {0.1f, 0.5f, 0.8f, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, greenMaterial);

	double moveRight1;
	double fallOver;
	double startSpinning;
	double moveLeft;
	double moveForward;
	double moveRight2;
	double standUp1;
	double standUp2;
	// move right
	Interpolate(t, 6.0, 8.0, moveRight1, 4000, 2000);
	// fall over
	Interpolate(t, 4.0, 4.5, fallOver, 0, 90);
	// spin for 24 seconds and kill the other pieces
	Interpolate(t, 5.0, 26.0, startSpinning, 0, 24930);//15000.0);
	// move left
	Interpolate(t, 10.0, 15.0, moveLeft, 0, 6000);
	// move towards the front of the board
	Interpolate(t, 17.0, 20.0, moveForward, 0, -6500);
	// move back to the right of the board
	Interpolate(t, 20.0, 25.0, moveRight2, 0, -6500);
	// stand back up
	Interpolate(t, 27.0, 27.5, standUp1, 0, -90);
	Interpolate(t, 27.0, 27.5, standUp2, 0, 90);

	// begin animation!!!!

	glPushMatrix();
	glTranslatef(moveRight1, 0, 8000);
	glRotatef(fallOver, 10, 0, 0);
	glTranslatef(moveLeft, 0, 0);
	glTranslatef(0, moveForward, 0);
	glTranslatef(moveRight2, 0, 0);
	glRotatef(standUp1, 0, 10, 0);
	//glRotatef(standUp2, 10, 0, 0);
	glRotatef(startSpinning, 0, 0, 1);
	glCallList(king);
	glPopMatrix();


	double blackQueenKnockBackX;
	double blackQueenKnockBackY;
	Interpolate(t, 5.1, 5.13, blackQueenKnockBackX, 5000, 5500);
	Interpolate(t, 5.1, 5.13, blackQueenKnockBackY, 8000, 28000);
	glPushMatrix();
	glTranslatef(blackQueenKnockBackX, 0, blackQueenKnockBackY);
	glCallList(queen);
	glPopMatrix();

	double leftBlackBishopKnockBackX;
	double leftBlackBishopKnockBackY;
	Interpolate(t, 5.1, 5.13, leftBlackBishopKnockBackX, 6000, 8000);
	Interpolate(t, 5.1, 5.13, leftBlackBishopKnockBackY, 8000, 28000);
	glPushMatrix();
	glTranslatef(leftBlackBishopKnockBackX, 0, leftBlackBishopKnockBackY);
	glCallList(bishop);
	glPopMatrix();

	double rightBlackBishopKnockBackX;
	double rightBlackBishopKnockBackY;
	double rightBlackBishopKnockBackRotate;
	Interpolate(t, 5.075, 5.5, rightBlackBishopKnockBackX, 3000, -28000);
	Interpolate(t, 5.075, 5.5, rightBlackBishopKnockBackY, 8000, -28000);
	Interpolate(t, 5.075, 5.5, rightBlackBishopKnockBackRotate, 0, -40);
	glPushMatrix();
	glTranslatef(rightBlackBishopKnockBackX, 0, rightBlackBishopKnockBackY);
	glRotatef(rightBlackBishopKnockBackRotate, 10, 10, 0);
	glCallList(bishop);
	glPopMatrix();


	double rightBlackKnightKnockBackX;
	double rightBlackKnightKnockBackY;
	double rightBlackKnightKnockBackRotate;
	Interpolate(t, 5.075, 5.5, rightBlackKnightKnockBackX, 2000, -28000);
	Interpolate(t, 5.075, 5.5, rightBlackKnightKnockBackY, 8000, -28000);
	Interpolate(t, 5.075, 5.5, rightBlackKnightKnockBackRotate, 180, -40);
	glPushMatrix();
	glTranslatef(rightBlackKnightKnockBackX, 0, rightBlackKnightKnockBackY);
	glRotatef(rightBlackKnightKnockBackRotate, 0, 80, 0);
	glCallList(knight);
	glPopMatrix();

	double leftBlackKnightKnockBackX;
	double leftBlackKnightKnockBackY;
	Interpolate(t, 12.5, 12.8, leftBlackKnightKnockBackX, 7000, 32000);
	Interpolate(t, 12.5, 12.8, leftBlackKnightKnockBackY, 8000, 40000);
	glPushMatrix();
	glTranslatef(leftBlackKnightKnockBackX, 0, leftBlackKnightKnockBackY);
	glRotatef(180, 0, 80, 0);
	glCallList(knight);
	glPopMatrix();

	double rightBlackRookKnockBackX;
	double rightBlackRookKnockBackY;
	Interpolate(t, 7.15, 7.25, rightBlackRookKnockBackX, 1000, -10000);
	Interpolate(t, 7.15, 7.25, leftBlackKnightKnockBackY, 8000, -5000);
	glPushMatrix();
	glTranslatef(rightBlackRookKnockBackX, 0, leftBlackKnightKnockBackY);
	glCallList(rook);
	glPopMatrix();

	double leftBlackRookKnockBackX;
	double leftBlackRookKnockBackY;
	Interpolate(t, 13.3, 13.5, leftBlackRookKnockBackX, 8000, 32000);
	Interpolate(t, 13.3, 13.5, leftBlackRookKnockBackY, 8000, 40000);
	glPushMatrix();
	glTranslatef(leftBlackRookKnockBackX, 0, leftBlackRookKnockBackY);
	glCallList(rook);
	glPopMatrix();

	

	double blackPawn8KnockBackX;
	double blackPawn8KnockBackY;
	Interpolate(t, 5.1, 5.5, blackPawn8KnockBackX, 1000, -28000);
	Interpolate(t, 5.1, 5.5, blackPawn8KnockBackY, 7000, -28000);
	glPushMatrix();
	glTranslatef(blackPawn8KnockBackX, 0, blackPawn8KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn7KnockBackX;
	double blackPawn7KnockBackY;
	Interpolate(t, 5.1, 5.5, blackPawn7KnockBackX, 2000, -28000);
	Interpolate(t, 5.1, 5.5, blackPawn7KnockBackY, 7000, -36000);
	glPushMatrix();
	glTranslatef(blackPawn7KnockBackX, 0, blackPawn7KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn6KnockBackX;
	double blackPawn6KnockBackY;
	Interpolate(t, 5.15, 5.35, blackPawn6KnockBackX, 3000, 28000);
	Interpolate(t, 5.15, 5.35, blackPawn6KnockBackY, 7000, -36000);
	glPushMatrix();
	glTranslatef(blackPawn6KnockBackX, 0, blackPawn6KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn5KnockBackX;
	double blackPawn5KnockBackY;
	Interpolate(t, 5.15, 5.35, blackPawn5KnockBackX, 4000, 28000);
	Interpolate(t, 5.15, 5.35, blackPawn5KnockBackY, 7000, -10000);
	glPushMatrix();
	glTranslatef(blackPawn5KnockBackX, 0, blackPawn5KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn4KnockBackX;
	double blackPawn4KnockBackY;
	Interpolate(t, 5.175, 5.35, blackPawn4KnockBackX, 5000, 28000);
	Interpolate(t, 5.175, 5.35, blackPawn4KnockBackY, 7000, -10000);
	glPushMatrix();
	glTranslatef(blackPawn4KnockBackX, 0, blackPawn4KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn3KnockBackX;
	double blackPawn3KnockBackY;
	Interpolate(t, 12, 12.2, blackPawn3KnockBackX, 6000, 5500);
	Interpolate(t, 12, 12.2, blackPawn3KnockBackY, 7000, 28000);
	glPushMatrix();
	glTranslatef(blackPawn3KnockBackX, 0, blackPawn3KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn2KnockBackX;
	double blackPawn2KnockBackY;
	Interpolate(t, 12.7, 12.9, blackPawn2KnockBackX, 7000, 5500);
	Interpolate(t, 12.7, 12.9, blackPawn2KnockBackY, 7000, 28000);
	glPushMatrix();
	glTranslatef(blackPawn2KnockBackX, 0, blackPawn2KnockBackY);
	glCallList(pawn);
	glPopMatrix();

	double blackPawn1KnockBackX;
	double blackPawn1KnockBackY;
	Interpolate(t, 13.2, 13.4, blackPawn1KnockBackX, 8000, 5500);
	Interpolate(t, 13.2, 13.4, blackPawn1KnockBackY, 7000, 28000);
	glPushMatrix();
	glTranslatef(blackPawn1KnockBackX, 0, blackPawn1KnockBackY);
	glCallList(pawn);
	glPopMatrix();


	GLfloat light_position[] = {1,2,-.1f, 0}; // light comes FROM this vector direction.
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueMaterial);
	gWorld.Draw();

	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 45.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ 100, /* Z far */ 30000.0);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	// set material's specular properties
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat light_position[] = {(float)eye[0], (float)eye[1], (float)eye[2],1};
	GLfloat white_light[] = {1,1,1,1};
	GLfloat low_light[] = {.3f,.3f,.3f,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHTING);	// enable general lighting
	glEnable(GL_LIGHT0);	// enable the first light.

//Display lists

	glNewList(pawn, GL_COMPILE);
	DrawPiece("PAWN.POL");
	glEndList();

	glNewList(knight, GL_COMPILE);
	DrawPiece("KNIGHT.POL");
	glEndList();

	glNewList(bishop, GL_COMPILE);
	DrawPiece("BISHOP.POL");
	glEndList();

	glNewList(rook, GL_COMPILE);
	DrawPiece("ROOK.POL");
	glEndList();

	glNewList(queen, GL_COMPILE);
	DrawPiece("QUEEN.POL");
	glEndList();

	glNewList(king, GL_COMPILE);
	DrawPiece("KING.POL");
	glEndList();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Shapes");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(0,0,0,1);	
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
