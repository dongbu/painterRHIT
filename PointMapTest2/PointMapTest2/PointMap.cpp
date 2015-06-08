#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Point.cpp"




class PointMap {
private:
	std::vector<Point> points;
	int size;
	int currentPos;

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
	

	void sizeMisMatch() {
		std::cerr << "size does not match";
		std::cerr << size << "does not equal" << points.size();
		std::cerr << "or";
		std::cerr << size << "does not equal" << points.size();
	}


	//note: also updates the current position for internal iterator
	Point getPointAt(int pos) {
		currentPos = pos;
		return points.at(pos);
	}

	Point getNext() {
		if (currentPos + 1 >= (points.size())) {
			return Point(NULL,NULL);
		}
		currentPos++;
		return points.at(currentPos);
	}

	Point getPrevious() {
		if (currentPos - 1 < 0) {
			return Point(NULL, NULL);
		}
		currentPos--;
		return points.at(currentPos);
	}

	void setPos(int pos) {
		if (pos < -1) {
			currentPos = -1;
		}
		else if (pos >= points.size()) {
			currentPos = points.size() - 1;
		}
		else {
			currentPos = pos;
		}
	}

	int getCurrentPos() {
		return currentPos;
	}
};