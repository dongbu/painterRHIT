#ifndef BRUSH_H_INCLUDED
#define BRUSH_H_INCLUDED

#include <opencv/cv.h>
#include "Brush.hpp"

class Brush{

public:
	double x;
	double y;
	double z;
	Brush(cv::Point p, double zCor);


public:
	Brush();
};

#endif
