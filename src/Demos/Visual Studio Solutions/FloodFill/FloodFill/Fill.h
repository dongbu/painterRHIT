#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace cv;

class Fill
{
public:
	void flood(Point p);
	void floodFill4Stack(Point p, int newColor, int oldColor);


	Fill();
	~Fill();

	Vec3b floodColor;
	Mat grid, processed;
};
