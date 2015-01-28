#ifndef BRUSH_H_INCLUDED
#define BRUSH_H_INCLUDED

#include <opencv/cv.h>
#include "Brush.hpp"

using namespace std;

class Brush{

public:
	double x;
	double y;
	double z;
	const char* dropCommand;
	const char* getCommand;

public:
	Brush();
	Brush(cv::Point p, double zCor);
	Brush(char* dropCmd, char* getCmd);

};

#endif
