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
#include <atomic>

//#include <highgui.h> // WINDOW_AUTOSIZE
using namespace std;

#ifndef SHAPE
#define SHAPE

/**
 * @brief The Shape class
 */
class Shape {
protected:
    cv::Scalar pen_color;
    cv::Vec3b pen_color_vec;
    int r,g,b;


public:
    string type;
    int id;
    bool isBreakPoint = false;

    /**
   * @brief sets the id of the shape
   * @param i
   */
    void setID(int i) { id = i; }
    int getID(int i=-1) {
        if (i>=0) { id = 0; }
        return id;
    }

    /**
   * @brief sets the pen color of the shape.
   * @param r
   * @param g
   * @param b
   */
    void setPenColor(int r, int g, int b) {
        pen_color = cv::Scalar(b,g,r); // yah, in this order
        pen_color_vec[0]= b = pen_color[0]; // yes, ugly :(
        pen_color_vec[1]= g = pen_color[1];
        pen_color_vec[2]= r = pen_color[2];
    }

    /**
   * @brief returns the pen color
   * @return cv::Scalar pen_color
   */
    cv::Scalar getPenColor() {
        return pen_color;
    }

    /**
   * @brief get xml color information.
   * @return line
   */
    std::string getColorXML() {
        std::string line;
        line.append(string_format("<color r=\"%i\" g=\"%i\" b=\"%i\"></color>",pen_color_vec[2],pen_color_vec[1],pen_color_vec[0]));
        return line;
    }

    /**
   * @brief get xml information.
   * @return line
   */
    virtual std::string getXML() {
        std::string line = getColorXML();
        return line;
    }

    /**
   * @brief toggles whether or not the shape has a breakpoint.
   * @param toggle
   */
    void toggleBreakPoint(bool toggle){
        this->isBreakPoint = toggle;
    }

    /**
   * @brief I honestly have no idea what this does.
   * @param fmt
   * @return std::string
   */
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
/**
 * @brief The PolyLine class
 */
class PolyLine: public Shape {
protected:
    int thickness;

public:
    std::vector<cv::Point> points;

    /**
   * @brief adds points to the PolyLine.
   * @param i
   * @param j
   */
    void addPoint(int i, int j) {
        points.push_back(cv::Point(i,j));
    }

    /**
     * @brief set the thickness of the lines.
     * @param t
     */
    void setThickness(int t=1) { thickness=t; }

    /**
     * @brief gets xml information
     * @return std::string
     */
    virtual std::string getXML() {
        std::string line;
        line.append(string_format("<shape type=\"polyline\" id=\"%i\" breakPoint=\"%i\" thickness=\"%i\">", getID(), isBreakPoint,thickness));
        line.append(Shape::getColorXML());
        line.append("<points>");
        for (int i=0; i<points.size(); i++) {
            line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
        }
        line.append("</points>");
        line.append("</shape>");
        return line;
    }

    /**
     * @brief draw's self
     * @param drawwindow
     */
    virtual void draw(DrawWindow *W) {
        cv::Scalar color = getPenColor();
        W->setPenColor(color[0],color[1],color[2]);
        W->setLineThickness(thickness);
        for (int i=0; i<points.size()-1; i++) {
            W->drawLine(points[i].x,points[i].y,points[i+1].x,points[i+1].y);
        }
    }
    /**
     * @brief constructor
     */
    PolyLine() : Shape() { type = "polyline"; thickness = 1; }
};
#endif

#ifndef POLYPOINTS
#define POLYPOINTS
// filled in region bounded by points
/**
 * @brief The PolyPoints class
 */
class PolyPoints: public Shape {
protected:
    int thickness;
    std::vector<cv::Point> points;

public:
    /**
     * @brief add points to PolyPoints
     * @param i
     * @param j
     */
    void addPoint(int i, int j) {
        points.push_back(cv::Point(i,j));
    }

    /**
     * @brief sets thickness of lines
     * @param t
     */
    void setThickness(int t=1) { thickness=t; }

    /**
     * @brief get XML information
     * @return std::string
     */
    virtual std::string getXML() {
        std::string line;
		line.append(string_format("<shape type=\"polypoints\" id=\"%i\" breakPoint=\"%i\" thickness=\"%i\">", getID(), isBreakPoint,thickness));
        line.append(Shape::getColorXML());
        line.append("<points>");
        for (int i=0; i<points.size(); i++) {
            line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
        }
        line.append("</points>");
        line.append("</shape>");
        return line;
    }

    /**
     * @brief draw self
     * @param draw window
     */
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

    /**
     * @brief constructor
     */
    PolyPoints() : Shape() { type = "polyline"; thickness = 1; }
};
#endif

#ifndef PIXELREGION
#define PIXELREGION
// bunch of pixels
/**
 * @brief The PixelRegion class
 */
class PixelRegion: public Shape {
protected:
    std::vector<cv::Point> points;

public:
    /**
     * @brief add point to Pixel Region
     * @param i
     * @param j
     */
    void addPoint(int i, int j) {
        points.push_back(cv::Point(i,j));
    }

    /**
     * @brief get XML information
     * @return std::string
     */
    virtual std::string getXML() {
        std::string line;
		line.append(string_format("<shape type=\"pixelregion\" id=\"%i\" breakPoint=\"%i\">", getID(), isBreakPoint));
        line.append(Shape::getColorXML());
        line.append("<points>");
        for (int i=0; i<points.size(); i++) {
            line.append(string_format("<p x=\"%i\" y=\"%i\"></p>", points[i].x, points[i].y));
        }
        line.append("</points>");
        line.append("</shape>");
        return line;
    }

    /**
     * @brief draw self
     * @param DrawWindow
     */
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
/**
 * @brief The Rectangle class
 */
class Rectangle: public Shape {
protected:
    cv::Point pt1;
    cv::Point pt2;
    int fill;

public:
    /**
     * @brief set the corners of the rectangle
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void setCorners(int x1, int y1, int x2, int y2) {
        pt1 = cv::Point(x1,y1);
        pt2 = cv::Point(x2,y2);
    }

    /**
     * @brief set fill type
     * @param f
     */
    virtual void setFill(int f=1) { fill=f; }

    /**
     * @brief get xml information
     * @return std::string
     */
    virtual std::string getXML() {
        std::string line;
		line.append(string_format("<shape type=\"rectangle\" id=\"%i\" fill=\"%i\" breakPoint=\"%i\">", getID(), fill, isBreakPoint));
        line.append(getColorXML());
        line.append(string_format("<corners pt1x=\"%i\" pt1y=\"%i\" pt2x=\"%i\" pt2y=\"%i\"></corners>",
                                  pt1.x,pt1.y,pt2.x,pt2.y));
        line.append("</shape>");
        return line;
    }

    /**
     * @brief draw self
     * @param Draw Window
     */
    virtual void draw(DrawWindow *W) {
        cv::Scalar color = getPenColor();
        W->setPenColor(color[0],color[1],color[2]);
        W->drawRectangle(pt1.x,pt1.y,pt2.x,pt2.y,fill);

    }

    /**
     * @brief constructor
     */
    Rectangle() : Shape() { type = "rectangle"; fill = 0; }
};
#endif

#ifndef ELLIPSE
#define ELLIPSE
/**
 * @brief The Ellipse class
 */
class Ellipse : public Shape {
protected:
    cv::Point pt;
    cv::Size axes;
    int fill;

public:
    /**
     * @brief set elipse required data
     * @param x
     * @param y
     * @param r
     */
    void setData(int x, int y, double r) { // circle
        pt = cv::Point(x,y);
        axes = cv::Size(r,r);
    }

    /**
     * @brief set elipse required data
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void setData(int x, int y, double width, double height) { // ellipse
        pt = cv::Point(x,y);
        axes = cv::Size(width,height);
    }

    /**
     * @brief set fill type
     * @param f
     */
    virtual void setFill(int f=1) { fill=f; }

    virtual std::string getXML() {
        std::string line;
        line.append(string_format("<shape type=\"ellipse\" id=\"%i\" fill=\"%i\" x=\"%i\" y=\"%i\" w=\"%d\" h=\"%d\" breakPoint=\"%i\">",
                                  getID(),fill,pt.x,pt.y,axes.width,axes.height, isBreakPoint));
        line.append(getColorXML());
        line.append("</shape>");
        return line;
    }

    /**
     * @brief draw self
     * @param Draw Window
     */
    virtual void draw(DrawWindow *W) {
        cv::Scalar color = getPenColor();
        W->setPenColor(color[0],color[1],color[2]);
        W->drawEllipse(pt.x,pt.y,axes.width,axes.height,0,fill);

    }

    /**
     * @brief constructor
     */
    Ellipse() : Shape() { type = "ellipse"; fill = 0; }
};
#endif

/*******  SHAPES ********/
#ifndef SHAPES
#define SHAPES
/**
 * @brief The Shapes class
 */
class Shapes {
protected:
    std::vector<Shape*> shapes;
    int max_id;

public:
    /**
     * @brief add shape to vector of shapes.
     * @param shape
     */
    void addShape(Shape *shape) {
        int id=shape->getID();
        if (id<0) {
            max_id++;
            shape->setID(max_id);
        }
        if (id>max_id) { max_id=id; }
        shapes.push_back(shape);
    }

    /**
     * @brief get XML information
     * @return std::string
     */
    std::string getXML() {
        std::string line = "<shapes>\n";
        for (int i=0; i<shapes.size(); i++) {
            line.append(shapes[i]->getXML());
            line.append("\n");
        }
        line.append("</shapes>\n");
        return line;
    }

    /**
     * @brief read XML and parse it into shapes.
     * @param shapes
     */
    void parseXML(pugi::xml_node *shapes) {
        int debug=0;
        for (pugi::xml_node shape = shapes->first_child(); shape; shape = shape.next_sibling()) {
            string type = shape.attribute("type").value();
            int id=shape.attribute("id").as_int();
            int r=shape.child("color").attribute("r").as_int();
            int g=shape.child("color").attribute("g").as_int();
            int b=shape.child("color").attribute("b").as_int();
			int breakPoint = shape.attribute("breakPoint").as_int();
            if (debug) cout << type;
            if (debug) printf(" shape ID:%i\n",id);
            if (debug) printf(" RGB %d %d %d\n",r,g,b);

            if (type.compare("polyline")==0) {
                PolyLine *PL = new PolyLine();
				int thickness = shape.attribute("thickness").as_int();
                PL->setPenColor(r,g,b);
                PL->setID(id);
				PL->setThickness(thickness);
				if (breakPoint == 1) PL->toggleBreakPoint(true);

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
				int thickness = shape.attribute("thickness").as_int();
                PP->setPenColor(r,g,b);
                PP->setID(id);
				PP->setThickness(thickness);
				if (breakPoint == 1) PP->toggleBreakPoint(true);

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
				if (breakPoint == 1) PR->toggleBreakPoint(true);

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
				if (breakPoint == 1) R->toggleBreakPoint(true);

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
				if (breakPoint == 1) E->toggleBreakPoint(true);

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

    /**
     * @brief tell all the shapes to draw themselves.
     * @param Draw Window
     */
    void DrawAll(DrawWindow *W) {
        for (int i = 0; i < shapes.size(); i++) {
            shapes.at(i);
            shapes.at(i)->draw(W);
        }
    }

    // draw only one command by its shape id
    /**
     * @brief tell one command (by ID) to draw itself.
     * @param Draw Window
     * @param id
     */
    void drawOne(DrawWindow *W, int id=0) {
        for (int i=0; i<shapes.size(); i++) {
            if (shapes[i]->getID() == id ) { shapes[i]->draw(W); }
        }
    }

    /**
     * @brief remove Shape by ID
     * @param id
     */
    void removeShape(int id) {
        for (int i=0; i<shapes.size(); i++) {
            if (shapes[i]->getID() == id) {
                shapes.erase(shapes.begin() + i);
                i=shapes.size()+1;
            }
        }
    }

    /**
     * @brief remove Shape by index.
     * @param index
     */
    void removeShapeAt(int index){
        shapes.erase(shapes.begin() + index);
    }

    /**
     * @brief get shape at index.
     * @param position
     * @return Shape
     */
    Shape* at(int position){
        return shapes.at(position);
    }

    /**
     * @brief get shape by ID
     * @param id
     * @return Shape
     */
    Shape* getById(int id){
        for (int i = 0; i < shapes.size(); i++){
            if (shapes[i]->getID() == id){
                return shapes.at(i);
            }
        }
    }

    /**
     * @brief number of shapes.
     * @return
     */
    int length(){
        return shapes.size();
    }

    /**
     * @brief clear shapes vector.
     */
    void clear(){
        shapes.clear();
    }

    /**
     * @brief swap two shapes positions in the vector.
     * @param pos1
     * @param pos2
     */
    void swap(int pos1, int pos2){
        std::iter_swap(shapes.begin() + pos1, shapes.begin() + pos2);
    }

    /**
     * @brief constructor
     */
    Shapes() { max_id=0; }
    /**
      * @brief deconstructor
      */
    ~Shapes() { }
};
#endif
