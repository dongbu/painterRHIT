
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace cv;
using namespace std;

void printUsage();
Point coord2Point(double*);

int main(int argc, char** argv) {
	Mat image;
	image = (Mat(512,512,CV_8UC3) + CV_RGB(255,255,255));

	if(argc < 2) { printUsage(); exit(0); }

	ifstream inputFile;
	inputFile.open(argv[1], ios::in);

	vector<String> stack;

	string token;
	while(inputFile >> token) {
		stack.push_back(token);
	}

	vector<vector<double*> > positions = vector<vector<double*> >();
	vector<double> workingStack;

	bool drawing = false;
	for(unsigned int i = 0; i <  stack.size(); i++) {
		double value;
		if(isdigit(stack[i][0]) || stack[i][0] == '-' || stack[i][0] == '+') {
			value = atof(stack[i].c_str());
			workingStack.push_back(value);
		} else {
			if(stack[i] == "MOVETO"){
				if(!drawing) continue;
				if(workingStack.size() < 3) {
					cout << "Ill-formed input data" << endl;
					exit(1);
				}
				double* tmp = (double*)malloc(sizeof(double) * 3);
				tmp[0] = workingStack.back();
				workingStack.pop_back();
				tmp[1] = workingStack.back();
				workingStack.pop_back();
				tmp[2] = workingStack.back();
				workingStack.pop_back();
				positions.back().push_back(tmp);

			} else if(stack[i] == "MOVE") {
				if(workingStack.size() < 3) {
					cout << "Ill-formed input data" << endl;
					exit(1);
				}
				drawing = !drawing;
				if(drawing) {
					vector<double*> tmp = vector<double*>();
					positions.push_back(tmp);
				}
				workingStack.pop_back();
				workingStack.pop_back();
				workingStack.pop_back();
			} else {
				cout << "Warning, unrecognized identifier: " << stack[i] << endl;
			}
		}
	}
	cout.flush();

	int drawWidth = 3;
	double* lastPoint;
	for(vector<vector<double*> >::iterator iter = positions.begin(); iter != positions.end(); ++iter) {
		lastPoint = (*iter)[0];
		for(vector<double*>::iterator iter2 = (*iter).begin(); iter2 != (*iter).end(); ++iter2) {
				cout << (*iter2)[0] << " " << (*iter2)[1] << " " << (*iter2)[2] << " " << endl;
				line(image, coord2Point(lastPoint), coord2Point((*iter2)), CV_RGB(0,0,0), drawWidth, 8);
				lastPoint = (*iter2);
		}
		cout << "New Path" << endl;
	}

	imshow("Display Image", image);

	waitKey(0);
	return 0;
}

Point coord2Point(double* in) {
	const static double scaleFactor = 10;
	const static Point offset = Point(0,-0);

	Point ret = Point(in[0]+offset.x,in[1]+offset.y);
	ret *= scaleFactor;
	ret += offset;
	return ret;
}

void printUsage() {
	cout << "USAGE:" << endl;
	cout << "./paintSim input_file" << endl;
}
