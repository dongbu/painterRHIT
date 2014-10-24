#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Drawing.hpp"

using namespace std;
using namespace cv;

void drawContours(ofstream &myfile, vector<vector<Point> > contours, double z){
	int numContours = contours.size(), numPoints = contours[0].size();

	char movetoCommand[25];
	  for(int j = 0; j < numContours; j++){
		  for(int i = 0; i < numPoints; i++){
			  //Move up first, then over, then down
			  if(i==0){// First point in contour
				  sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z+20);
				  myfile << movetoCommand;
			  }
		  	  sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z);
		  	  myfile << movetoCommand;
		  	  if(i==numPoints-1){
		  		sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z+20);
		  		myfile << movetoCommand;
		  	  }
	  	  }
	  }
}
