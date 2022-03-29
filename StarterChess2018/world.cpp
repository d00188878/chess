#include <iostream>
#include "glut.h"
#include "world.h"
#include "graphics.h"

Cell::Cell() {
}

void Cell::Draw(int x, int y) {
	glPushMatrix();

	if (this->isWhite) {
		glGetMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
	}
	else {
		glGetMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueMaterial);
	}

	glBegin(GL_QUADS);

	glVertex3i(x, 0, y);

	glVertex3i(x, 0, y + 2000);

	glVertex3i(x + 2000, 0, y + 2000);

	glVertex3i(x + 2000, 0, y);

	glEnd();
	glPopMatrix();
}

World::World() {
	bool isWhite = 1;
	for (int x = 0; x < BOARD_SIZE; x += 1000) {
		for (int y = 0; y < BOARD_SIZE; y += 1000) {
			mCells[x][y].isWhite = isWhite;
			/*if (isWhite) {
				std::cout << "white" << std::endl;
				mCells[x][y].r = 255;
				mCells[x][y].g = 255;
				mCells[x][y].b = 255;
			}
			else {
				std::cout << "black" << std::endl;
				mCells[x][y].r = 0;
				mCells[x][y].g = 0;
				mCells[x][y].b = 0;
			}*/
			isWhite = !isWhite;
		}
	}
}

void World::Draw() {
	bool isWhite = 1;
	for (int x = 0; x < BOARD_SIZE; x += 1000) {
		for (int y = 0; y < BOARD_SIZE; y += 1000) {
			mCells[x][y].Draw(x, y);
		}
	}
}