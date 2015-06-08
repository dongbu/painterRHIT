#include "stdafx.h"

class Point {

private:
	int xPos, yPos;
public:
	Point(int x, int y){
		xPos = x;
		yPos = y;
	}

	int getX() {
		return xPos;
	}
	int getY() {
		return yPos;
	}


};