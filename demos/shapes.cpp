#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include "pugixml.hpp"
#include "pugixml.cpp"
#include "drawwindow.cpp"

//#include <highgui.h> // WINDOW_AUTOSIZE
using namespace std;

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

  virtual void draw(DrawWindow *W) { printf("hey, you should know how to draw yourself\n"); } 

  Shape() { id = -1; type = "shape"; }
  ~Shape() { }
};

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
    line.append(string_format("<shape type=\"polyline\" id=\"%i\">",getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
      line.append(string_format("<p x=\"%i\" y=\"%i\"></p>",points[i].x,points[i].y));
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
    line.append(string_format("<shape type=\"polypoints\" id=\"%i\">",getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
      line.append(string_format("<p x=\"%i\" y=\"%i\"></p>",points[i].x,points[i].y));
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
    line.append(string_format("<shape type=\"pixelregion\" id=\"%i\">",getID()));
    line.append(Shape::getColorXML());
    line.append("<points>");
    for (int i=0; i<points.size(); i++) {
      line.append(string_format("<p x=\"%i\" y=\"%i\"></p>",points[i].x,points[i].y));
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
    line.append(string_format("<shape type=\"rectangle\" id=\"%i\" fill=\"%i\">",getID(),fill));
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

/*******  SHAPES ********/

class Shapes {
protected:
  std::vector<Shape*> shapes;
  int max_id;

public:
  void addPolyLine(PolyLine *shape) {
    int id=shape->getID(max_id); 
    if (id==max_id) { max_id++; }
    shapes.push_back(shape);
  }

  void addPolyPoints(PolyPoints *shape) {
    int id=shape->getID(max_id); 
    if (id==max_id) { max_id++; }
    shapes.push_back(shape);
  }

  void addPixelRegion(PixelRegion *shape) {
    int id=shape->getID(max_id); 
    if (id==max_id) { max_id++; }
    shapes.push_back(shape);
  }

  void addRectangle(Rectangle *shape) {
    int id=shape->getID(max_id); 
    if (id==max_id) { max_id++; }
    shapes.push_back(shape);
  }

  void addShape(Shape *shape) { // test to see if need and add* for each
    int id=shape->getID(max_id); 
    if (id==max_id) { max_id++; }
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

  void drawAll(DrawWindow *W) {
    for (int i=0; i<shapes.size(); i++) {
      shapes[i]->draw(W);
    }
  }

  // draw only one command by its shape id
  void drawOne(DrawWindow *W, int id=0) {
    for (int i=0; i<shapes.size(); i++) {
      if (shapes[i]->getID() == id ) { shapes[i]->draw(W); }
    }
  }

  Shapes() { max_id=0; }
  ~Shapes() { }
};

int main(void)
{
  int debug=0;
  cv::RNG rng(12345);

  Shapes S;

  // add some lines
  int w=400;
  int h=500;
  PolyLine *PL;
  for (int i=0; i<5; i++) {
    PL = new PolyLine();
    PL->setPenColor(rng.uniform(50,250),rng.uniform(50,250),rng.uniform(50,250));
    for (int p=0; p<10; p++) {
      PL->addPoint(rng.uniform(100,w-100),rng.uniform(100,h-100));
    }
    S.addPolyLine(PL);
  }
  
  // add a poly point region
  PolyPoints *PP = new PolyPoints();
  int ww=100;
  int delx=20;
  int dely=200;
  PP->setPenColor(255,255,0);
  PP->addPoint( ww/4.0 + delx, 7*ww/8.0 + dely );
  PP->addPoint( 3*ww/4.0 + delx, 7*ww/8.0 + dely );
  PP->addPoint( 3*ww/4.0 + delx, 13*ww/16.0 + dely );
  PP->addPoint( 11*ww/16.0 + delx, 13*ww/16.0 + dely );
  PP->addPoint( 19*ww/32.0 + delx, 3*ww/8.0 + dely );
  PP->addPoint( 3*ww/4.0 + delx, 3*ww/8.0 + dely );
  PP->addPoint( 3*ww/4.0 + delx, ww/8.0 + dely );
  PP->addPoint( 26*ww/40.0 + delx, ww/8.0 + dely );
  PP->addPoint( 26*ww/40.0 + delx, ww/4.0 + dely );
  PP->addPoint( 22*ww/40.0 + delx, ww/4.0 + dely );
  PP->addPoint( 22*ww/40.0 + delx, ww/8.0 + dely );
  PP->addPoint( 18*ww/40.0 + delx, ww/8.0 + dely );
  PP->addPoint( 18*ww/40.0 + delx, ww/4.0 + dely );
  PP->addPoint( 14*ww/40.0 + delx, ww/4.0 + dely );
  PP->addPoint( 14*ww/40.0 + delx, ww/8.0 + dely );
  PP->addPoint( ww/4.0 + delx, ww/8.0 + dely );
  PP->addPoint( ww/4.0 + delx, 3*ww/8.0 + dely );
  PP->addPoint( 13*ww/32.0 + delx, 3*ww/8.0 + dely );
  PP->addPoint( 5*ww/16.0 + delx, 13*ww/16.0 + dely );
  PP->addPoint( ww/4.0 + delx, 13*ww/16.0 + dely );
  S.addPolyPoints(PP);
  
  // add pixel region
  PixelRegion *PR = new PixelRegion();
  PR->setPenColor(0,0,rng.uniform(0,244));
  for (int i=0; i<100; i++) {
    PR->addPoint(rng.uniform(0,w),rng.uniform(h-100,h));
  }
  S.addPixelRegion(PR);

  // add rectangle
  Rectangle *R = new Rectangle();
  R->setCorners(20,20,50,60);
  R->setPenColor(200,0,0);
  S.addRectangle(R);

  R = new Rectangle();
  R->setCorners(w-100,20,w-50,260);
  R->setPenColor(200,0,220);
  R->setFill(1);
  S.addRectangle(R);

  // draw the shapes
  DrawWindow W = DrawWindow(w,h,"Generated Shapes"); // w,h
  W.clearWindow(230,230,230); // default background is white
  S.drawAll(&W);
  W.show();

  // save to XML
  std::string xml = "<?xml version=\"1.0\"?>\n";
  xml.append(S.getXML());
  ofstream myfile;
  myfile.open ("shapes.xml");
  myfile << xml;
  myfile.close();
  cout << xml;

  // load up a drawing from XML

  Shapes SS;
  
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("shapes.xml");

  cout << "Load result: " << result.description() << endl;

  pugi::xml_node shapes = doc.child("shapes");

  for (pugi::xml_node shape = shapes.first_child(); shape; shape = shape.next_sibling()) {
    string type = shape.attribute("type").value();
    int id=shape.attribute("id").as_int();
    int r=shape.child("color").attribute("r").as_int();
    int g=shape.child("color").attribute("g").as_int();
    int b=shape.child("color").attribute("b").as_int();
    if (debug) cout << type;
    if (debug) printf(" shape ID:%i\n",id);
    if (debug) printf(" RGB %d %d %d\n",r,g,b);

    if (type.compare("polyline")==0) {
      PL = new PolyLine();
      PL->setPenColor(r,g,b);

      pugi::xml_node points = shape.child("points");
      for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
	int x=point.attribute("x").as_int();
	int y=point.attribute("y").as_int();
	if (debug) printf(" - point %i %i\n",x,y);
	PL->addPoint(x,y);
      }
      SS.addShape(PL);
    }

    if (type.compare("polypoints")==0) {
      PP = new PolyPoints();
      PP->setPenColor(r,g,b);

      pugi::xml_node points = shape.child("points");
      for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
	int x=point.attribute("x").as_int();
	int y=point.attribute("y").as_int();
	if (debug) printf(" - point %i %i\n",x,y);
	PP->addPoint(x,y);
      }
      SS.addShape(PP);
    }

    if (type.compare("pixelregion")==0) {
      PR = new PixelRegion();
      PR->setPenColor(r,g,b);

      pugi::xml_node points = shape.child("points");
      for (pugi::xml_node point = points.first_child(); point; point = point.next_sibling()) {
	int x=point.attribute("x").as_int();
	int y=point.attribute("y").as_int();
	if (debug) printf(" - point %i %i\n",x,y);
	PR->addPoint(x,y);
      }
      SS.addShape(PR);
    }

    if (type.compare("rectangle")==0) {
      Rectangle *R = new Rectangle();
      R->setPenColor(r,g,b);

      pugi::xml_node corners = shape.child("corners");
      int pt1x=corners.attribute("pt1x").as_int();
      int pt1y=corners.attribute("pt1y").as_int();
      int pt2x=corners.attribute("pt2x").as_int();
      int pt2y=corners.attribute("pt2y").as_int();
      if (debug) printf(" - corners %i %i %i %i\n",pt1x,pt1y,pt2x,pt2y);
      R->setCorners(pt1x,pt1y,pt2x,pt2y);
      int fill=shape.attribute("fill").as_int();
      R->setFill(fill);

      SS.addShape(R);
    }
  }

  xml = "<?xml version=\"1.0\"?>\n";
  xml.append(SS.getXML());
  cout << xml;

  DrawWindow WW = DrawWindow(w,h,"Shapes Loaded via XML"); // w,h
  WW.clearWindow(230,230,230); // default background is white
  SS.drawAll(&WW);
  WW.show();

  while (1) {
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop
  }

}
