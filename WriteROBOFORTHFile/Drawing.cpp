#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Drawing.hpp"
#include "Brush.hpp"

using namespace std;
using namespace cv;

void drawContours(ofstream &myfile, vector<vector<Point> > &contours, double z, int numContours, int numPoints){
	char movetoCommand[25];
	int n=0;
	  for(int j = 0; j < numContours; j++){
		  for(int i = 0; i < numPoints; i++){
			  n=i;
			  //Move up first, then over, then down
			  if(i==0){// First point in contour
				  sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z+20);
				  myfile << movetoCommand;
			  }
		  	  sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z);
//		  	  i=n;
		  	  myfile << movetoCommand;
		  	  if(i==numPoints-1){
		  		sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", (double)contours[j][i].x, (double)contours[j][i].y, z+20);
		  		myfile << movetoCommand;
		  	  }
	  	  }
	  }
}

void stopBrush(ofstream &myfile, Brush &currentBrush){
	char movetoCommand[25];
	// Retract from drawing
	myfile << "READY\n";
	myfile << "0.0 0.0 100.0 MOVE\n";

	// Move to a position to put back the current brush
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", currentBrush.x, currentBrush.y, currentBrush.z + 100.0f);
	myfile << movetoCommand;
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", currentBrush.x, currentBrush.y, currentBrush.z);
	myfile << movetoCommand;

	// Let go of the brush
//	myfile << "UNGRIP\n";
	sprintf(movetoCommand, "TELL WRIST -500 MOVE\n");
	myfile << movetoCommand;
}

void startBrush(ofstream &myfile, Brush &nextBrush){
	char movetoCommand[25];
	// Retract
	myfile << "0.0 0.0 100.0 MOVE\n";

	// Move to a position to put back the current brush
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", nextBrush.x, nextBrush.y, nextBrush.z + 100.0f);
	myfile << movetoCommand;
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", nextBrush.x, nextBrush.y, nextBrush.z);
	myfile << movetoCommand;

	// Grab the next brush
//	myfile << "GRIP\n";
	sprintf(movetoCommand, "TELL WRIST 500 MOVE\n");
	myfile << movetoCommand;

	// Retract
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", nextBrush.x, nextBrush.y, nextBrush.z + 100.0f);
	myfile << movetoCommand;
}
