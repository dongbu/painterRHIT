#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <opencv/cv.h>
#include "Drawing.hpp"
#include "Brush.hpp"

using namespace std;
using namespace cv;

#define PI acos(-1)

void parallelContours(vector<vector<Point> > &contours, double yReady,
		double yMax, int numPoints);
void randomContours(vector<vector<Point> > &contours, double yReady,
		double yMax, int numPoints, int numContours);
void triangle(vector<vector<Point> > &contours, double yReady, double yMax, int &numContours, int &numBrushes);
double fRand(double fMin, double fMax);

int main() {
	clock_t t;
	srand((unsigned int)time(&t)%UINT_MAX);
//	char msg[10];
//	for (int i = 0; i < 5; i++) {
//		sprintf(msg, "%d: %u\n", i, (unsigned int)time(&t)%UINT_MAX);
//		cout << msg;
//		usleep(10000);
//	}

	ofstream myfile;
	myfile.open("Output.ED2");

	// Initialization
	myfile << "ROBOFORTH\nSTART\nCALIBRATE\n";

	// Get to a good, safe, built-in starting position
	myfile << "READY\n";

	// Create vectors to store Points
	int numPoints = 5, numContours = 4, numBrushes = 3;

	// Create vector to store contour vectors
	vector<vector<Point> > contours(numContours, vector<Point>(numPoints));

	// Create an OpenCV Points Y coordinates and constant X and Z coordinates
	double z = -80.0; // This Z is for marker touching paper
	double yReady = 200.0, yMax = 400.0;
//	parallelContours(contours, yReady, yMax, numPoints);
//	randomContours(contours, yReady, yMax, numPoints, numContours);
	triangle(contours, yReady, yMax, numContours, numBrushes);

	// Create Brush objects to define coordinates for different brushes
	double zBrushes = 0.0;

	vector<Point> coordinates(numBrushes);

	// Set coordinates for brushes
	// Brush 1
	coordinates[0] = Point(-100.0f, 200.0f);
	// Brush 2
	coordinates[1] = Point(-100.0f, 300.0f);
	// Brush 3
	coordinates[2] = Point(-100.0f, 400.0f);

	vector<Brush> brushes(numBrushes);
	for(int i = 0; i < numBrushes; i++){
		brushes[i] = Brush(coordinates[i], zBrushes);
	}

	// Write MOVETO commands

	for (int i = 0; i < numBrushes; i++) {
		startBrush(myfile, brushes[i]);
		writeROBOFORTHFromContours(myfile, contours, z);
		stopBrush(myfile, brushes[i]);
	}

	// Retract after last brush put back
	myfile << "0.0 0.0 100.0 MOVE\n";

	// Move back to HOME position
	myfile << "HOME\n";

	myfile.close();
	return 0;
}

void parallelContours(vector<vector<Point> > &contours, double yReady,
		double yMax, int numPoints) {
	double x, y;
	for (int i = 0; i < numPoints; i++) {
		x = 0.0;
		y = yMax - i * ((yMax - yReady) / (double) numPoints); // Create different Y coordinates
		contours[0][i] = Point(x, y);  // Add point to points vector
	}
	for (int i = 0; i < numPoints; i++) {
		x = 20.0;
		y = yMax - i * ((yMax - yReady) / (double) numPoints); // Create different Y coordinates
		contours[1][i] = Point(x, y);  // Add point to points vector
	}
	for (int i = 0; i < numPoints; i++) {
		x = 40.0;
		y = yMax - i * ((yMax - yReady) / (double) numPoints); // Create different Y coordinates
		contours[2][i] = Point(x, y);  // Add point to points vector
	}
	for (int i = 0; i < numPoints; i++) {
		x = 60.0;
		y = yMax - i * ((yMax - yReady) / (double) numPoints); // Create different Y coordinates
		contours[3][i] = Point(x, y);  // Add point to points vector
	}
}

void randomContours(vector<vector<Point> > &contours, double yReady,
		double yMax, int numPoints, int numContours) {
	// Random contours -50<x<50, 0<y<400
	double startx, starty, endx, endy;
	double x, y;
	for (int j = 0; j < numContours; j++) {
		startx = fRand(-100, 100);
		starty = fRand(200, 400);
		endx = fRand(-100, 100);
		endy = fRand(200, 400);
		for (int i = 0; i < numPoints; i++) {
			x = startx - (startx - endx) * i / (numPoints - 1);
			y = starty - (starty - endy) * i / (numPoints - 1);
			contours[j][i] = Point(x, y);  // Add point to points vector
		}
	}

}

void triangle(vector<vector<Point> > &contours, double yReady, double yMax, int &numContours, int &numBrushes){
	numContours = 1;
	numBrushes = 1;
	contours[0][0] = Point(0.0f, 200.0f);
	contours[0][1] = Point(200.0f / tan(PI/3.0f), 400.0f);
	contours[0][2] = Point(-200.0f / tan(PI/3.0f), 400.0f);
	contours[0][3] = Point(0.0f, 200.0f);
	contours[0][4] = Point(0.0f, 200.0f);
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
