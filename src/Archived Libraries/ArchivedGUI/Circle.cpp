#include "Circle.h"


Circle::Circle()
{
	this->isLine = true;
}


Circle::~Circle()
{
}


void Circle::draw(){
	printf("drawing a circle!\n");
}

void Circle::putIntoXml(){

}

void Circle::setIsLine(bool line){
	this->isLine = line;
}