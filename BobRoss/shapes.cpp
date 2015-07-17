#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <cstdarg>
#include "pugixml.hpp"
//#include "pugixml.cpp"
#include "drawwindow.cpp"
#include <Future>
#include <cstdlib>

//#include <highgui.h> // WINDOW_AUTOSIZE
using namespace std;

#ifndef SHAPE
#define SHAPE

class Shape {
protected:
  cv::Scalar pen_color;
  cv::Vec3b pen_color_vec;
  int r,g,b;

public:
  string type;
  int id;

  void setID(int i) { id = i; }
  int getID(int i=-1) {
    if (i>=0) { id = 0; }
    return id;
  }

  void setPenColor(int r, int g, int b) {
    pen_color = cv::Scalar(b,g,r); // yah, in this order
    pen_color_vec[0]= b = pen_color[0]; // yes, ugly :(
    pen_color_vec[1]= g = pen_color[1];
    pen_color_vec[2]= r = pen_color[2];
  }

  cv::Scalar getPenColor() {
    return pen_color;
  }

  std::string getColorXML() {
    std::string line;
    line.append(string_format("<color r=\"%i\" g=\"%i\" b=\"%i\"></color>",pen_color_vec[2],pen_color_vec[1],pen_color_vec[0]));
    return line;
  }
  
  virtual std::string getXML() {
    std::string line = getColorXML();
    return line;
  }

  std::string string_format(const std::string fmt, ...) {
	  int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	  std::string str;
	  va_list ap;
	  while (1) {     // Maximum two passes on a POSIX system...
		  str.resize(size);
		  va_start(ap, fmt);
		  int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		  va_end(ap);
		  if (n > -1 && n < size) {  // Everything worked
			  str.resize(n);
			  return str;
		  }
		  if (n > -1)  // Needed size returned
			  size = n + 1;   // For null char
		  else
			  size *= 2;      // Guess at a larger size (OS specific)
	  }
	  return str;
  }

  virtual void draw(DrawWindow *W) { printf("hey, you should know how to draw yourself\n"); }

  Shape() { id = -1; type = "shape"; }
  ~Shape() { }
};
#endif

#ifndef POLYLINE
#define POLYLINE
class PolyLine: public Shape {
protected:
  int thickness;
  std::vector<cv::Point> points;

public:
  void addPoint(int i, int j) {
    points.push_back(cv::Point(i,j));
  }

  void setThickness(int t=1) { thickness=t; }

  virtual std::string getXML() {
    std::string line;
	line.append(string_format("<shape type=\"polyline\" id=\"%i\">", getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
		line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
    }
    line.append("</points>");
    line.append("</shape>");
    return line;
  }

  virtual void draw(DrawWindow *W) {
    cv::Scalar color = getPenColor();
    W->setPenColor(color[0],color[1],color[2]);
    W->setLineThickness(thickness);
    for (int i=0; i<points.size()-1; i++) {
      W->drawLine(points[i].x,points[i].y,points[i+1].x,points[i+1].y);
    }
	 
  }

  PolyLine() : Shape() { type = "polyline"; thickness = 1; }
};
#endif

#ifndef POLYPOINTS
#define POLYPOINTS
// filled in region bounded by points
class PolyPoints: public Shape {
protected:
  int thickness;
  std::vector<cv::Point> points;

public:
  void addPoint(int i, int j) {
    points.push_back(cv::Point(i,j));
  }

  void setThickness(int t=1) { thickness=t; }

  virtual std::string getXML() {
    std::string line;
	line.append(string_format("<shape type=\"polypoints\" id=\"%i\">", getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
		line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
    }
    line.append("</points>");
    line.append("</shape>");
    return line;
  }

  virtual void draw(DrawWindow *W) {
    cv::Scalar color = getPenColor();
    W->setPenColor(color[0],color[1],color[2]);
    W->setLineThickness(thickness);
    W->startPolyPoints();
    for (int i=0; i<points.size()-1; i++) {
      W->addPolyPoint(points[i].x,points[i].y);
    }
    W->drawPolyPoints();
	 
  }
  
  PolyPoints() : Shape() { type = "polyline"; thickness = 1; }
};
#endif

#ifndef PIXELREGION
#define PIXELREGION
// bunch of pixels
class PixelRegion: public Shape {
protected:
  int thickness;
  std::vector<cv::Point> points;

public:
  void addPoint(int i, int j) {
    points.push_back(cv::Point(i,j));
  }

  void setThickness(int t=1) { thickness=t; }

  virtual std::string getXML() {
    std::string line;
	line.append(string_format("<shape type=\"pixelregion\" id=\"%i\">", getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
		line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
    }
    line.append("</points>");
    line.append("</shape>");
    return line;
  }

  virtual void draw(DrawWindow *W) {
    cv::Scalar color = getPenColor();
    W->setPenColor(color[0],color[1],color[2]);
    for (int i=0; i<points.size()-1; i++) {
    W->drawPixel(points[i].x,points[i].y);
	 
    }
  }
  
  PixelRegion() : Shape() { type = "pixelregion"; }
};
#endif

#ifndef RECTANGLE
#define RECTANGLE
class Rectangle: public Shape {
protected:
  cv::Point pt1;
  cv::Point pt2;
  int fill;

public:
  void setCorners(int x1, int y1, int x2, int y2) {
    pt1 = cv::Point(x1,y1);
    pt2 = cv::Point(x2,y2);
  }

  virtual void setFill(int f=1) { fill=f; }

  virtual std::string getXML() {
    std::string line;
	line.append(string_format("<shape type=\"rectangle\" id=\"%i\" fill=\"%i\">", getID(), fill));
    line.append(getColorXML());
	line.append(string_format("<corners pt1x=\"%i\" pt1y=\"%i\" pt2x=\"%i\" pt2y=\"%i\"></corners>",
                  pt1.x,pt1.y,pt2.x,pt2.y));
    line.append("</shape>");
    return line;
  }

  virtual void draw(DrawWindow *W) {
    cv::Scalar color = getPenColor();
    W->setPenColor(color[0],color[1],color[2]);
    W->drawRectangle(pt1.x,pt1.y,pt2.x,pt2.y,fill);
	 
  }

  Rectangle() : Shape() { type = "rectangle"; fill = 0; }
};
#endif

#ifndef ELLIPSE
#define ELLIPSE
class Ellipse : public Shape {
protected:
  cv::Point pt;
  cv::Size axes;
  int fill;

public:
  void setData(int x, int y, double r) { // circle
    pt = cv::Point(x,y);
    axes = cv::Size(r,r);
  }

  void setData(int x, int y, double width, double height) { // ellipse
    pt = cv::Point(x,y);
    axes = cv::Size(width,height);
  }

  virtual void setFill(int f=1) { fill=f; }

  virtual std::string getXML() {
    std::string line;
	line.append(string_format("<shape type=\"ellipse\" id=\"%i\" fill=\"%i\" x=\"%i\" y=\"%i\" w=\"%d\" h=\"%d\">",
                  getID(),fill,pt.x,pt.y,axes.width,axes.height));
    line.append(getColorXML());
    line.append("</shape>");
    return line;
  }

  virtual void draw(DrawWindow *W) {
    cv::Scalar color = getPenColor();
    W->setPenColor(color[0],color[1],color[2]);
    W->drawEllipse(pt.x,pt.y,axes.width,axes.height,0,fill);
	 
  }

  Ellipse() : Shape() { type = "ellipse"; fill = 0; }
};
#endif

/*******  SHAPES ********/
#ifndef SHAPES
#define SHAPES
class Shapes {
protected:
  std::vector<Shape*> shapes;
  int max_id;
  std::atomic<bool> running;

public:
  void addShape(Shape *shape) {
    int id=shape->getID();
    if (id<0) {
      max_id++;
      shape->setID(max_id);
    }
    if (id>max_id) { max_id=id; }
    shapes.push_back(shape);
  }

  std::string getXML() {
    std::string line = "<shapes>\n";
    for (int i=0; i<shapes.size(); i++) {
      line.append(shapes[i]->getXML());
      line.append("\n");
    }
    line.append("</shapes>\n");
    return line;
  }

  void parseXML(pugi::xml_node *shapes) {
    int debug=0;
	printf("parsing xml\n");
    for (pugi::xml_node shape = shapes->first_child(); shape; shape = shape.next_sibling()) {
      string type = shape.attribute("type").value();
      int id=shape.attribute("id").as_int();
      int r=shape.child("color").attribute("r").as_int();
      int g=shape.child("color").attribute("g").as_int();
      int b=shape.child("color").attribute("b").as_int();
      if (debug) cout << type;
      if (debug) printf(" shape ID:%i\n",id);
      if (debug) printf(" RGB %d %d %d\n",r,g,b);

      if (type.compare("polyline")==0) {
    PolyLine *PL = new PolyLine();
    PL->setPenColor(r,g,b);
    PL->setID(id);

    pugi::xml_node points = shape.child("points");
    for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
      int x=point.attribute("x").as_int();
      int y=point.attribute("y").as_int();
      if (debug) printf(" - point %i %i\n",x,y);
      PL->addPoint(x,y);
    }
    addShape(PL);
      }

      if (type.compare("polypoints")==0) {
    PolyPoints *PP = new PolyPoints();
    PP->setPenColor(r,g,b);
    PP->setID(id);

    pugi::xml_node points = shape.child("points");
    for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
      int x=point.attribute("x").as_int();
      int y=point.attribute("y").as_int();
      if (debug) printf(" - point %i %i\n",x,y);
      PP->addPoint(x,y);
    }
    addShape(PP);
      }

      if (type.compare("pixelregion")==0) {
    PixelRegion *PR = new PixelRegion();
    PR->setPenColor(r,g,b);
    PR->setID(id);

    pugi::xml_node points = shape.child("points");
    for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
      int x=point.attribute("x").as_int();
      int y=point.attribute("y").as_int();
      if (debug) printf(" - point %i %i\n",x,y);
      PR->addPoint(x,y);
    }
    addShape(PR);
      }

      if (type.compare("rectangle")==0) {
    Rectangle *R = new Rectangle();
    R->setPenColor(r,g,b);
    R->setID(id);

    pugi::xml_node corners = shape.child("corners");
    int pt1x=corners.attribute("pt1x").as_int();
    int pt1y=corners.attribute("pt1y").as_int();
    int pt2x=corners.attribute("pt2x").as_int();
    int pt2y=corners.attribute("pt2y").as_int();
    if (debug) printf(" - corners %i %i %i %i\n",pt1x,pt1y,pt2x,pt2y);
    R->setCorners(pt1x,pt1y,pt2x,pt2y);
    int fill=shape.attribute("fill").as_int();
    R->setFill(fill);
    addShape(R);
      }

      if (type.compare("ellipse")==0) {
    Ellipse *E = new Ellipse();
    E->setPenColor(r,g,b);
    E->setID(id);

    int x=shape.attribute("x").as_int();
    int y=shape.attribute("y").as_int();
    int w=shape.attribute("w").as_int();
    int h=shape.attribute("h").as_int();
    if (debug) printf(" - corners %i %i %d %d\n",x,y,w,h);
    E->setData(x,y,w,h);
    int fill=shape.attribute("fill").as_int();
    E->setFill(fill);
    addShape(E);
      }
    }
  }

  void drawAll(DrawWindow *W) {
	  running = true;
	  auto d1 = std::async(&Shapes::drawAllHelper, this, W);
  }

  void drawAllHelper(DrawWindow *W){
	  for (int i = 0; i<shapes.size(); i++) {
		  if (!running) { return; }
		  W->show();
		  shapes[i]->draw(W);
		  _sleep(1000);
	  }
	  
  }

  void setRunning(bool toggle){
	  running = toggle;
  }

  // draw only one command by its shape id
  void drawOne(DrawWindow *W, int id=0) {
    for (int i=0; i<shapes.size(); i++) {
      if (shapes[i]->getID() == id ) { shapes[i]->draw(W); }
    }
  }

  void removeShape(int id) {
    for (int i=0; i<shapes.size(); i++) {
      if (shapes[i]->getID() == id) {
    shapes.erase(shapes.begin() + i);
    i=shapes.size()+1;
      }
    }
  }

  void removeShapeAt(int index){
	  shapes.erase(shapes.begin() + index);
  }

  Shape* at(int position){
	  return shapes.at(position);
  }

  Shape* getById(int id){
	  for (int i = 0; i < shapes.size(); i++){
		  if (shapes[i]->getID() == id){
			  return shapes.at(i);
		  }
	  }
  }

  int length(){
	  return shapes.size();
  }

  void swap(int pos1, int pos2){
	  printf("check 1\n");
	  std::iter_swap(shapes.begin() + pos1, shapes.begin() + pos2);
	  printf("check 2\n");
  }

  Shapes() { max_id=0; }
  ~Shapes() { }
};
#endif
