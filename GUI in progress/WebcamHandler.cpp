#include "WebcamHandler.h"


WebcamHandler::WebcamHandler()
{
}


WebcamHandler::~WebcamHandler()
{
}

void WebcamHandler::openCamera(int id){
	cv::VideoCapture stream1(id);   //0 is the id of video device.0 if you have only one camera.
	stream1.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	if (!stream1.isOpened()) { //check if video device has been initialised
		printf("cannot open camera");
	}
	cv::Mat grayImage;
	while (true) {
		cv::Mat cameraFrame;
		stream1.read(cameraFrame);
		cv::imshow("cam", cameraFrame);
		if (cv::waitKey(30) >= 0)
			break;
	}
	//imwrite("C:/Pictures/colors/green.bmp", grayImage);
	stream1.release();
	return;
}
