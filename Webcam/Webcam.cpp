#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv/cv.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
	stream1.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera 1";
	}
	Mat grayImage;
	while (true) {
		Mat cameraFrame;
		stream1.read(cameraFrame);
		imshow("cam", cameraFrame);
		if (waitKey(30) >= 0)
			break;
	}
	imwrite( "C:/Pictures/colors/green.bmp", grayImage );
	stream1.release();
	return 0;
}
