#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <cstdarg>
#include "pugixml.hpp"
#ifdef __APPLE__
#include "pugixml.cpp"
#endif
#include "drawwindow.cpp"
#include "paint_util.h"

// dunno why apple needs .cpp
#ifdef __APPLE__
#include "paint_util.cpp"
#endif

class PolyLine;
class PixelRegion;

class Shape {
protected:
	cv::Scalar pen_color;
	cv::Vec3b pen_color_vec;
	int r, g, b;
	int thickness = 0;

public:
	std::string type;
	int id;
	bool isBreakPoint;
	bool fill;
	bool hasPainted;
	bool hasSimulated;
	char pos = 'A';

	void setID(int i) { id = i; }
	int getID(int i = -1) {
		if (i >= 0) { id = 0; }
		return id;
	}

	void setPenColor(int r, int g, int b) {
		pen_color = cv::Scalar(b, g, r); // yah, in this order
		pen_color_vec[0] = b;// = pen_color[0]; // yes, ugly :(
		pen_color_vec[1] = g;// = pen_color[1];
		pen_color_vec[2] = r;// = pen_color[2];
	}

	void setThickness(int t = 1) { thickness = t; }

	cv::Scalar getPenColor() {
		return pen_color;
	}

	std::string getColorXML() {
		std::string line;
		line.append(paint_util::string_format("<color r=\"%i\" g=\"%i\" b=\"%i\"></color>", pen_color_vec[2], pen_color_vec[1], pen_color_vec[0]));
		return line;
	}

	virtual std::string getXML() {
		std::string line = getColorXML();
		return line;
	}

	virtual std::string getText(){
		std::string line = "";
		return line;
	}


	void toggleBreakPoint(bool toggle) {
		isBreakPoint = toggle;
	}

	virtual void draw(DrawWindow *W) { 
		printf("hey, you should know how to draw yourself (%i x %i)\n",W->width,W->height); 
	}

	virtual PolyLine* toPolyline(){
		printf("if you are seeing this message, this class cannot convert to polyline\n");
		return NULL;
	}

	virtual PixelRegion* toPixelRegion(){
		printf("if you are seeing this message, this class cannot convert to pixelregion\n");
		return NULL;
	}

	Shape() { id = -1; type = "shape"; isBreakPoint = false; fill = false; hasPainted = false; hasSimulated = false; }
	~Shape() { }
};

class PolyLine : public Shape {
protected:

public:
	std::vector<cv::Point> points;
	void addPoint(int i, int j) { addPoint(cv::Point(i, j)); }
	void addPoint(cv::Point pt) { points.push_back(pt); }

	virtual std::string getXML() {
		std::string line;
		line.append(paint_util::string_format("<shape type=\"polyline\" id=\"%i\" breakpoint=\"%i\" thickness=\"%i\">", getID(), isBreakPoint, thickness));
		line.append(Shape::getColorXML());
		line.append("<points>");
		for (int i = 0; i < (int)points.size(); i++) {
			line.append(paint_util::string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
		}
		line.append("</points>");
		line.append("</shape>");
		return line;
	}

	//create ABB text file for polyline
	virtual std::string getText(){
		std::string line;
		line.append("blue,");
		for (int i = 0; i < (int)points.size(); i++){
			line.append(paint_util::string_format("%i,%i,", points[i].x, points[i].y));
		}
		line.append(";,");
		return line;
	}

	virtual void draw(DrawWindow *W) {
		cv::Scalar color = getPenColor();
		W->setPenColor(color[0], color[1], color[2]);
		W->setLineThickness(thickness);
		for (int i = 0; i < (int)points.size() - 1; i++) {
			W->drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}
	}

	virtual PolyLine* toPolyline(){
		return this;
	}

	std::vector<cv::Point> getPoints(){
		return points;
	}
	PolyLine() : Shape() { type = "polyline"; thickness = 1; fill = false; }
};


// bunch of pixels
class PixelRegion : public Shape {
protected:
	int style; // 1=square, 2=circle
	std::vector<cv::Point> points;

public:
	void addPoint(cv::Point pt) { points.push_back(pt); }
	//void addPoint(int i, int j) { addPoint(cv::Point(i, j)); }
	void addPoint(int i, int j, int dup_check=0) { // add point if it doesn't exist yet
		if (dup_check) {
			int found = 0;
			for (int n = 0; n < (int)points.size(); n++) {
				if (points[n].x == i && points[n].y == j) {
					found = 1;
					n = (int)points.size();
				}
			}
			if (!found) addPoint(cv::Point(i, j));
		}
		else {
			addPoint(cv::Point(i, j));
		}
	}

	void setStyle(int s = 1) { style = s; }

	virtual std::string getXML() {
		std::string line;
		line.append(paint_util::string_format("<shape type=\"pixelregion\" id=\"%i\" breakpoint=\"%i\" thickness=\"%i\" style=\"%i\">", getID(), isBreakPoint, thickness, style));
		line.append(Shape::getColorXML());
		line.append("<points>");
		for (int i = 0; i < (int)points.size(); i++) {
			line.append(paint_util::string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
		}
		line.append("</points>");
		line.append("</shape>");
		return line;
	}

	//create ABB text file for pixel region
	virtual std::string getText(){
		std::string line;
		line.append("blue,");
		for (int i = 0; i < (int)points.size(); i++) {
			line.append(paint_util::string_format("%i,%i,", points[i].x, points[i].y));
		}
		line.append(";,");
		return line;
	}

	virtual void draw(DrawWindow *W) {
		cv::Scalar color = getPenColor();
		W->setPenColor(color[0], color[1], color[2]);
		W->setLineThickness(1);

		for (int i = 0; i < (int)points.size() - 1; i++) {
			if (thickness == 1) {
				W->drawPixel(points[i].x, points[i].y);
			}
			else {
				if (style == 1) { // square
					int delx = thickness / 2;
					int dely = thickness / 2;
					W->drawRectangle(points[i].x - delx, points[i].y - dely, points[i].x + (thickness - delx), points[i].y + (thickness - dely), 1);
					printf("R %i,%i,%i,%i\n", points[i].x - delx, points[i].y - dely, points[i].x + (thickness - delx), points[i].y + (thickness - dely));
				}
				else {
					W->drawEllipse(points[i].x, points[i].y, thickness / 2, thickness / 2, 0, 1);
					printf("E %i,%i,%i,%i\n", points[i].x, points[i].y, thickness / 2, thickness / 2);
				}
			}
		}
	}

	virtual PixelRegion* toPixelRegion(){
		return this;
	}

	std::vector<cv::Point> getPoints(){
		return this->points;
	}

	PixelRegion() : Shape() { type = "pixelregion"; style = 1; thickness = 1; fill = true; }
};


// filled in region bounded by points
class PolyPoints : public Shape {
protected:
	std::vector<cv::Point> points;

public:
	void addPoint(int i, int j) { addPoint(cv::Point(i, j)); }
	void addPoint(cv::Point pt) { points.push_back(pt); }

	// returns a polyline representation of a PolyPoints (note: just returns the perimeter)
	virtual PolyLine* toPolyline() { // note: only perimeter
		PolyLine* PL = new PolyLine();
		for (int i = 0; i < (int)points.size() - 1; i++) { PL->addPoint(points[i]); }
		PL->addPoint(points[0]);
		return PL;
	}

	// returns a pixelregion representation of a rectangle
	virtual PixelRegion* toPixelRegion() {
		// plan is to draw a temp graphic then convert it to pixels (yah ugly... )
		int minx = 9999999;
		int miny = 9999999;
		int maxx = 0;
		int maxy = 0;
		for (int i = 0; i < (int)points.size() - 1; i++) {
			if (points[i].x > maxx) { maxx = points[i].x; }
			if (points[i].x < minx) { minx = points[i].x; }
			if (points[i].y > maxy) { maxy = points[i].y; }
			if (points[i].y < miny) { miny = points[i].y; }
		}

		unsigned int n = points.size();
		int npt[] = { n };

		cv::Point **ppoints = new cv::Point*[1];
		ppoints[0] = new cv::Point[n];
		for (size_t i = 0; i < n; i++) {
			ppoints[0][i] = cv::Point(points[i].y - miny, points[i].x - minx); // dunno why y,x but seems to work
		}
		const cv::Point* ppt[1] = { ppoints[0] };

		int w = maxx - minx + 1;
		int h = maxy - miny + 1;
		cv::Mat grid = cv::Mat(w, h, CV_8UC3); // 3 channel color
		grid.setTo(cv::Scalar(255, 255, 255));
		fillPoly(grid, ppt, npt, 1, cv::Scalar(0, 0, 0), 8);

		PixelRegion* PR = new PixelRegion();
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				if (grid.at<cv::Vec3b>(i, j)[0] == 0) { // i,j is in the region
					PR->addPoint(i + minx, j + miny);
				}
			}
		}

		delete[] ppoints[0];
		delete[] ppoints;
		return PR;
	}

	virtual std::string getXML() {
		std::string line;
		line.append(paint_util::string_format("<shape type=\"polypoints\" id=\"%i\" breakpoint=\"%i\" thickness=\"%i\">", getID(), isBreakPoint, thickness));
		line.append(Shape::getColorXML());
		line.append("<points>");
		for (int i = 0; i < (int)points.size(); i++) {
			line.append(paint_util::string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
		}
		line.append("</points>");
		line.append("</shape>");
		return line;
	}

	//create ABB text file for polypoints
	virtual std::string getText(){
		std::string line;
		line.append("blue,");
		for (int i = 0; i < (int)points.size(); i++) {
			line.append(paint_util::string_format("%i,%i,", points[i].x, points[i].y));
		}
		line.append(";,");
		return line;
	}

	virtual void draw(DrawWindow *W) {
		cv::Scalar color = getPenColor();
		W->setPenColor(color[0], color[1], color[2]);
		W->setLineThickness(thickness);
		W->startPolyPoints();
		for (int i = 0; i < (int)points.size() - 1; i++) {
			W->addPolyPoint(points[i].x, points[i].y);
		}
		W->drawPolyPoints();
	}

	PolyPoints() : Shape() { type = "polyline"; thickness = 1; fill = true; }
};


class RectangleShape : public Shape {
protected:
	cv::Point pt1;
	cv::Point pt2;

public:
	void setCorners(int x1, int y1, int x2, int y2) {
		pt1 = cv::Point(x1, y1);
		pt2 = cv::Point(x2, y2);
	}

	// returns a polyline representation of a rectangle
	PolyLine* toPolyline() { // note: only perimeter
		PolyLine* PL = new PolyLine();
		PL->addPoint(pt1);
		PL->addPoint(pt2.x, pt1.y);
		PL->addPoint(pt2);
		PL->addPoint(pt1.x, pt2.y);
		PL->addPoint(pt1);
		return PL;
	}

	// returns a pixelregion representation of a rectangle
	virtual PixelRegion* toPixelRegion() {
		PixelRegion* PR = new PixelRegion();
		for (int i = std::min(pt1.x, pt2.x); i <= std::max(pt1.x, pt2.x); i++) {
			for (int j = std::min(pt1.y, pt2.y); j <= std::max(pt2.y, pt1.y); j++) {
				if (fill == 1 || (i == pt1.x || i == pt2.x || j == pt1.y || j == pt2.y)) {
					PR->addPoint(i, j);
				}
			}
		}
		return PR;
	}

	virtual void setFill(int f = 1) { fill = f; }

	virtual std::string getXML() {
		std::string line;
		line.append(paint_util::string_format("<shape type=\"rectangle\" id=\"%i\" fill=\"%i\" breakpoint=\"%i\">", getID(), fill, isBreakPoint));
		line.append(getColorXML());
		line.append(paint_util::string_format("<corners pt1x=\"%i\" pt1y=\"%i\" pt2x=\"%i\" pt2y=\"%i\"></corners>",
			pt1.x, pt1.y, pt2.x, pt2.y));
		line.append("</shape>");
		return line;
	}

	//create ABB text file for rectangle
	virtual std::string getText(){
		std::string line;
		line.append("blue,");
		line.append(paint_util::string_format("%i,%i,%i,%i,", pt1.x, pt1.y, pt2.x, pt2.y));
		line.append(";,");
		return line;
	}

	virtual void draw(DrawWindow *W) {
		cv::Scalar color = getPenColor();
		W->setPenColor(color[0], color[1], color[2]);
		W->drawRectangle(pt1.x, pt1.y, pt2.x, pt2.y, fill);
	}

	RectangleShape() : Shape() { type = "rectangle"; fill = false; }
};

class EllipseShape : public Shape {
protected:
	cv::Point pt;
	cv::Size axes;

public:
	void setData(int x, int y, double r) { // circle
		pt = cv::Point(x, y);
		axes = cv::Size(r, r);
	}

	void setData(int x, int y, double width, double height) { // ellipse
		pt = cv::Point(x, y);
		axes = cv::Size(width, height);
	}

	// returns a polyline representation of a circle [don't worry about doing a real ellipse for now]
	virtual PolyLine* toPolyline() { // note: only perimeter
		PolyLine *PL = new PolyLine();
		double radius = (axes.width + axes.height) / 4. + .4; // .25 to help anti-aliasing
		//printf("xy=%i,%i r=%f\n",pt.x,pt.y,radius);
		int n = radius * 2;
		double pi = 3.14159265358979323846; // yah, should find the math var
		for (int i = 0; i < n; i++) {
			double rad = 2.*pi*((double)i / (double)n);
			int x = pt.x + radius*cos(rad);
			int y = pt.y + radius*sin(rad);
			//printf("%i,%i at %f\n",x,y,rad);
			PL->addPoint(x, y);
		}
		PL->addPoint(pt.x + radius, pt.y);
		return PL;
	}

	// returns a pixelregion representation of a circle  [don't worry about doing a real ellipse for now]
	virtual PixelRegion* toPixelRegion() {
		PixelRegion* PR = new PixelRegion();
		double radius = (axes.width + axes.height) / 4. + .4; // .4 to help anti-aliasing

		for (int dx = 0; dx <= radius; dx++) {
			double rad = acos((double)dx / (double)radius);
			int dy = sin(rad) * radius;
			//printf("dx dy %i %i\n",dx,dy);

			for (int i = -dx; i <= dx; i++) {
				for (int j = -dy; j <= dy; j++) {
					if (i == -dx || i == dx || j == -dy || j == dy) { // just the rectangle
						if (fill || (abs(i*j) == dx*dy)) { // if !fill, then just 4 corners
							PR->addPoint(pt.x + i, pt.y + j, 1);
							//printf(" i,j = %i,%i\n",i,j);
						}
					}
				}
			}
		}
		return PR;
	}

	virtual void setFill(int f = 1) { fill = f; }

	virtual std::string getXML() {
		std::string line;
		line.append(paint_util::string_format("<shape type=\"ellipse\" id=\"%i\" fill=\"%i\" x=\"%i\" y=\"%i\" w=\"%d\" h=\"%d\" breakPoint=\"%i\">",
			getID(), fill, pt.x, pt.y, axes.width, axes.height, isBreakPoint));
		line.append(getColorXML());
		line.append("</shape>");
		return line;
	}


	//create ABB text file for an elipse
	virtual std::string getText(){
		std::string line;
		line.append("blue,");
		//line.append(paint_util::string_format("\"%i\",\"%i\",\"%i\",\"%i\",", pt1.x, pt1.y, pt2.x, pt2.y));
		//please no
		line.append(";,");
		return line;
	}

	virtual void draw(DrawWindow *W) {
		cv::Scalar color = getPenColor();
		W->setPenColor(color[0], color[1], color[2]);
		W->drawEllipse(pt.x, pt.y, axes.width, axes.height, 0, fill);
	}

	EllipseShape() : Shape() { type = "ellipse"; fill = false; }
};

/*******  SHAPES ********/

class Shapes {
protected:
	std::vector<Shape*> shapes;
	int max_id;

public:
	void addShape(Shape *shape) {
		int id = shape->getID();
		if (id < 0) {
			max_id++;
			shape->setID(max_id);
		}
		if (id > max_id) { max_id = id; }
		shapes.push_back(shape);
	}

	std::string getXML() {
		std::string line = "<shapes>\n";
		for (int i = 0; i < (int)shapes.size(); i++) {
			line.append(shapes[i]->getXML());
			line.append("\n");
		}
		line.append("</shapes>\n");
		return line;
	}

	std::string getText(){
		std::string line = "";
		for (int i = 0; i < (int)shapes.size(); i++){
			line.append(shapes[i]->getText());
		}
		return line;
	}

	void parseXML(pugi::xml_node *shapes) {
		int debug = 0;

		for (pugi::xml_node shape = shapes->first_child(); shape; shape = shape.next_sibling()) {
			std::string type = shape.attribute("type").value();
			int id = shape.attribute("id").as_int();
			int r = shape.child("color").attribute("r").as_int();
			int g = shape.child("color").attribute("g").as_int();
			int b = shape.child("color").attribute("b").as_int();
			int breakPoint = shape.attribute("breakPoint").as_int();
			if (debug) std::cout << type;
			if (debug) printf(" shape ID:%i\n", id);
			if (debug) printf(" RGB %d %d %d\n", r, g, b);

			if (type.compare("polyline") == 0) {
				PolyLine *PL = new PolyLine();
				PL->setPenColor(r, g, b);
				PL->setID(id);
				int thickness = shape.attribute("thickness").as_int();
				if (breakPoint == 1) PL->toggleBreakPoint(true);
				PL->setThickness(thickness);

				pugi::xml_node points = shape.child("points");
				for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
					int x = point.attribute("x").as_int();
					int y = point.attribute("y").as_int();
					if (debug) printf(" - point %i %i\n", x, y);
					PL->addPoint(x, y);
				}
				addShape(PL);
			}

			if (type.compare("polypoints") == 0) {
				PolyPoints *PP = new PolyPoints();
				int thickness = shape.attribute("thickness").as_int();
				PP->setPenColor(r, g, b);
				PP->setID(id);
				PP->setThickness(thickness);
				if (breakPoint == 1) PP->toggleBreakPoint(true);


				pugi::xml_node points = shape.child("points");
				for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
					int x = point.attribute("x").as_int();
					int y = point.attribute("y").as_int();
					if (debug) printf(" - point %i %i\n", x, y);
					PP->addPoint(x, y);
				}
				addShape(PP);
			}

			if (type.compare("pixelregion") == 0) {
				PixelRegion *PR = new PixelRegion();
				PR->setPenColor(r, g, b);
				PR->setID(id);
				if (breakPoint == 1) PR->toggleBreakPoint(true);
				int style = shape.attribute("style").as_int();
				int thickness = shape.attribute("thickness").as_int();
				PR->setStyle(style);
				PR->setThickness(thickness);

				pugi::xml_node points = shape.child("points");
				for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
					int x = point.attribute("x").as_int();
					int y = point.attribute("y").as_int();
					if (debug) printf(" - point %i %i\n", x, y);
					PR->addPoint(x, y);
				}
				addShape(PR);
			}

			if (type.compare("rectangle") == 0) {
				RectangleShape *R = new RectangleShape();
				R->setPenColor(r, g, b);
				R->setID(id);
				if (breakPoint == 1) R->toggleBreakPoint(true);


				pugi::xml_node corners = shape.child("corners");
				int pt1x = corners.attribute("pt1x").as_int();
				int pt1y = corners.attribute("pt1y").as_int();
				int pt2x = corners.attribute("pt2x").as_int();
				int pt2y = corners.attribute("pt2y").as_int();
				if (debug) printf(" - corners %i %i %i %i\n", pt1x, pt1y, pt2x, pt2y);
				R->setCorners(pt1x, pt1y, pt2x, pt2y);
				int fill = shape.attribute("fill").as_int();
				R->setFill(fill);
				addShape(R);
			}

			if (type.compare("ellipse") == 0) {
				EllipseShape *E = new EllipseShape();
				E->setPenColor(r, g, b);
				if (breakPoint == 1) E->toggleBreakPoint(true);
				E->setID(id);

				int x = shape.attribute("x").as_int();
				int y = shape.attribute("y").as_int();
				int w = shape.attribute("w").as_int();
				int h = shape.attribute("h").as_int();
				if (debug) printf(" - corners %i %i %d %d\n", x, y, w, h);
				E->setData(x, y, w, h);
				int fill = shape.attribute("fill").as_int();
				E->setFill(fill);
				addShape(E);
			}
		}
	}

	void drawAll(DrawWindow *W) {
		for (int i = 0; i < (int)shapes.size(); i++) {
			shapes[i]->draw(W);
		}
	}

	// draw only one command by its shape id
	// optional ugly hack to specify an override color [b/c can't figure out how to SS.at(id).setColor(r,g,b)]
	void drawOne(DrawWindow *W, int id = 0, int r = -1, int g = -1, int b = -1) {
		for (int i = 0; i < (int)shapes.size(); i++) {
			cv::Scalar color;
			if (shapes[i]->getID() == id) {
				if (r > -1 && g > -1 && b > -1) { // save and set color
					color = shapes[i]->getPenColor();
					shapes[i]->setPenColor(b, g, r);
				}
				shapes[i]->draw(W);
				if (r > -1 && g > -1 && b > -1) { // put back color
					shapes[i]->setPenColor(color[2], color[1], color[0]);
				}
			}
		}
	}

	void removeShape(int id) {
		for (int i = 0; i < (int)shapes.size(); i++) {
			if (shapes[i]->getID() == id) {
				shapes.erase(shapes.begin() + i);
				i = (int)shapes.size() + 1;
			}
		}
	}

	void removeShapeAt(int index) {
		shapes.erase(shapes.begin() + index);
	}

	Shape* at(int position) {
		return shapes.at(position);
	}

	Shape* getById(int id) {
		for (size_t i = 0; i < shapes.size(); i++) {
			if (shapes[i]->getID() == id) {
				return shapes.at(i);
			}
		}
		return NULL;
	}

	// ABC: should be renamed to something like getNumShapes as length is ambiguous
	int length() { return shapes.size(); }

	void clear() {
		shapes.clear();
		max_id = 0;
	}

	void swap(int pos1, int pos2) {
		std::iter_swap(shapes.begin() + pos1, shapes.begin() + pos2);
	}

	Shapes() { max_id = 0; }
	~Shapes() { }
};

