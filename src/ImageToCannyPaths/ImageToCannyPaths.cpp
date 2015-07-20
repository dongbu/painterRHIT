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
#include "../../WriteROBOFORTHFile/Brush.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cout << "Usage: ImageToCannyPaths.exe <path to image> <path to output file> <canny threshold> <z-height>" << endl;
		return 0;
	}
	char* inputFilePath = argv[1];
	char* outputFilePath = argv[2];
	ofstream outputFile;
	outputFile.open(outputFilePath);
	int cannyThresh = atoi(argv[3]);
	int z = atoi(argv[4]);

	outputFile << "CARTESIAN\n";

	Mat grayscaleImage = imread(inputFilePath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat cannyOutput;
	Canny(grayscaleImage,cannyOutput, cannyThresh, cannyThresh*2);

	vector<vector<Point> > contours;
	findContours(cannyOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(-600, 600));

	Brush rBrush = Brush("RDROP", "RGET");
	Brush gBrush = Brush("GDROP", "GGET");

	vector<Brush> brushes = {rBrush, rBrush, rBrush, rBrush};

	writeROBOFORTHFromContours(outputFile, contours, brushes, z, 0.25);
	cout << "Done" << endl;
	  imshow("GraySource", grayscaleImage);
	  waitKey(0);


	return 0;
}
