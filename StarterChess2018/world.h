#pragma once

const int BOARD_SIZE = 8000;

struct Cell {
	Cell();
	void Draw(int x, int y);
	bool isWhite;
	unsigned int r, g, b;
};

class World {
public:
	World();
	void Draw();
private:
	Cell mCells[BOARD_SIZE][BOARD_SIZE];
};