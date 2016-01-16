#include "shapes.cpp"
#include <unistd.h> // sleep

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
    //    S.addPolyLine(PL);
    S.addShape(PL);
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
  S.addShape(PP);
  
  // test overwriting with pixel region
  PixelRegion *PPtoPR = PP->toPixelRegion();
  PPtoPR->setPenColor(0,255,255);
  //S.addShape(&PPtoPR);

  // add pixel region
  PixelRegion *PR = new PixelRegion();
  PR->setPenColor(0,0,rng.uniform(0,244));
  for (int i=0; i<100; i++) {
    PR->addPoint(rng.uniform(0,w),rng.uniform(h-100,h));
  }
  S.addShape(PR);

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

  PolyLine *RtoPL = R->toPolyline();
  RtoPL->setPenColor(0,255,0);
  S.addShape(RtoPL);

  // test overwriting with pixel region
  PixelRegion *RtoPR = R->toPixelRegion();
  RtoPR->setPenColor(0,255,255);
  //  S.addShape(&RtoPR);

  // add circle
  EllipseShape *E = new EllipseShape();
  E->setFill(1);
  E->setPenColor(200,0,0);
  E->setData(30,200,20.0); // circle
  S.addShape(E);

  PolyLine *EtoPL = E->toPolyline();
  EtoPL->setPenColor(0,255,0);
  S.addShape(EtoPL);

  PixelRegion *EtoPR = E->toPixelRegion();
  EtoPR->setPenColor(255,255,0);
  S.addShape(EtoPR);

  E = new EllipseShape();
  E->setData(130,200,20.0,40.);
  S.addShape(E);

  // draw the shapes
  DrawWindow W = DrawWindow(w,h,"Generated Shapes",1);
  W.clearWindow(230,230,230); // default background is white
  S.drawAll(&W);
  W.show();

  // save to XML
  std::string xml = "<?xml version=\"1.0\"?>\n";
  xml.append(S.getXML());
  std::ofstream myfile;
  myfile.open ("shapes.xml");
  myfile << xml;
  myfile.close();
  if (debug) std::cout << xml;

  // load up a drawing from XML

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("shapes.xml");

  if (debug) std::cout << "Load result: " << result.description() << std::endl;

  pugi::xml_node shapes = doc.child("shapes");
  Shapes SS;
  SS.parseXML(&shapes);

  xml = "<?xml version=\"1.0\"?>\n";
  xml.append(SS.getXML());
  if (debug) std::cout << xml;

  DrawWindow W2 = DrawWindow(w,h,"Shapes Loaded via XML"); // w,h
  W2.clearWindow(230,230,230); // default background is white
  SS.drawAll(&W2);
  W2.show();

  // remove some shapes by ID
  for (int i=4; i<6; i++) { SS.removeShape(i); }

  DrawWindow W3 = DrawWindow(w,h,"removed shapes"); // w,h
  W3.clearWindow(230,230,230); // default background is white
  SS.drawAll(&W3);
  W3.show();

  while (1) {
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop

    if (k == int('n')) {
      

      

    }

  }

}
