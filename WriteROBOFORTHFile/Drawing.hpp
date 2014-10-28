#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Brush.hpp"

using namespace std;
using namespace cv;

void writeROBOFORTHFromContours(ofstream &myfile, vector<vector<Point> > &contours, double z);
void stopBrush(ofstream &myfile, Brush &currentBrush);
void startBrush(ofstream &myfile, Brush &nextBrush);
#endif
