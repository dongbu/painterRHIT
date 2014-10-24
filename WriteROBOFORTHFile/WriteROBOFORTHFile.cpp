#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Drawing.hpp"

using namespace std;
using namespace cv;

int main () {
  ofstream myfile;
  myfile.open ("Output.ED2");

  // Initialization
  myfile << "ROBOFORTH\nSTART\nCALIBRATE\n";

  // Get to a good, safe, built-in starting position
  myfile << "READY\n";

  // Create vectors to store Points
  int numPoints = 5, numContours = 4;

  // Create vector to store contour vectors
  vector<vector<Point> > contours(numContours, vector<Point>(numPoints));

  // Create an OpenCV Points Y coordinates and constant X and Z coordinates
  double x, y, z = -80.0;// This Z is for marker touching paper
  double yReady = 200.0, yMax= 400.0;
  for(int i = 0; i < numPoints; i++){
	  x=0.0;
	  y = yMax - i * ((yMax - yReady) / (double)numPoints);// Create different Y coordinates
	  contours[0][i] = Point(x, y);// Add point to points vector
  }
  for(int i = 0; i < numPoints; i++){
	  x = 20.0;
  	  y = yMax - i * ((yMax - yReady) / (double)numPoints);// Create different Y coordinates
  	  contours[1][i] = Point(x, y);// Add point to points vector
  }
  for(int i = 0; i < numPoints; i++){
	  x = 40.0;
	  y = yMax - i * ((yMax - yReady) / (double)numPoints);// Create different Y coordinates
	  contours[2][i] = Point(x, y);// Add point to points vector
  }
  for(int i = 0; i < numPoints; i++){
	  x = 60.0;
	  y = yMax - i * ((yMax - yReady) / (double)numPoints);// Create different Y coordinates
	  contours[3][i] = Point(x, y);// Add point to points vector
  }

  // Write MOVETO commands
  drawContours(myfile, contours, z);


  // Move back to HOME position
  myfile << "HOME\n";

  myfile.close();
  return 0;
}
