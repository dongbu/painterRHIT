#ifndef Square_H
#define Square_H
#include "Shape.h"
#include <stdio.h>
class Square :
	public Shape
{
public:
	Square();
	~Square();
	
	//from Shape//
	virtual void draw();
	virtual void putIntoXml();
	virtual void setIsLine(bool is);
	//from shape//

private:
	//from shape//
	bool isLine;
	//from shape//
};

#endif

