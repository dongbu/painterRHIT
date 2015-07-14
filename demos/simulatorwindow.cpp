#include "drawwindow.cpp"

int main(void)
{
  cv::RNG rng(12345); // random number generator object

  DrawWindow W = DrawWindow(300,500,"Simulator Window"); // w,h
  W.clearWindow(230,230,230); // default background is white
  W.setLineThickness(3);
  W.drawLine(10,10,50,100);
  W.setPenColor(200,0,0);
  W.drawLine(cv::Point(20,20),cv::Point(60,110));

  W.setLineThickness(1);
  W.setPenColor(0,200,0);
  W.drawRectangle(200,5,230,100);
  W.setPenColor(0,200,200);
  W.drawRectangle(260,5,290,100,1);

  W.setPenColor(0,0,0);
  W.drawCircle(215,120,10);
  W.drawCircle(275,120,10,1);

  W.setPenColor(200,200,0);
  W.drawEllipse(215,220,10,60,0,0);
  W.setPenColor(200,100,0);
  W.drawEllipse(275,220,10,60,45,1);

  W.setPenColor(0,0,0);
  W.drawText(50,200,"hello");

  for (int i=0; i<100; i++) {
    W.setPenColor(rng.uniform(10,200),rng.uniform(10,200),rng.uniform(10,200));
    int x=rng.uniform(0,300);
    int y=rng.uniform(300,350);
    W.drawPixel(x,y);
  }

  // example of using vector of points to draw pixels
  std::vector<cv::Point> pixels;
  for (int i=0; i<100; i++) {
    int x=rng.uniform(0,300);
    int y=rng.uniform(400,450);
    pixels.push_back(cv::Point(x,y));
  }
  W.setPenColor(0,0,255);
  W.drawRegion(pixels);

  W.startPolyPoints();
  int w=100;
  int delx=20;
  int dely=200;
  W.addPolyPoint( w/4.0 + delx, 7*w/8.0 + dely );
  W.addPolyPoint( 3*w/4.0 + delx, 7*w/8.0 + dely );
  W.addPolyPoint( 3*w/4.0 + delx, 13*w/16.0 + dely );
  W.addPolyPoint( 11*w/16.0 + delx, 13*w/16.0 + dely );
  W.addPolyPoint( 19*w/32.0 + delx, 3*w/8.0 + dely );
  W.addPolyPoint( 3*w/4.0 + delx, 3*w/8.0 + dely );
  W.addPolyPoint( 3*w/4.0 + delx, w/8.0 + dely );
  W.addPolyPoint( 26*w/40.0 + delx, w/8.0 + dely );
  W.addPolyPoint( 26*w/40.0 + delx, w/4.0 + dely );
  W.addPolyPoint( 22*w/40.0 + delx, w/4.0 + dely );
  W.addPolyPoint( 22*w/40.0 + delx, w/8.0 + dely );
  W.addPolyPoint( 18*w/40.0 + delx, w/8.0 + dely );
  W.addPolyPoint( 18*w/40.0 + delx, w/4.0 + dely );
  W.addPolyPoint( 14*w/40.0 + delx, w/4.0 + dely );
  W.addPolyPoint( 14*w/40.0 + delx, w/8.0 + dely );
  W.addPolyPoint( w/4.0 + delx, w/8.0 + dely );
  W.addPolyPoint( w/4.0 + delx, 3*w/8.0 + dely );
  W.addPolyPoint( 13*w/32.0 + delx, 3*w/8.0 + dely );
  W.addPolyPoint( 5*w/16.0 + delx, 13*w/16.0 + dely );
  W.addPolyPoint( w/4.0 + delx, 13*w/16.0 + dely );
  W.drawPolyPoints();

  W.show();

  while (1) {
    int k = cv::waitKey(33);
    if (k==27) { // Esc key to stop
      return(0);
    }
  }

}

  
