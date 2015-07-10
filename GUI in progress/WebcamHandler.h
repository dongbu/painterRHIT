#ifndef WEBCAMHANDLER_H
#define WEBCAMHANDLER_H
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv/cv.h>
#include "opencv2/opencv.hpp"

class WebcamHandler
{
public:
	WebcamHandler();
	virtual ~WebcamHandler();
	void openCamera(int id);
	bool cameraActive;

};

#endif // WEBCAMHANDLER_H