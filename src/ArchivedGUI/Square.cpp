#include "Square.h"


Square::Square()
{
	this->isLine = true;
}


Square::~Square()
{
}

void Square::draw(){
	printf("drawing a square!\n");
}

void Square::putIntoXml(){

}

void Square::setIsLine(bool line){
	this->isLine = line;
}