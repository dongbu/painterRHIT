#include "drawwindow.cpp"
  // 810 taylor n


// demo to create paths for a brush in a region

cv::Vec3b scalarToVec3b (cv::Scalar s) { 
  cv::Vec3b vec;
  vec[0] = s[0];
  vec[1] = s[1];
  vec[2] = s[2];
  return vec;
}

// returns 1 if all of the pixels at points are of color color (ABC: change points to *points)
int imageOnlyHasColor(cv::Mat *image, std::vector<cv::Point> points, cv::Vec3b color, int offsetx=0, int offsety=0) {
  cv::Point p;
  int debug=0;
  if (debug) printf(" - points has %i points, offset %i,%i\n",(int)points.size(),offsetx,offsety);

  cv::Vec3b icolor;
  for (int i=0; i<(int)points.size(); i++) {
    p = points[i];
    int x = p.x+offsetx;
    int y = p.y+offsety;
    //printf(" - - testing %i,%i\n",x,y);
    if (x>=0 && y>=0 && x<image->cols && y<image->rows) {
      icolor = image->at<cv::Vec3b>(cv::Point(x,y));
      if (icolor[0]!=color[0] || icolor[1]!=color[1] || icolor[2]!=color[2]) {
	if (debug) printf(" - - - %i,%i doesn't match color %i %i, %i %i, %i %i\n",x,y,icolor[0],color[0],icolor[1],color[1],icolor[2],color[2]);
	return 0; 
      } else {
	if (debug) printf(" - - - %i,%i matches color %i %i, %i %i, %i %i\n",x,y,icolor[0],color[0],icolor[1],color[1],icolor[2],color[2]);
      }
    }
  }
  if (debug) printf(" - - - all points are the given color\n");
  return 1;
}


// returns 1 if all of the pixels at points are of color color (ABC: change points to *points)
int imageOnlyHasColor(int *grid, int w, int h, std::vector<cv::Point> points, int offsetx=0, int offsety=0) {
  cv::Point p;
  int debug=0;
  if (debug) printf(" - points has %i points, offset %i,%i\n",(int)points.size(),offsetx,offsety);

  cv::Vec3b icolor;
  for (int i=0; i<(int)points.size(); i++) {
    p = points[i];
    int x = p.x+offsetx;
    int y = p.y+offsety;
    //printf("G:(%i %i)\n",x,y); 

    //printf(" - - testing %i,%i\n",x,y);
    if (x>=0 && y>=0 && x<w && y<h) {
      //if (grid[x*h+y] !=1 ) { printf(" => %i  W:%i H:%i\n",grid[x*h+y],w,h); }
      if (grid[x*h+y]==3) { // untouchable
	//printf(" - - - %i,%i is untouchable\n",x,y);
	return 0; 
      } else {
	//if (debug) printf(" - - - %i,%i is ok\n",x,y);
      }
    } else {
      return 0; // only if you don't want to put brush outside of paper
    }
  }
  if (debug) printf(" - - - all points are the given color\n");
  return 1;
}

// returns 1 if any of the pixels at points are of color color (ABC: change points to *points)
int imageHasColor(cv::Mat *image, std::vector<cv::Point> points, cv::Vec3b color, int offsetx=0, int offsety=0) {
  cv::Point p;
  for (int i=0; i<(int)points.size(); i++) {
    p = points[i];
    int x = p.x+offsetx;
    int y = p.y+offsety;
    if (x>=0 && y>=0 && x<image->cols && y<image->rows && image->at<cv::Vec3b>(cv::Point(x,y)) == color) return 1; 
  }
  return 0;
}

// returns 1 if none of the pixels at points are of color color (ABC: change points to *points)
int imageHasNoColor(cv::Mat *image, std::vector<cv::Point> points, cv::Vec3b color, int offsetx=0, int offsety=0) {
  cv::Point p;
  for (int i=0; i<(int)points.size(); i++) {
    p = points[i];
    int x = p.x+offsetx;
    int y = p.y+offsety;
    if (x>=0 && y>=0 && x<image->cols && y<image->rows && image->at<cv::Vec3b>(cv::Point(x,y)) == color) return 0; 
  }
  return 1;
}

void defineBoundary(cv::Mat *image, cv::Scalar region_color, std::vector<cv::Point> *boundary) {
  int w=image->cols;
  int h=image->rows;
  // determine the boundary pixels
  for (int i=0; i<w; i++) {
    for (int j=0; j<h; j++) {
      cv::Scalar color = image->at<cv::Vec3b>(cv::Point(i,j));
      
      if (color == region_color) {
	// see if pixel is at boundary of region to be painted
	if (i==0 || j==0 || i==w-1 || j==h-1) {
	  boundary->push_back(cv::Point(i,j));
	} else {
	  // check for neighbors
	  int left=std::max(i-1 , 0);
	  int right=std::min(i+1 , w-1);
	  int top=std::max(j-1 , 0);
	  int bottom=std::min(j+1 ,h-1);
	  //int pixels=(1+right-left)*(1+bottom-top) - 1; // how many neighbor pixels to compare
	  
	  for (int n=left; n<=right; n++) {
	    for (int m=top; m<=bottom; m++) {
	      cv::Scalar neighbor_color = image->at<cv::Vec3b>(cv::Point(n,m));
	      if (neighbor_color[0] != region_color[0] ||
		  neighbor_color[1] != region_color[1] ||
		  neighbor_color[2] != region_color[2] ) {
		boundary->push_back(cv::Point(i,j));
		n=right;
		m=bottom;
	      }
	    }
	  }
	}
      }
    }
  }
}


//int main(void)
int main( int argc, char** argv )
{
  int seed=345;
  if (argv[1]) { seed = atoi(argv[1]); }
  cv::RNG rng(seed); // random number generator object

  int w=300;
  int h=200;
  int border=50; // extend paintable region outside the canvas so can draw over the edges

  int* grid = new int [w*h]; // which pixels are ,to be painted(1),paintable(2 e.g., will be painted over a diff color), untouchable(3)
  for (int i=0; i<w*h; i++) { grid[i]=3; }
  
  DrawWindow W = DrawWindow(w+2*border,h+2*border,"Region"); // w,h
  W.moveWindow(30,50);

  W.setPenColor(100,0,0);  // show all pixels to be untouchable
  W.drawRectangle(border,border,border+w,border+h,1); // default 

  //  W.drawPixel(n,m);

  // mark pixels that will be painted over by another color later
  if (0) {
    W.setPenColor(0,0,100);
    for (int i=0; i<10; i++) {
      int x= rng.uniform(0.,.8*w);
      int y = rng.uniform(0.,.8*h);
      int rw = rng.uniform(10.,10+.5*w);
      int rh = rng.uniform(10.,10+.5*h);
      for (int n=x; n<=x+rw; n++) {
	for (int m=y; m<=y+rh; m++) {
	  if (n<w && m<h) {
	    grid[n*h+m]=2; // will be painted later
	    W.drawPixel(n+border,m+border);
	  }
	} 
      }
    }
  }

  // mark pixels to be painted (e.g., the region)
  W.setPenColor(0,0,0);
  for (int i=0; i<10; i++) {
    int x= rng.uniform(0.,.8*w);
    int y = rng.uniform(0.,.8*h);
    int rw = rng.uniform(10.,10+.5*w);
    int rh = rng.uniform(10.,10+.5*h);
    for (int n=x; n<=x+rw; n++) {
      for (int m=y; m<=y+rh; m++) {
	if (n<w && m<h) {
	  grid[n*h+m]=1; // wanted to be painted
	  W.drawPixel(n+border,m+border);
	}
      } 
    }
  }

  W.show();

  cv::Vec3b set_color;

  // create brush mask
  int brush_w = 10; // max dimensions of brush
  int brush_h = 20;
  //int *brush_mask new int [brush_w*brush_h]; // which pixels are mask
  //for (int i=0; i<brush_w*brush_h; i++) { brush_mask[i]=0; }
  
  cv::Scalar brush_color = cv::Scalar(0,0,0); 
  cv::Scalar not_brush_color = cv::Scalar(255,255,255); 
  //  cv::Mat brush(brush_w,brush_h, CV_8UC3, not_brush_color);
  cv::Mat brush = cv::Mat::zeros(cv::Size(brush_w,brush_h), CV_8UC3);
  brush.setTo(not_brush_color);

  std::vector<cv::Point> brush_pixels;
  set_color = scalarToVec3b(brush_color);

  // draw brush (note: this is so we can use arbitrary shaped brushes
  for (int i=0; i<brush_w; i++) {
    for (int j=0; j<brush_h; j++) {
      brush.at<cv::Vec3b>(cv::Point(i,j)) = set_color;
    } 
  }

  cv::namedWindow( "brush", CV_WINDOW_AUTOSIZE );
  cv::imshow( "brush", brush );

  for (int i=0; i<brush_w; i++) {
    for (int j=0; j<brush_h; j++) {
      if (brush.at<cv::Vec3b>(cv::Point(i,j)) == set_color) {
	brush_pixels.push_back(cv::Point(i,j));
      }
    }
  }

  std::vector<cv::Point> brush_boundary;
  defineBoundary(&brush,brush_color,&brush_boundary);
  printf("brush boundary has %i points\n",(int)brush_boundary.size());

  std::vector<cv::Point> boundary;
  cv::Mat *painting = &W.grid;
  defineBoundary(painting,brush_color,&boundary);
  // offset the region
  for (int i=0; i<(int)boundary.size(); i++) {
    boundary[i].x-=border;
    boundary[i].y-=border;
  }
  printf("painting boundary has %i points\n",(int)boundary.size());
  
  set_color = scalarToVec3b(cv::Scalar(0,0,255));
  set_color[0]=0;
  set_color[1]=0;
  set_color[2]=255;

  std::vector<cv::Point> candidate_pixels;

  //cv::Vec3b candidate_color = scalarToVec3b(cv::Scalar(0,255,0));
  for (int i=0; i<(int)boundary.size(); i++) {
    int x=boundary[i].x;
    int y=boundary[i].y;

    if (i>=0) {
      // see if it's possible to put a pixel at the boundary of the brush at this pixel
      // without having brush pixel touching an image pixel of a different color than the brush
      //printf(" checking painting point %i,%i\n",x,y);

      int ok_point_id=-1;
      for (int bb_id=0; bb_id<(int)brush_boundary.size(); bb_id++) {
	int bb_x = brush_boundary[bb_id].x;
	int bb_y = brush_boundary[bb_id].y;
	//printf(" - offset %i,%i\n",bb_x,bb_y);
	// ABC: should be brush_pixels not brush_boundary (or test boundary first)
	if (imageOnlyHasColor(grid, w, h, brush_boundary, x-bb_x, y-bb_y)) {
	  ok_point_id=bb_id;
	}
      }
      if (ok_point_id>=0) {
	int bb_x = brush_boundary[ok_point_id].x;
	int bb_y = brush_boundary[ok_point_id].y;
	candidate_pixels.push_back(cv::Point(x-bb_x,y-bb_y));
	//printf(" point %i,%i is a candidate if brush at %i %i\n",x,y,bb_x,bb_y);
	W.setPenColor(0,255,0);
	W.drawPixel(x+border,y+border);
      } else {
	W.setPenColor(0,0,255);
	W.drawPixel(x+border,y+border);
      }
    } else {
      W.setPenColor(0,0,255);
      W.drawPixel(x+border,y+border);
    }
  }

  printf("Found %lu candidate_pixels\n", candidate_pixels.size());

  W.setPenColor(0,255,255);
  for (int i=0; i<(int)candidate_pixels.size(); i++) {
    //int x=std::max(0,std::min(candidate_pixels[i].x + 3,w)); // so easier to see
    //int y=std::max(0,std::min(candidate_pixels[i].y + 3,h));
    int x=candidate_pixels[i].x + 3; // so easier to see
    int y=candidate_pixels[i].y + 3;
    //printf("ok %i (%i,%i)\n",i,x,y);
    W.drawPixel(x+border,y+border);
  }
  printf("drew candidate_pixels\n");

  W.show();

  // draw the brush at every candidate_pixels point
  DrawWindow F = DrawWindow(w+2*border,h+2*border,"FF"); // w,h
  F.moveWindow(30+w+2*border+10,50);
  F.clearWindow(230,230,230); // default background is white
  F.setPenColor(180,180,180);
  for (int i=0; i<(int)candidate_pixels.size(); i++) {
    for (int b=0; b<=(int)brush_pixels.size(); b++) {
      int x=candidate_pixels[i].x + brush_pixels[b].x;
      int y=candidate_pixels[i].y + brush_pixels[b].y;
      F.drawPixel(x+border, y+border);
    }
  }

  // draw desired region boundary
  F.setPenColor(0,0,0);
  for (int i=0; i<(int)boundary.size(); i++) {
    F.drawPixel(boundary[i].x+border , boundary[i].y+border);
  }

  
  F.show();

  if (0) {
    DrawWindow FW = DrawWindow(w+border,h+border,"Painting"); // w,h
    //FW.moveWindow(30+w+10,50);
    FW.clearWindow(230,230,230); // default background is white
    FW.setPenColor(0,0,0);
    for (int i=0; i<(int)candidate_pixels.size(); i++) {
      //FW.drawRegion(brush_pixels);
    }
    FW.show();
  }

  while (1) {
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k>0 || k==27) { return(0); }  // Esc key to stop
  }
}
