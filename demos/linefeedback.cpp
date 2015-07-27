#include "webcam.cpp"
#include "drawwindow.cpp"
#include "shapes.cpp"
#include "helpers.cpp"
#include "regiontopaths.cpp"  // brush
#include <unistd.h> // sleep

// use webcam to give feedback on drawing a line between two points

int main(void)
{
  srand (static_cast <unsigned> (time(0)));

  int w=1100;
  int h=600;
  DrawWindow W = DrawWindow(w,h,"Draw Feedback"); 
  Brush brush = Brush(30,20,"ellipse");
  brush.setDrawMode("paint");

  int p1x = 20;
  int p1y = 10;
  int p2x = 1020;
  int p2y = 10;

  W.clearWindow(230,230,230);
  W.setPenColor(0,0,0);
  W.setLineThickness(1);
  W.drawLine(p1x,p1y,p2x,p2y);
  W.speckle(0.04);
  W.blur(5);
  brush.setColor(255,0,0);
  int done=0;
  int loop=0;
  while (!done) {
    loop++;
    brush.loadPaintPixel(50);
    brush.drawLine(&W,p1x,p1y,p2x,p2y);
    
    std::vector<cv::Point> errors;
    double score=brush.scorePaintLine(&W,p1x,p1y,p2x,p2y,&errors);
    printf("Paint score: %.2f\n",100*score);
    if (score<0.99) {
      p1x=errors[0].x;
      p1y+=30;
      p2y+=30;
    }
    if (score>.99 || loop>12) { done=1; }
  }

  W.show();

  if (0) { // test a line to see if it matches a desired color
    PixelTools Tool;
    std::vector<cv::Point> errors;
    double qual = Tool.testLineQualityUNUSED(&W.grid,cv::Point(p1x,p1y),cv::Point(p2x,p2y),cv::Scalar(0,0,255),.3,&errors);
    printf("Line score = %.2f %%\n",100*qual);
    W.setPenColor(0,0,255);
    for (int i=0; i<(int)errors.size(); i++) {
      //printf("Bad pixels: %i,%i\n",errors[i].x,errors[i].y);
      //W.drawPixel(errors[i]);
    }
    W.show();
  }
  
  while (1) {   
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop
  }

}
