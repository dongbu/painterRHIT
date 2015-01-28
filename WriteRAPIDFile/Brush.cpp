#include <opencv/cv.h>
#include "Brush.hpp"

Brush::Brush(){
	x = 0.0f;
	y = 200.0f;
	z = 0.0f;
}

Brush::Brush(cv::Point p, double zCor) {
	x = p.x;
	y = p.y;
	z = zCor;
}

Brush::Brush(char* dropCmd, char* getCmd) {
	dropCommand = dropCmd;
	getCommand = getCmd;

}
