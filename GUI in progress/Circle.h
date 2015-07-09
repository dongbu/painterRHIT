#ifndef Circle_H
#define Circle_H
#include "Shape.h"
#include <stdio.h>
class Circle :
	public Shape
{
public:
	Circle();
	~Circle();

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