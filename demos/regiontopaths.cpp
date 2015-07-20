#pragma once

#include "drawwindow.cpp"

// Tools to take a set of desired points to paint and returns paths for a brush
// see regiontopaths_demo.cpp for example use

class RegionToPaths {
protected:
  int width, height, border;

  // brush variables
  int brush_w, brush_h;
  std::vector<cv::Point> brush_boundary;
  std::vector<cv::Point> brush_interior;
  std::vector<cv::Point> initial_boundary; // boundary of the regions to be painted
  std::vector<cv::Point> boundary; // boundary of the regions to be painted
  std::vector<cv::Point> one_border_candidate_pixels; // possible places where could put brush for one border pass
  std::vector<cv::Point> candidate_pixels; // possible places where could put brush for all loops
  std::vector<cv::Point> untouchable_boundary_pixels;
  std::vector<cv::Point> brush_pixels;
  cv::Mat brush;

public:
  int *grid;

  cv::Vec3b scalarToVec3b (cv::Scalar s) { 
    cv::Vec3b vec;
    vec[0] = s[0];
    vec[1] = s[1];
    vec[2] = s[2];
    return vec;
  }

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

  // scans an image and returns the pixels at the border of a colored region (optionally, returns the interior)
  void defineBoundary(cv::Mat *image, cv::Scalar region_color,
		      std::vector<cv::Point> *boundary, std::vector<cv::Point> *interior = NULL,
		      int offsetx=0, int offsety=0) {
    int w=image->cols;
    int h=image->rows;
    // determine the boundary pixels
    for (int i=0; i<w; i++) {
      for (int j=0; j<h; j++) {
	cv::Scalar color = image->at<cv::Vec3b>(cv::Point(i,j)); // hxxx
	
	int is_boundary=0;
	if (color == region_color) {
	  // see if pixel is at boundary of region to be painted
	  if (i==0 || j==0 || i==width-1 || j==height-1) {
	    is_boundary=1;
	  } else {
	    // check for neighbors
	    int left=std::max(i-1 , 0);
	    int right=std::min(i+1 , width-1);
	    int top=std::max(j-1 , 0);
	    int bottom=std::min(j+1 ,height-1);
	    //int pixels=(1+right-left)*(1+bottom-top) - 1; // how many neighbor pixels to compare
	      
	    for (int n=left; n<=right; n++) {
	      for (int m=top; m<=bottom; m++) {
		cv::Scalar neighbor_color = image->at<cv::Vec3b>(cv::Point(n,m));
		if (neighbor_color[0] != region_color[0] ||
		    neighbor_color[1] != region_color[1] ||
		    neighbor_color[2] != region_color[2] ) {
		  is_boundary=1;
		  //boundary->push_back(cv::Point(i,j));
		  n=right;
		  m=bottom;
		}
	      }
	    }
	  }	
	  if (is_boundary) {
	    boundary->push_back(cv::Point(i+offsetx,j+offsety));
	  } else if (interior) {
	    interior->push_back(cv::Point(i+offsetx,j+offsety));
	  }
	}
      }
    }
  }
  
  // given a region of pixels, returns the pixels at the border of a colored region (optionally, returns the interior)
  void defineBoundary(std::vector<cv::Point> region, cv::Scalar region_color, 
		      std::vector<cv::Point> *boundary, std::vector<cv::Point> *interior = NULL) {
    int minx=region[0].x;
    int miny=region[0].y;
    int maxx=0;
    int maxy=0;
    for (int i=0; i<(int)region.size(); i++) {
      minx = std::min(minx,region[i].x);
      miny = std::min(minx,region[i].y);
      maxx = std::max(maxx,region[i].x);
      maxy = std::max(maxx,region[i].y);
    }
    cv::Mat image = cv::Mat::zeros(cv::Size(maxx-minx,maxy-miny), CV_8UC3);
    //cv::Mat image = cv::Mat( maxy-miny, maxx-minx, CV_8UC3 ); // 3 channel color
      
    if (region_color[0]==0 && region_color[0]==0 && region_color[0]==0) {
      image.setTo(cv::Scalar(1,1,1));
    } else {
      image.setTo(cv::Scalar(0,0,0));
    }
      
    cv::Vec3b region_color_vec = scalarToVec3b(region_color);
    for (int i=0; i<(int)region.size(); i++) {
      image.at<cv::Vec3b>(region[i]) = region_color_vec;
    }
    defineBoundary(&image,region_color,boundary,interior,minx,miny);
  }

  void defineBrush(int w, int h, std::string type="rectangle", int show_brush = 0) {
    // create brush mask
    brush_w = w; // max dimensions of brush
    brush_h = h;
  
    //ABC: simplify brush to an int mask as dosnt' need to be color'd
    cv::Scalar brush_color = cv::Scalar(0,0,0); 
    cv::Scalar not_brush_color = cv::Scalar(255,255,255); 
    brush = cv::Mat::zeros(cv::Size(brush_w,brush_h), CV_8UC3);
    brush.setTo(not_brush_color);

    cv::Vec3b brush_color_vec3b = scalarToVec3b(brush_color);

    // draw brush (note: this is so we can use arbitrary shaped brushes
    if (type.compare("ellipse")==0) {
      cv::ellipse( brush, cv::Point(std::floor(w/2),std::floor(h/2)), 
		   cv::Size(std::floor(brush_w/2), std::floor(brush_h/2)), 0., 0., 360., brush_color, -1);
    } else {
      cv::rectangle( brush, cv::Point(0,0), cv::Point(brush_w,brush_h), brush_color, -1);
    }
    if (show_brush) {
      cv::namedWindow( "Brush", CV_WINDOW_AUTOSIZE );
      cv::imshow( "Brush", brush );
    }

    // make a vector of brush pixels
    for (int i=0; i<brush_w; i++) {
      for (int j=0; j<brush_h; j++) {
	if (brush.at<cv::Vec3b>(cv::Point(i,j)) == brush_color_vec3b) {
	  brush_pixels.push_back(cv::Point(i,j));
	}
      }
    }

    // find the pixels of the boundary of the brush
    defineBoundary(&brush,brush_color,&brush_boundary,&brush_interior);
    //printf("brush boundary has %i points\n",(int)brush_boundary.size());
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

  // draws the grid on a window
  void putGridOnWindow(DrawWindow *W, int offsetx=0, int offsety=0) {
    for (int i=0; i<width; i++) {
      for (int j=0; j<height; j++) {
	//if (grid[i*height+j] == 1) { W->setPenColor(100,100,100); } // grey
	if (grid[i*height+j] == 1) { W->setPenColor(0,0,0); } // grey
	if (grid[i*height+j] == 2) { W->setPenColor(150,150,250); } // blue
	if (grid[i*height+j] == 3) { W->setPenColor(250,150,150); } // red
	W->drawPixel(i+offsetx,j+offsety);
      }
    }
  }

  // mostly for debugging... shows the brush strokes in a DrawWindow
  void drawBrushStrokes(DrawWindow *W, int offsetx=0, int offsety=0) { 
    offsety=border;
    offsetx=border;

    // draws the brush at every candidate pixel 
    W->setPenColor(180,180,180);
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      for (int b=0; b<=(int)brush_pixels.size(); b++) {
	int x=candidate_pixels[i].x + brush_pixels[b].x;
	int y=candidate_pixels[i].y + brush_pixels[b].y;
	W->drawPixel(x+offsetx, y+offsety);
      }
    }

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

  // returns a vector of candidate brush location points
  void getCandidateBrushStrokes(std::vector<std::vector<cv::Point> >*strokes) {
    // make a grid with 1=brush location, 0=otherwise
    int *cgrid = new int [width*height];
    for (int i=0; i<width*height; i++) { grid[i]=0; } 
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      int x=candidate_pixels[i].x;
      int y=candidate_pixels[i].y;
      cgrid[x * height + y] = 1;
    }

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
	    stroke.push_back(cv::Point(x,y));
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
		stroke.push_back(cv::Point(found_i,found_j));
		cgrid[found_i*height+found_j]=0; // marked neighbor used
		x=found_i;
		y=found_j;
	      } else {
		done=1;
	      }
	    }
	    strokes->push_back(stroke);
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
	strokes->push_back(stroke);
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

  RegionToPaths(int w, int h, int b=0) {
    width = w;
    height = h;
    border = b;
    grid = new int [w*h]; // which pixels are ,to be painted(1),paintable(2 e.g., will be painted over a diff color), untouchable(3)
    grid[201]=4;
    for (int i=0; i<w*h; i++) { grid[i]=3; } // default all pixels are untouchable
  }
};
 
