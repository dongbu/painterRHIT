#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Point.cpp"




class PointMap {
private:
	std::vector<Point> points;
	int size;
	int currentPos;

	//if this runs, something horrible has happened.
	void sizeMisMatch() {
		std::cerr << "size does not match";
		std::cerr << size << "does not equal" << points.size();
		std::cerr << "or";
		std::cerr << size << "does not equal" << points.size() << std::endl;
	}

public:
	

	PointMap() {
		size = 0;
		currentPos = -1;
	}
	
	int getNumberOfPoints() {
		return size;
	}

	void addPoint(int x, int y) {
		Point p = Point(x, y);
		points.push_back(p);
		size++;
		if (size != points.size() || size != points.size()) {
			sizeMisMatch();
		}
	}
	
	
	//note: also updates the current position for internal iterator
	Point getPointAt(int pos) {
		currentPos = pos;
		return points.at(pos);
	}

	//gets the next point according to internal position
	//prints out if index exceeded, but still returns point(0,0)
	Point getNext() {
		if (currentPos + 1 >= (points.size())) {
			std::cerr << "index out of bounds" << std::endl;
			return Point(NULL,NULL);
		}
		currentPos++;
		std::cout << "current position: " << currentPos << std::endl;
		return points.at(currentPos);
	}

	//gets the previous point according to internal position
	//prints out if index exceeded, but still returns point(0,0)
	Point getPrevious() {
		if (currentPos - 1 < 0) {
			std::cerr << "index out of bounds" << std::endl;
			return Point(NULL, NULL);
		}
		currentPos--;
		std::cout << "current position: " << currentPos << std::endl;
		return points.at(currentPos);
	}

	//sets the current internal position.  forces the position to be valid.
	//returns whether it was successfully set to the intended position.
	bool setPos(int pos) {
		if (pos < 0) {
			currentPos = -1;
			if (pos == -1) {
				return true;
			}
			return false;
		}
		else if (pos >= points.size()) {
			currentPos = points.size() - 1;
			return false;
		}
		else {
			currentPos = pos;
			return true;
		}
	}

	int getCurrentPos() {
		return currentPos;
	}
};