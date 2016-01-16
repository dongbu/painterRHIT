// 6_Color_Identification.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

char key;

int _tmain(int argc, _TCHAR* argv[])
{
	//GaussianBlur(frame, frame, Size(7, 7), 1.5, 1.5);  //BLUR AN IMAGE
	//frame = frame(Range(0, 400), Range(0, 100));  //SLICE AN IMAGE
	//inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst); //MASK AN IMAGE
	//element-wise multiplication is A.mul(B)
	//bitwise_and(InputArray src1, InputArray src2, OutputArray dst)

	//Look into automatic thresholding


	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	for (;;)
	{
		Mat frame, mask[3], dddMask;

		//Read in a frame from a webcam
		cap >> frame;

		//convert to HSV colorspace
		cvtColor(frame, frame, CV_BGR2HSV);

		//create & store a 1d mask
		Scalar lower_blue = Scalar(90, 75, 75);
		Scalar upper_blue = Scalar(120, 255, 255);

		inRange(frame, lower_blue, upper_blue, mask[0]);

		//create & apply a 3d mask
		mask[1] = mask[0]; mask[2] = mask[0];
		merge(mask, 3, dddMask);
		bitwise_and(frame, dddMask, frame);

		//convert back to original colorspace && display final results
		cvtColor(frame, frame, CV_HSV2BGR);
		erode(frame, frame, Mat());
		erode(frame, frame, Mat());
		imshow("Color-Filtered Image", frame); //display image

		if (waitKey(10) >= 0) break; //stop showing things if any key is pressed

		//Mat chan[3];
		//split(frame, chan);
		//Mat H = chan[0]; Mat S = chan[1]; Mat V = chan[2];

		////Creating & applying 'blue' mask
		//Mat temp;
		//inRange(H, 90, 112, H); //blue range
		//temp = H.mul(S); divide(temp, H, S);
		//temp = H.mul(V); divide(temp, H, V);

		////bitwise_and(H, S, S); //pruning saturations
		////bitwise_and(H, V, V); //pruning values
		//chan[0] = H; chan[1] = S; chan[2] = V;
		//merge(chan, 3, frame);
		//cvtColor(frame, frame, CV_HSV2BGR); //convert to HSV colorspace
		//GaussianBlur(frame, frame, Size(7, 7), 1.5, 1.5);  //BLUR AN IMAGE

		////bitwise_and(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray());
		////bitwise_and(H, frame, frame, H);

		//Mat mask;
		//threshold(H, mask, 90, 1, 0); //binary threshold for Hues 90-112 degrees

		//////Displaying
		//cvtColor(frame, frame, CV_HSV2BGR);
		//imshow("blue-masked frame", frame);

	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}