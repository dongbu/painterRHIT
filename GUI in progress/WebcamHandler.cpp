#include "WebcamHandler.h"


WebcamHandler::WebcamHandler()
{
	cameraActive = false;
}


WebcamHandler::~WebcamHandler()
{
}

void WebcamHandler::openCamera(int id){
	if (cameraActive) return;

	cameraActive = true;
	cv::VideoCapture stream1(id);   //0 is the id of video device.0 if you have only one camera.
	stream1.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	if (!stream1.isOpened()) { //check if video device has been initialised
		printf("cannot open camera");
	}
	printf("press a key with camera selected to capture image\n");
	printf("image will be captured after a certain time regardless\n");
	cv::Mat grayImage;
	int i = 0;
	while (true) {
		i++;
		cv::Mat cameraFrame;
		stream1.read(cameraFrame);
		cv::imshow("cam", cameraFrame);
		if (i == 350){
			printf("timeout pending/n");
		}
		if (cv::waitKey(30) >= 0 || i > 400){
			break;
		}
	}
	//imwrite("C:/Pictures/colors/green.bmp", grayImage);
	stream1.release();
	cameraActive = false;
}
