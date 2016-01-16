#include "regiontopaths.cpp"
#include <ctime> // for random number generator

#include <GLUT/glut.h>

// demo to create paths for a brush in a region

void defineDemoRegion(RegionToPaths *rtp, int w, int h) {
  time_t now = time(0);
  cv::RNG rng(now); // random number generator object
  //cv::RNG rng(4); // random number generator object

  // mark pixels that will be painted over by another color later
  // that is... pixels where the brush could paint even tho it's not desired
  if (1) {
    for (int i = 0; i < 10; i++) {
      int x = rng.uniform(0., .8*w);
      int y = rng.uniform(0., .8*h);
      int rw = rng.uniform(10., 10 + .5*w);
      int rh = rng.uniform(10., 10 + .5*h);
      for (int n = x; n <= x + rw; n++) {
	for (int m = y; m <= y + rh; m++) {
	  if (n < w && m < h) {
	    rtp->addOverpaintablePixel(n, m);
	  }
	}
      }
    }
  }

  // mark pixels to be painted (e.g., the region)
  for (int i = 0; i < 10; i++) {
    int x = rng.uniform(0., .8*w);
    int y = rng.uniform(0., .8*h);
    int rw = rng.uniform(10., 10 + .5*w);
    int rh = rng.uniform(10., 10 + .5*h);
    for (int n = x; n <= x + rw; n++) {
      for (int m = y; m <= y + rh; m++) {
	if (n < w && m < h) {
	  rtp->addDesiredPixel(n, m);
	}
      }
    }
  }
}

int main(int argc, char** argv) {
  int w = 400;
  int h = 300;
  int border = 30; // extend paintable region outside the canvas so can draw over the edges

  //if (argv[1]) { w = atoi(argv[1]); }
  //if (argv[2]) { h = atoi(argv[2]); }
  //if (argv[3]) { int seed = atoi(argv[3]); rng(seed); }

  DrawWindow W = DrawWindow(w + 2 * border, h + 2 * border, "Region"); // w,h
  W.moveWindow(0, 50);

  DrawWindow BP = DrawWindow(w + 2 * border, h + 2 * border, "Brush Paths"); // w,h
  BP.moveWindow(0, 50 + h + 2 * border + 50);

  RegionToPaths RTP = RegionToPaths(w, h, border);

  int done = 0;
  while (!done) {

    RTP.clear();
    defineDemoRegion(&RTP, w, h);

    W.clearWindow(230, 230, 230); // default background is white
    W.setPenColor(0, 0, 0);
    W.setLineThickness(1);
    W.drawRectangle(border - 4, border - 4, border + w + 4, border + h + 4, 0); // draw outline of canvas
    RTP.putGridOnWindow(&W, border, border);
    W.show();

    BP.clearWindow(230, 230, 230); // default background is white
    BP.setPenColor(0, 0, 0);
    BP.setLineThickness(1);
    BP.drawRectangle(border - 4, border - 4, border + w + 4, border + h + 4, 0); // draw outline of canvas
    BP.show();

    //Brush brush = Brush(30,20,"rectangle");
    Brush brush = Brush(20, 30, "ellipse");
    brush.setColor(20, 20, 40);
    RTP.defineBrush(&brush);

    RTP.definePaths(&W); // NOTE: &W is optional .. if send a DrawWindow, it shows debugging
    W.show();

    // get the list of all the brush points (unstructured)
    std::vector<cv::Point> brush_locations;
    RTP.getCandidateBrushLocations(&brush_locations);
    for (int i = 0; i < (int)brush_locations.size(); i++) {
      //printf("BRUSH %i: (%i,%i)\n",i,brush_locations[i].x,brush_locations[i].y);
    }

    // get a vector of brush stroke points & draw blue lines on the path
    printf("Get the brush strokes\n");
    std::vector<std::vector<cv::Point> > brush_strokes = RTP.getBrushStrokes();

    // draw the brush at every candidate_pixels point
    //RTP.drawBrushCandidates(&BP);
    RTP.drawBrushStrokes(&BP);
    BP.show();

    BP.setPenColor(0, 0, 250);
    for (int i = 0; i < (int)brush_strokes.size(); i++) {
      printf("Stroke %i/%i (%i points) ", i, (int)brush_strokes.size(), (int)brush_strokes[i].size());
      for (int p = 0; p < (int)brush_strokes[i].size(); p++) {
	int x = brush_strokes[i][p].x;
	int y = brush_strokes[i][p].y;
	printf("(%i,%i) ", x, y);
	BP.drawPixel(x + border, y + border);
      }
      printf("\n");
    }
    BP.show();

    DrawWindow P = DrawWindow(w + 2 * border, h + 2 * border, "Brush Painting"); // w,h
    P.moveWindow(w + 2 * border, 50);
    P.clearWindow(230, 230, 230); // default background is white
    P.speckle(0.04);
    P.blur(5);
    P.setPenColor(0, 0, 0);
    P.setLineThickness(1);
    P.drawRectangle(border - 4, border - 4, border + w + 4, border + h + 4, 0); // draw outline of canvas
    P.show();

    brush.setColor(0, 0, 250);
    brush.setDrawOffset(border, border);
    brush.setDrawMode("paint");

    PixelTools Tool;
    for (int i = 0; i < (int)brush_strokes.size(); i++) {
      std::vector<cv::Point> stroke = brush_strokes[i];
      printf("Painting stroke %i\n", i);
      int done = 0;
      int loop = 0;
      while (!done) {
	loop++;
	brush.loadPaintPixel(50);
	brush.drawContiguousPoints(&P, &stroke);

	// draw a yellow rectangle at the beginning of the stroke
	cv::Point pt = stroke[0];
	P.setPenColor(255, 255, 0);
	P.drawRectangle(pt.x - 1 + border, pt.y - 1 + border, pt.x + 1 + border, pt.y + 1 + border, 1);

	std::vector<cv::Point> errors;
	double score = brush.scorePaintPoints(&P, &stroke, &errors, .1, .1);
	printf(" Loop %i - %i points, paint score: %.2f\n", loop, (int)stroke.size(), 100 * score);
	if (score < 0.99) { stroke = errors; }
	if (score > 0.99 || loop>12) { done = 1; }
	P.show();
	int wait = 1;
	while (wait) {
	  int k = cv::waitKey(33);
	  if (k > 0) { wait = 0; }
	}
      }
    }
    brush.setDrawOffset(0, 0);
    P.show();
    printf("Done painting\n");

    int wait = 1;
    while (wait) {
      int k = cv::waitKey(33);
      //if (k>0) { printf("key = %d\n",k); }
      if (k == int('n')) { wait = 0; } // do another
      if (k == 27) { wait = 0; done = 1; }  // Esc key to stop
    }
  }
}
