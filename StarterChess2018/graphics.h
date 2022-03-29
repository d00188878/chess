#include "glut.h"

void text_output(double x, double y, char *string);
void DrawCircle(double x1, double y1, double radius);
void DrawRectangle(double x1, double y1, double x2, double y2);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

// "Prototypes" of some color variables defined in graphics.cpp
extern GLdouble redMaterial[];
extern GLfloat greenMaterial[];
extern GLdouble brightGreenMaterial[];
extern GLfloat blueMaterial[];
extern GLfloat whiteMaterial[];

enum gPieces { pawn = 100, knight, bishop, rook, queen, king };