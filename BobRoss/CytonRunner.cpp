#include "CytonRunner.h"
#include <qmessagebox>

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0

using namespace Ec;
using namespace cv;


CytonRunner::CytonRunner()
{
}


CytonRunner::~CytonRunner()
{
}

bool CytonRunner::connect(){
	if (init()){
		printf("established connection to Cyton\n");
		return true;
	}
	else{
		printf("failed to connect to Cyton\n");
	}
	return false;
}


void CytonRunner::loadWorkspace(std::string fileLocation){

}
void CytonRunner::createWorkspace(){

}
void CytonRunner::saveWorkspace(){

}
void CytonRunner::startup(){

}
void CytonRunner::shutdown(){

}
void CytonRunner::goToPos(int x, int y){

}
void CytonRunner::raiseBrush(){

}
void CytonRunner::lowerBrush(){

}
void CytonRunner::getPaint(int paint_can_id){

}
void CytonRunner::drawPoint(std::pair<int, int> pt){

}
void CytonRunner::stroke(std::pair<int, int> pt1, std::pair<int, int> pt2){

}