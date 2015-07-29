#pragma once
//#include "stdafx.h"
#include "drawwindow.cpp"
#include "pixeltools.cpp"

// Tools to take a set of desired points to paint and returns paths for a brush
// see regiontopaths_demo.cpp for example use

// Object to keep track of brush profile information
class Brush {
protected:
  int width, height, centerx, centery;
  int style; // 1=square, 2=ellipse
  int draw_mode; //1=opaque, 2=simulate paint
  std::vector<cv::Point> pixels;
  cv::Point draw_offset; // used when want to draw brush offset from 0,0
  cv::Scalar color;
  double *pixel_paint; // how much paint is on each brush pixel (unit = how many pixels expected to be able to paint)
  cv::RNG rng;

public:
  cv::Mat brush;

  int getWidth() { return width; }
  int getHeight() { return height; }
  cv::Point getCenter() { return cv::Point(centerx,centery); }
  std::vector<cv::Point> getPixels() { return pixels; }
  void setColor(cv::Scalar c) { color=c; }
  void setColor(int r, int g, int b) { color=cv::Scalar(b,g,r); }
  cv::Scalar getColor() { return color; }
  void setDrawMode(std::string mode="normal") { 
    if (mode.compare("normal")==0) { draw_mode=1; }
    if (mode.compare("paint")==0) { draw_mode=2; }
  }

  void setDrawOffset(int x=0,int y=0) { draw_offset=cv::Point(x,y); }

  // not a beautiful place for this but c'est la vie
  std::string string_format(const std::string fmt, ...) {
	  int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	  std::string str;
	  va_list ap;
	  while (1) {     // Maximum two passes on a POSIX system...
		  str.resize(size);
		  va_start(ap, fmt);
		  int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		  va_end(ap);
		  if (n > -1 && n < size) { str.resize(n); return str; }
		  if (n > -1) size = n + 1; else size *= 2;
	  }
	  return str;
  }

  std::string getColorXML() {
    std::string line;
    line.append(string_format("<color r=\"%i\" g=\"%i\" b=\"%i\"></color>",color[2],color[1],color[0]));
    return line;
  }

  virtual std::string getXML() {
    std::string line;
    line.append(string_format("<shape type=\"brush\" w=\"%i\" h=\"%i\" style=\"%i\"",width,height,style));
    line.append(getColorXML());
    line.append("<points>");
    for (int i=0; i<(int)pixels.size(); i++) {
      line.append(string_format("<p x=\"%i\" y=\"%i\"></p>",pixels[i].x,pixels[i].y));
    }
    line.append("</points>");
    line.append("</shape>");
    return line;
  }

  // put 'amount' of paint on each brush pixel
  void loadPaintPixel(double amount) {
    for (int i=0; i<(int)pixels.size(); i++) {
      float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      pixel_paint[pixels[i].x*height+pixels[i].y] = amount * (.9 + 2*r); 
    }
  }

  // draw the brush at a single point
  void draw(DrawWindow *W, cv::Point pt) {
    W->setPenColor(color);
    cv::Vec3b canvas_color;
    int num_pixels = (int)pixels.size();
    
    for (int b=0; b<(int)pixels.size(); b++) {
      int px=pixels[b].x;
      int py=pixels[b].y;
      int x=pt.x - centerx + px + draw_offset.x;
      int y=pt.y - centery + py + draw_offset.y;
      //printf("%i %i %i %i\n",px,py,x,y);
      if (draw_mode==1) {
	W->drawPixel(x, y); 
      } else { //=2 try to simulate paint
	int id = px*height + py;
	double paint = pixel_paint[id]; // how much paint in on the brush pixel

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	double paint_used = std::fmin(pixel_paint[id],0.5 + r);
	if (paint<10) { // use less per pixel
	  paint_used=paint_used * paint/5;
	}
	pixel_paint[id] -= paint_used;
	double opacity = std::fmin(1,1.0/num_pixels * paint_used * paint);
	if (opacity>0) {
	  //printf("P%i: (%i,%i)  %f %f %f\n",b,px,py,paint,opacity,paint_used);
	  canvas_color = W->getPixel(x,y);
	  for (int i=0; i<3; i++) { canvas_color[i]=(1-opacity)*canvas_color[i]+opacity*color[i]; }
	  W->setPenColor(canvas_color);
	  W->drawPixel(x, y);
	}
      }
    }
  }
  void draw(DrawWindow *W, int x, int y) { draw(W,cv::Point(x,y)); }
  
  // draw the brush along a list of points (assume contiguous)
  void drawContiguousPoints(DrawWindow *W, std::vector<cv::Point> *points) {
    for (int i=0; i<(int)points->size(); i++) { 
      cv::Point pt = points->at(i);
      draw(W,pt); 
    }
  }

  // draw the brush between two points
  void drawLine(DrawWindow *W, int p1x, int p1y, int p2x, int p2y) {
    PixelTools Tool;
    std::vector<cv::Point> points = Tool.getPixelsBetweenPoints(p1x,p1y,p2x,p2y);
    drawContiguousPoints(W,&points);
  }

  // draw the brush along a list of points
  void drawPoints(DrawWindow *W, std::vector<cv::Point> *points) {
    for (int i=0; i<(int)points->size(); i++) { draw(W,points->at(i)); }
  }

  // returns 0-1 how close color
  double colorDistance(cv::Scalar c1, cv::Scalar c2) {
    int norm = std::abs(c1[0]-c2[0]) + std::abs(c1[1]-c2[1]) + std::abs(c1[2]-c2[2]);
    //printf("N1:%i\n",norm);
    return (double)norm/(255. * 3.);
  }

  // returns what fraction of brush pixels of grid at a point are close to brush color
  double scorePaintPoint(cv::Mat *grid, cv::Point pt, double max_distance) {
    int num_pixels = (int)pixels.size();
    if (!num_pixels) { return 0.0; }
    int num_bad_pixels=0;
    for (int b=0; b<num_pixels; b++) {
      int px=pixels[b].x;
      int py=pixels[b].y;
      int x=pt.x + px + draw_offset.x;
      int y=pt.y + py + draw_offset.y;
      cv::Vec3b grid_color = grid->at<cv::Vec3b>(cv::Point(x,y));
      double distance = colorDistance(grid_color,color);
      if (distance>max_distance) { num_bad_pixels++; }
    }
    return (double)(num_pixels-num_bad_pixels)/(double)num_pixels;
  }

  // returns the % of pixels that are adequately covered by the brush
  double scorePaintPoints(DrawWindow *W, std::vector<cv::Point> *points, std::vector<cv::Point> *errors=NULL, 
			double max_pixel_bad_ratio=0, double max_distance=0) {
    if (!max_pixel_bad_ratio) { max_pixel_bad_ratio = .1; }
    if (!max_distance) { max_distance=.2; } 
    int num_pixels = (int)points->size();
    int num_bad_pixels=0;
    if (!num_pixels) { return 0; } // note: not needed
    for (int i=0; i<num_pixels; i++) { 
      double score_point = scorePaintPoint(&W->grid,points->at(i),max_distance);
      if (score_point<max_pixel_bad_ratio) { 
	num_bad_pixels++; 
	if (errors) { errors->push_back(points->at(i)); }
      }
      //printf("P%i: (%i,%i), %f  NBAD:%i\n",i,points[i].x,points[i].y,score_point,num_bad_pixels);
    }
    return (double)(num_pixels-num_bad_pixels)/(double)num_pixels;
  }

  // returns the % of pixels along a line are adequately covered by the brush
  double scorePaintLine(DrawWindow *W, int p1x, int p1y, int p2x, int p2y, std::vector<cv::Point> *errors=NULL, 
			double max_pixel_bad_ratio=0, double max_distance=0) {
    PixelTools Tool;
    std::vector<cv::Point> points = Tool.getPixelsBetweenPoints(p1x,p1y,p2x,p2y);
    return scorePaintPoints(W,&points,errors,max_pixel_bad_ratio,max_distance);
  }

  Brush(int w, int h, std::string type="rectangle", int show_brush = 0) {
    // create brush mask
    width = w; // max dimensions of brush
    height = h;
    centerx = w/2;
    centery = h/2;
    draw_mode = 1; // default brush to be opaque
    pixel_paint = new double [width*height]; 
    for (int i=0; i<width*height; i++) { pixel_paint[i]=0.; }
    draw_offset=cv::Point(0,0);

    //ABC: could simplify brush to an int mask as does not need to be color'd (but ok so can use cv::)zv
    cv::Scalar brush_color = cv::Scalar(0,0,0); 
    cv::Scalar not_brush_color = cv::Scalar(255,255,255); 
    brush = cv::Mat::zeros(cv::Size(width,height), CV_8UC3);
    brush.setTo(not_brush_color);
    
    cv::Vec3b brush_color_vec3b(brush_color[0],brush_color[1],brush_color[2]);
    
    // draw brush (note: this is so we can use arbitrary shaped brushes
    if (type.compare("ellipse")==0) {
      style = 2;
      cv::ellipse( brush, cv::Point(std::floor(w/2),std::floor(h/2)), 
		   cv::Size(std::floor(width/2), std::floor(height/2)), 0., 0., 360., brush_color, -1);
    } else {
      style = 1;
      cv::rectangle( brush, cv::Point(0,0), cv::Point(width,height), brush_color, -1);
    }
    if (show_brush) {
      cv::namedWindow( "Brush", CV_WINDOW_AUTOSIZE );
      cv::imshow( "Brush", brush );
    }
    
    // make a vector of brush pixels
    for (int i=0; i<width; i++) {
      for (int j=0; j<height; j++) {
	if (brush.at<cv::Vec3b>(cv::Point(i,j)) == brush_color_vec3b) {
	  pixels.push_back(cv::Point(i,j));
	}
      }
    }
  }
};

/******************************************************************/

// object to determine brush paths to fill in a region of pixels
// note: "region" doesn't require contiguous pixels
class RegionToPaths {
protected:
  int width, height, border;
  PixelTools Tools;

  // brush variables
  Brush *brush;
  std::vector<cv::Point> brush_boundary;
  std::vector<cv::Point> brush_interior;
  std::vector<cv::Point> brush_pixels;

  std::vector<cv::Point> initial_boundary; // boundary of the original regios to be painted
  std::vector<cv::Point> boundary; // boundary of an intermediary region to be painted
  std::vector<cv::Point> one_border_candidate_pixels; // possible places where could put brush for one border pass
  std::vector<cv::Point> candidate_pixels; // possible places where could put brush for all loops
  std::vector<cv::Point> untouchable_boundary_pixels;
  std::vector<std::vector<cv::Point> > strokes; // vector of brush path points to fill in a region
  //cv::Mat brush_mat;

public:
  int *grid;

  // returns 0 if any of the points can't be painted... otherwise it returns the # of ok pixels that
  // are in the desired region to be painted (ABC: change points to *points)
  int imageOnlyHasColor(std::vector<cv::Point> points, int offsetx=0, int offsety=0) {
    cv::Point p;
    int debug=0;
    if (debug) printf(" - points has %i points, offset %i,%i\n",(int)points.size(),offsetx,offsety);
    
    int num_ok_pixels=0;
    cv::Vec3b icolor;
    for (int i=0; i<(int)points.size(); i++) {
      p = points[i];
      int x = p.x+offsetx;
      int y = p.y+offsety;
      //printf("G:(%i %i)\n",x,y); 
      //printf(" - - testing %i,%i\n",x,y);
      if (x>=0 && y>=0 && x<width && y<height) {
	if (grid[x*height+y]==3) { // untouchable
	  //printf(" - - - %i,%i is untouchable\n",x,y);
	  return 0; 
	} else if (grid[x*height+y]==1) { // desired place to paint
	  //if (debug) printf(" - - - %i,%i is ok\n",x,y);
	  num_ok_pixels++;
	}
      } else {
	return 0; // only if you don't want to put brush outside of paper
      }
    }
    if (debug) printf(" - - - all points are the given color\n");
    return num_ok_pixels;
  }

  void defineBrush(Brush *b) {
    brush = b;
    brush_pixels=brush->getPixels();
    Tools.defineBoundary(brush_pixels,brush->getColor(),&brush_boundary,&brush_interior);
  }

  // scans the grid returns the pixels at the border of a given grid value (optionally, returns the interior)
  void defineBoundaryFromGrid(int value=1, std::vector<cv::Point> *boundary = NULL, std::vector<cv::Point> *interior = NULL) {
    for (int i=0; i<width; i++) {
      for (int j=0; j<height; j++) {
	int is_boundary=0;
	if (grid[i*height+j] == value) {
	  // see if pixel is at boundary of region to be painted
	  if (i==0 || j==0 || i==width-1 || j==height-1) {
	    is_boundary=1;
	  } else {
	    // check for neighbors
	    int left=std::max(i-1 , 0);
	    int right=std::min(i+1 , width-1);
	    int top=std::max(j-1 , 0);
	    int bottom=std::min(j+1 ,height-1);
	    
	    for (int n=left; n<=right; n++) {
	      for (int m=top; m<=bottom; m++) {
		if (grid[n*height+m] != value) {
		  is_boundary=1;
		  n=right;
		  m=bottom;
		}
	      }
	    }
	  }	
	  if (is_boundary) {
	    boundary->push_back(cv::Point(i,j));
	  } else if (interior) {
	    interior->push_back(cv::Point(i,j));
	  }
	}
      }
    }
  }

  // draws the grid on a window (typically used for debugging)
  void putGridOnWindow(DrawWindow *W, int offsetx=0, int offsety=0) {
    for (int i=0; i<width; i++) {
      for (int j=0; j<height; j++) {
	//if (grid[i*height+j] == 1) { W->setPenColor(100,100,100); } // grey
	if (grid[i*height+j] == 1) { W->setPenColor(0,0,0); } // black
	if (grid[i*height+j] == 2) { W->setPenColor(150,150,250); } // blue
	if (grid[i*height+j] == 3) { W->setPenColor(250,150,150); } // red
	W->drawPixel(i+offsetx,j+offsety);
      }
    }
  }

  // for debugging
  void drawBoundaries(DrawWindow *W, int offsetx=0, int offsety=0) { 
    // draw desired region boundary
    W->setPenColor(0,0,0);
    for (int i=0; i<(int)initial_boundary.size(); i++) {
      W->drawPixel(initial_boundary[i].x+border , initial_boundary[i].y+border);
    }
    
    // draw desired region boundary pixels that were untouchable
    W->setPenColor(150,0,0);
    for (int i=0; i<(int)untouchable_boundary_pixels.size(); i++) {
      W->drawPixel(untouchable_boundary_pixels[i].x+border , untouchable_boundary_pixels[i].y+border);
    }

    // draw where the brush went
    W->setPenColor(100,250,100);
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      //W->setPenColor(0,50*i%200,i%200);
      W->drawPixel(candidate_pixels[i].x+offsetx, candidate_pixels[i].y+offsety);
    }
  }

  // mostly for debugging... shows the brush strokes in a DrawWindow
  void drawBrushCandidates(DrawWindow *W, int offsetx=0, int offsety=0) { 
    offsety=border;
    offsetx=border;
    cv::Point brush_center = brush->getCenter(); 

    // draws the brush at every candidate pixel 
    W->setPenColor(180,180,180);
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      brush->draw(W, candidate_pixels[i].x+brush_center.x + offsetx, candidate_pixels[i].y+brush_center.y+offsety);
    }
    drawBoundaries(W,offsetx,offsety);
  }

  // mostly for debugging... shows the brush strokes in a DrawWindow
  void drawBrushStrokes(DrawWindow *W, int offsetx=0, int offsety=0) { 
    offsety=border;
    offsetx=border;

    // draws the brush along every stroke
    for (int i=0; i<(int)strokes.size(); i++) {
      for (int j=0; j<(int)strokes[i].size(); j++) {
	brush->draw(W, strokes[i][j].x+offsetx, strokes[i][j].y+offsety);
      }
    }
    drawBoundaries(W,offsetx,offsety);
  }

  // define one path inside the border of pixels for a region
  void definePath(DrawWindow *W = NULL) {
    // reset the lists
    while (!one_border_candidate_pixels.empty()) {
      one_border_candidate_pixels.pop_back();
    }
    while (!boundary.empty()) {
      boundary.pop_back();
    }
    
    // find the pixels at the boundary of the region you want to paint
    defineBoundaryFromGrid(1,&boundary);
    //printf("Region boundary has %i points\n",(int)boundary.size());

    // find the pixels where you can place the brush and not over paint somewhere it shouldn't
    for (int i=0; i<(int)boundary.size(); i++) {
      int x=boundary[i].x;
      int y=boundary[i].y;

      if (i>=0) {
	// see if it's possible to put a pixel at the boundary of the brush at this pixel
	// without having brush pixel touching an image pixel of a different color than the brush
	//printf(" checking painting point %i,%i\n",x,y);

	int ok_point_id=-1;
	int max_ok_pixels=0;

	// test just the brush_boundary as faster than testint all the pixels of the brush...
	// tradeoff is that there is a special case where a brush could fit in a space that
	// is smaller than the brush & has overpaintable pixels on each side & untouchable
	// pixels on each side of the overpaintable pixels... so the brush interior is at
	// the boundary of the desired paint region.  As this is rare, take speed over accuracy

	for (int bb_id=0; bb_id<(int)brush_boundary.size(); bb_id++) {  
	//for (int bb_id=0; bb_id<(int)brush_pixels.size(); bb_id++) {
	  int bb_x = brush_boundary[bb_id].x;
	  int bb_y = brush_boundary[bb_id].y;
	  //printf(" - offset %i,%i\n",bb_x,bb_y);

	  // check the boundary of the brush to see if it's ok... 
	  int num_ok_pixels=imageOnlyHasColor(brush_boundary, x-bb_x, y-bb_y);
	  if (num_ok_pixels>0) { // .. if boundary is ok, then check interior
	    int num_ok_pixels_interior=imageOnlyHasColor(brush_interior, x-bb_x, y-bb_y);
	    if (num_ok_pixels_interior==0) { // if can't paint the interior, then scrap it
	      num_ok_pixels=0;
	    } else {
	      num_ok_pixels += num_ok_pixels_interior;
	    }
	  }

	  if (num_ok_pixels>max_ok_pixels) { // looks for the best place to put a brush
	    ok_point_id=bb_id;
	    max_ok_pixels = num_ok_pixels;
	  }
	}
	if (ok_point_id>=0) {
	  int bb_x = brush_boundary[ok_point_id].x;
	  int bb_y = brush_boundary[ok_point_id].y;
	  one_border_candidate_pixels.push_back(cv::Point(x-bb_x,y-bb_y));
	  candidate_pixels.push_back(cv::Point(x-bb_x,y-bb_y));
	  //printf(" point %i,%i is a candidate if brush at %i %i\n",x,y,bb_x,bb_y);
	  if (W) {
	    W->setPenColor(0,255,0);
	    W->drawPixel(x+border,y+border);
	  }
	} else {
	  if (W) {
	    W->setPenColor(0,0,255);
	    W->drawPixel(x+border,y+border);
	  }
	  // record pixels that can't be touched by the brush w/o painting over untouchable pixels
	  untouchable_boundary_pixels.push_back(boundary[i]);
	}
      } else {
	if (W) {
	  W->setPenColor(0,0,255);
	  W->drawPixel(x+border,y+border);
	}
      }
    }

    //printf("Found %lu this loop candidate_pixels\n", one_border_candidate_pixels.size());
    //printf("Found %lu total candidate_pixels\n", candidate_pixels.size());
    //printf("Found %lu untouchable_boundary_pixels\n", untouchable_boundary_pixels.size());

    if (W) {
      W->setPenColor(0,255,255);
      for (int i=0; i<(int)candidate_pixels.size(); i++) {
	int x=candidate_pixels[i].x + 2; // so easier to see
	int y=candidate_pixels[i].y + 2;
	W->drawPixel(x+border,y+border);
      }
      //printf("drew candidate_pixels\n");
    }
  }

  // recursively define paths to fill up a region
  // NOTE: &W is optional .. if send a DrawWindow, it shows debugging
  void definePaths(DrawWindow *W = NULL) {
    definePath(W);
    initial_boundary = boundary;

    int num_loops=1;
    while (1 && num_loops<20 && one_border_candidate_pixels.size()>0) {
      num_loops++;

      // update the grid and consider everything painted to be ok to paint over again
      for (int i=0; i<(int)candidate_pixels.size(); i++) {
	for (int b=0; b<(int)brush_pixels.size(); b++) {
	  int x=candidate_pixels[i].x + brush_pixels[b].x;
	  int y=candidate_pixels[i].y + brush_pixels[b].y;
	  addOverpaintablePixel(x,y);
	}
      }
      definePath(W);
    }
  }

  // returns a vector of candidate brush location points
  void getCandidateBrushLocations(std::vector<cv::Point> *brush_locations) {
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      brush_locations->push_back(candidate_pixels[i]);
    }
  }

  // simple sort bool
  static bool comparePoints (cv::Point p1,cv::Point p2) { return (p1.x<p2.x); }

  // returns a vector of candidate brush location points
  std::vector<std::vector<cv::Point> > getBrushStrokes() {
    if (strokes.empty()) { defineBrushStrokes(); }
    return strokes;
  }

  // calculates a vector of candidate brush location points
  void defineBrushStrokes() {
    cv::Point brush_center = brush->getCenter(); 

    printf("BBB:%i %i\n",brush_center.x,brush_center.y);

    // make a grid with 1=brush location, 0=otherwise
    int *cgrid = new int [width*height];
    for (int i=0; i<width*height; i++) { grid[i]=0; } 
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      int x=candidate_pixels[i].x;
      int y=candidate_pixels[i].y;
      cgrid[x * height + y] = 1;
    }

    std::sort(candidate_pixels.begin(), candidate_pixels.end(), comparePoints);

    // find all the candidate_pixels that have only 1 neighbor candidate_pixels (e.g., start of line)
    // once all 1 neighbors are done, then do 2+ and get the rest
    int neighbor_count_check = 1; // how many neighbors to look for
    int starts_found=1;
    while (starts_found) {
      starts_found=0;

      for (int i=0; i<(int)candidate_pixels.size(); i++) {
	int x=candidate_pixels[i].x;
	int y=candidate_pixels[i].y;

	if (cgrid[x*height+y]==1) {
	  // count number of candidate_pixels neighbors
	  int num_candidate_neighbors=0;
	  int left=std::max(x-1 , 0);
	  int right=std::min(x+1 , width-1);
	  int top=std::max(y-1 , 0);
	  int bottom=std::min(y+1 ,height-1);
	  for (int n=left; n<=right; n++) {
	    for (int m=top; m<=bottom; m++) {
	      if ((n!=x || m!=y) && cgrid[n*height+m]==1) { num_candidate_neighbors++; }
	    }
	  }

	  if ((neighbor_count_check==1 && num_candidate_neighbors==1) ||
	      (neighbor_count_check==2 && num_candidate_neighbors>1)) {
	    // found start of a line
	    std::vector <cv::Point> stroke;
	    //stroke.push_back(cv::Point(x,y));
	    stroke.push_back(cv::Point(x + brush_center.x , y + brush_center.y));
	    //printf("starting at %i %i\n",x,y);
	    
	    cgrid[x*height+y]=0; // marked it used
	    int done=0;
	    while (!done) {
	      // find the closest neighbor pixel that's a candidate_pixel
	      int left=std::max(x-1 , 0);
	      int right=std::min(x+1 , width-1);
	      int top=std::max(y-1 , 0);
	      int bottom=std::min(y+1 ,height-1);
	      int found_i=-1;
	      int found_j=-1;
	      double max_closeness=-1;
	      for (int n=left; n<=right; n++) {
		for (int m=top; m<=bottom; m++) {
		  if ((n!=x || m!=y) && cgrid[n*height+m]==1) { // neighbor is a candidate
		    int closeness = 0;
		    if (n==x) closeness++;
		    if (m==y) closeness++;
		    if (closeness>max_closeness) {
		      found_i=n;
		      found_j=m;
		      max_closeness = closeness;
		    }
		  }
		}
	      }
	      if (found_i>=0 && found_j>=0) {
		//printf(" - adding %i %i\n",found_i,found_j);
		stroke.push_back(cv::Point(found_i + brush_center.x, found_j + brush_center.y));
		//stroke.push_back(cv::Point(found_i,found_j));
		cgrid[found_i*height+found_j]=0; // marked neighbor used
		x=found_i;
		y=found_j;
	      } else {
		done=1;
	      }
	    }
	    strokes.push_back(stroke);
	    starts_found++;
	  }
	}
      }
      if (starts_found==0 && neighbor_count_check==1) {
	// found all the points with only 1 neighbor, so now get all those with 2+ neighbors
	starts_found=1;
	neighbor_count_check=2;
      }
    }
    
    // at this point, all the points with 1+ neighbors have already been made into paths...
    // so the rest are single points
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      int x=candidate_pixels[i].x;
      int y=candidate_pixels[i].y;

      // check if there's another one later in the list (single points can be duplicated)
      int found=0;
      for (int j=i+1; j<(int)candidate_pixels.size(); j++) {
	if (x==candidate_pixels[j].x && y==candidate_pixels[j].y) { found = 1; }
      }

      if (!found && cgrid[x*height+y]==1) {
	std::vector <cv::Point> stroke;
	stroke.push_back(cv::Point(x,y));
	strokes.push_back(stroke);
      }
    }
  }

  // set which pixel you don't want covered
  void addUntouchablePixel(int i, int j) {
    grid[i*height+j]=3; 
  }

  // set which pixel you could paint if need b/c it will be painted over later
  void addOverpaintablePixel(int i, int j) {
    grid[i*height+j]=2; 
  }

  // set which pixel you want painted
  void addDesiredPixel(int i, int j) {
    grid[i*height+j]=1; 
  }

  void clear() { // clears all variables for reuse
    for (int i=0; i<width*height; i++) { grid[i]=3; } // default all pixels are untouchable
    // maybe skip reseeting these: brush_boundary brush_interior brush_pixels
    initial_boundary.clear();
    boundary.clear();
    one_border_candidate_pixels.clear();
    candidate_pixels.clear();
    untouchable_boundary_pixels.clear();
    for (int i=0; i<(int)strokes.size(); i++) { strokes[i].clear(); }
    strokes.clear();
  }

  RegionToPaths(int w, int h, int b=0) {
    width = w;
    height = h;
    border = b;
    grid = new int [w*h]; // which pixels are ,to be painted(1),paintable(2 e.g., will be painted over a diff color), untouchable(3)
    Tools = PixelTools();
    clear();
  }
};
 
