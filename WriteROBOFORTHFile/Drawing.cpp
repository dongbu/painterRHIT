#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include "Drawing.hpp"
#include "Brush.hpp"

using namespace std;
using namespace cv;

void writeROBOFORTHFromContours(ofstream &myfile,
		vector<vector<Point> > &contours, vector<Brush> &brushes, double z,
		double scale) {
	char movetoCommand[25];
	sprintf(movetoCommand, "1000 ACCEL !\n");
	myfile << movetoCommand;

	for (std::pair<vector<vector<Point> >::iterator, vector<Brush>::iterator> iter(
			contours.begin(), brushes.begin()); iter.first != contours.end();
			++iter.first, ++iter.second) {
		if (iter.second == brushes.begin() || (iter.second - 1)->dropCommand != iter.second->dropCommand) {
			sprintf(movetoCommand, iter.second->getCommand);
			myfile << movetoCommand << endl;
		}
		for (vector<Point>::iterator pointIterator = iter.first->begin();
				pointIterator != iter.first->end(); pointIterator++) {
			//Move up first, then over, then down
			if (pointIterator == iter.first->begin()) { // First point in contour
				sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n",
						(double) pointIterator->x * scale,
						(double) pointIterator->y * scale, z + 20);
				myfile << movetoCommand;
				myfile << "100 SPEED !" << endl;
			}
			sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n",
					(double) pointIterator->x * scale,
					(double) pointIterator->y * scale, z);
			myfile << movetoCommand;
			if ((pointIterator + 1) == iter.first->end()) {
				// Close the path
				sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n",
						(double) iter.first->begin()->x * scale,
						(double) iter.first->begin()->y * scale, z);
				myfile << movetoCommand;
				sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n",
						(double) iter.first->begin()->x * scale,
						(double) iter.first->begin()->y * scale, z + 20);
				myfile << movetoCommand;
				myfile << "1000 SPEED !" << endl;
			}
		}
		if (iter.second + 1 == brushes.end()
				|| (iter.second + 1)->dropCommand != iter.second->dropCommand) {
			sprintf(movetoCommand, iter.second->dropCommand);
			myfile << movetoCommand << endl;
		}
	}
}

void stopBrush(ofstream &myfile, Brush &currentBrush) {
	char movetoCommand[25];
	sprintf(movetoCommand, "1000 SPEED !\n");
	myfile << movetoCommand;
	sprintf(movetoCommand, "500 ACCEL !\n");
	myfile << movetoCommand;

	double plungeDepth = 100.0;
	double increment = 5.0;

	// Retract from drawing
	myfile << "READY\n";
	myfile << "0.0 0.0 100.0 MOVE\n";

	// Move to a position to put back the current brush
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", currentBrush.x,
			currentBrush.y, currentBrush.z + plungeDepth);
	myfile << movetoCommand;

	// Plunge downwards
	plunge(myfile, plungeDepth, increment);

	// Let go of the brush
	sprintf(movetoCommand, "TELL WRIST -5000 MOVE\n");
	myfile << movetoCommand;
	sprintf(movetoCommand, "CARTESIAN\n");
	myfile << movetoCommand;
}

void startBrush(ofstream &myfile, Brush &nextBrush) {
	char movetoCommand[25];
	sprintf(movetoCommand, "1000 SPEED !\n");
	myfile << movetoCommand;
	sprintf(movetoCommand, "500 ACCEL !\n");
	myfile << movetoCommand;

	double plungeDepth = 100.0;
	double increment = 5.0;

	// Retract from previous brush
	retract(myfile, plungeDepth, increment);

	// Move to a position to pick up the next brush
	sprintf(movetoCommand, "%.1f %.1f %.1f MOVETO\n", nextBrush.x, nextBrush.y,
			nextBrush.z + plungeDepth);
	myfile << movetoCommand;

	// Plunge downwards
	plunge(myfile, plungeDepth, increment);

	// Grab the next brush
	sprintf(movetoCommand, "TELL WRIST 5000 MOVE\n");
	myfile << movetoCommand;
	sprintf(movetoCommand, "CATESIAN\n");
	myfile << movetoCommand;

	// Retract upwards
	retract(myfile, plungeDepth, increment);
}

// Retract upwards
void retract(ofstream &myfile, double plungeDepth, double increment) {
	char movetoCommand[25];

	for (int i = 0; i < plungeDepth; i = i + increment) {
		sprintf(movetoCommand, "%.1f %.1f %.1f MOVE\n", 0.0f, 0.0f, increment);
		myfile << movetoCommand;
	}
}

// Plunge downwards
void plunge(ofstream &myfile, double plungeDepth, double increment) {
	char movetoCommand[25];

	for (int i = 0; i < plungeDepth; i = i + increment) {
		sprintf(movetoCommand, "%.1f %.1f %.1f MOVE\n", 0.0f, 0.0f, -increment);
		myfile << movetoCommand;
	}
}
