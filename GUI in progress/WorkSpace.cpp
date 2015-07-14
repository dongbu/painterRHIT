#include "WorkSpace.h"

WorkSpace::WorkSpace(){
	printf("WorkSpace loading\n");
	projectName = "";
	projectLocation = "ProjectFiles/Temp";
	lineIndex = 0;
	commandIndex = 0;
	solidIndex = 0;
	lineAttributeIndex = 0;
	runFromAdjust = 0;
	finishIndex = 0;
	//commandStack = ???
	canvasSize = new QSize(3, 4);
	pixelToCm = 0;
	fillMethod = "random";
}