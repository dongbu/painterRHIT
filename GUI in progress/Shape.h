#ifndef SHAPE_H
#define SHAPE_H
class Shape
{
public:
	//deconstructor
	virtual ~Shape() = 0;

	//puts information into an xml file.
	virtual void putIntoXml() = 0;
	
	//sets the variable isLine.  (false indicates a solid).
	virtual void setIsLine(bool is) = 0;

	//draws the function.
	virtual void draw() = 0;

protected:
	bool isLine;
};
#endif
