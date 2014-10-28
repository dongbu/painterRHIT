//============================================================================
// Name        : ImageToCannyPaths.cpp
// Author      : Ben Paul and Sean Kling
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../../WriteROBOFORTHFile/Drawing.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Usage: ImageToCannyPaths.exe <path to image> <path to output file> <canny threshold>" << endl;
		return 0;
	}
	char* inputFilePath = argv[1];
	char* outputFilePath = argv[2];
	ofstream outputFile;
	outputFile.open(outputFilePath);
	int cannyThresh = atoi(argv[3]);

	Mat grayscaleImage = imread(inputFilePath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat cannyOutput;
	Canny(grayscaleImage,cannyOutput, cannyThresh, cannyThresh*2);

	vector<vector<Point> > contours;
	findContours(cannyOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1);

	writeROBOFORTHFromContours(outputFile, contours, 100);
	cout << "Done" << endl;
	return 0;
}
