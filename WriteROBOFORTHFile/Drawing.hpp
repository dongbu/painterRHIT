#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

void drawContours(ofstream &myfile, vector<vector<Point> > contours, double z);
#endif
