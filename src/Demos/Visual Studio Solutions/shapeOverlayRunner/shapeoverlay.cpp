#include "shapes.cpp"

int main(void)
{
  Shapes S;

  // add some lines
  int w=300;
  int h=200;
  
  // add a poly point region
  PolyPoints *PP = new PolyPoints();
  int ww=100;
  int delx=20;
  int dely=100;
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
  S.addShape(PP);
  
  // add rectangle
  RectangleShape *R = new RectangleShape();
  R->setCorners(20,20,50,60);
  R->setPenColor(200,0,0);
  S.addShape(R);

  R = new RectangleShape();
  R->setCorners(w-100,20,w-50,260);
  R->setPenColor(200,0,220);
  R->setFill(1);
  S.addShape(R);

  // draw the shapes
  DrawWindow W = DrawWindow(w,h,"Generated Shapes",1);
  W.clearWindow(230,230,230); // default background is white
  S.drawAll(&W);
  W.moveWindow(20,20);
  W.show();

  // create an overlay of the thing you want to highlight
  DrawWindow Woverlay = DrawWindow(w,h,"overlay",1);
  Woverlay.clearWindow(0,0,0);
  Woverlay.moveWindow(300,0);
  S.drawOne(&Woverlay,1,250,0,200); // r,g,b should not be 0,0,0
  Woverlay.show();

  DrawWindow Wtoggle = DrawWindow(w,h,"toggle shapes",1); // w,h

  int toggle=0;
  while (1) {
    int k = cv::waitKey(33);
    if (k==27) { return(0); }  // Esc key to stop

    if (k == int('n')) {
      if (toggle) {
	toggle=0;
	Wtoggle.copy(&W);
	Wtoggle.overlay(&Woverlay,1);
      } else {
	toggle=1;
	Wtoggle.copy(&W);
      }
      Wtoggle.show();
      Wtoggle.moveWindow(20+w,20);
    }
  }
}
