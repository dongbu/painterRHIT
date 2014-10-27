#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Brush.hpp"

using namespace std;
using namespace cv;

void drawContours(ofstream &myfile, vector<vector<Point> > &contours, double z, int numContours, int numPoints);
void stopBrush(ofstream &myfile, Brush &currentBrush);
void startBrush(ofstream &myfile, Brush &nextBrush);
#endif
