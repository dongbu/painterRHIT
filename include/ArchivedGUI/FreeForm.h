#ifndef FREEFORM_H
#define FREEFORM_H
#include "Shape.h"
class FreeForm :
	public Shape
{
public:
	FreeForm();
	~FreeForm();

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