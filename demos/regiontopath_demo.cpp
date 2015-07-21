#include "regiontopaths.cpp"

#include <ctime> // for random number generator

// demo to create paths for a brush in a region

int main( int argc, char** argv )
  {
    int w=400;
    int h=300;
    int border=30; // extend paintable region outside the canvas so can draw over the edges
    time_t now = time(0);
    cv::RNG rng(now); // random number generator object

    //if (argv[1]) { w = atoi(argv[1]); }
    //if (argv[2]) { h = atoi(argv[2]); }
    //if (argv[3]) { int seed = atoi(argv[3]); rng(seed); }

    RegionToPaths RTP = RegionToPaths(w,h,border);

    // mark pixels that will be painted over by another color later
    // that is... pixels where the brush could paint even tho it's not desired

    if (1) {
      for (int i=0; i<10; i++) {
	int x= rng.uniform(0.,.8*w);
	int y = rng.uniform(0.,.8*h);
	int rw = rng.uniform(10.,10+.5*w);
	int rh = rng.uniform(10.,10+.5*h);
	for (int n=x; n<=x+rw; n++) {
	  for (int m=y; m<=y+rh; m++) {
	    if (n<w && m<h) {
	      RTP.addOverpaintablePixel(n,m);
	    }
	  } 
	}
      }
    }

    // mark pixels to be painted (e.g., the region)
    for (int i=0; i<10; i++) {
      int x= rng.uniform(0.,.8*w);
      int y = rng.uniform(0.,.8*h);
      int rw = rng.uniform(10.,10+.5*w);
      int rh = rng.uniform(10.,10+.5*h);
      for (int n=x; n<=x+rw; n++) {
	for (int m=y; m<=y+rh; m++) {
	  if (n<w && m<h) {
	    RTP.addDesiredPixel(n,m);
	  }
	} 
      }
    }

    DrawWindow W = DrawWindow(w+2*border,h+2*border,"Region"); // w,h
    W.moveWindow(0,50);
    W.setPenColor(0,0,0);
    W.setLineThickness(1);
    W.drawRectangle(border-4,border-4,border+w+4,border+h+4,0); // draw outline of canvas
    RTP.putGridOnWindow(&W,border,border);
    W.show();

    //while (1) {};

    //Brush brush(30,20,"rectangle");
    //RTP.defineBrush(30,20,"rectangle");
    RTP.defineBrush(30,20,"ellipse");
    //RTP.defineBrush(&brush);

    RTP.definePaths(&W); // NOTE: &W is optional .. if send a DrawWindow, it shows debugging
    W.show();

    // draw the brush at every candidate_pixels point
    DrawWindow BP = DrawWindow(w+2*border,h+2*border,"Brush Paths"); // w,h
    BP.moveWindow(0,50+h+2*border+50);
    BP.clearWindow(230,230,230); // default background is white
    BP.setPenColor(0,0,0);
    BP.setLineThickness(1);
    BP.drawRectangle(border-4,border-4,border+w+4,border+h+4,0); // draw outline of canvas
    RTP.drawBrushStrokes(&BP);
    BP.show();

    // get the list of all the brush points (unstructured)
    std::vector<cv::Point> brush_locations;
    RTP.getCandidateBrushLocations(&brush_locations);
    for (int i=0; i<(int)brush_locations.size(); i++) {
      //printf("BRUSH %i: (%i,%i)\n",i,brush_locations[i].x,brush_locations[i].y);
    }

    // get a vector of brush stroke points
    printf("Get the brush strokes\n");
    std::vector<std::vector<cv::Point> > brush_strokes;
    RTP.getCandidateBrushStrokes(&brush_strokes);
    BP.setPenColor(0,0,250);
    for (int i=0; i<(int)brush_strokes.size(); i++) {
      printf("Stroke %i (%i points) ",i,(int)brush_strokes[i].size());
      for (int p=0; p<(int)brush_strokes[i].size(); p++) {
	int x=brush_strokes[i][p].x;
	int y=brush_strokes[i][p].y;
	printf("(%i,%i) ",x,y);
	BP.drawPixel(x+border,y+border);
      }
      printf("\n");
    }
    BP.show();

    while (1) {
      int k = cv::waitKey(33);
      //if (k>0) { printf("key = %d\n",k); }
      if (k>0 || k==27) { return(0); }  // Esc key to stop
    }
  }
