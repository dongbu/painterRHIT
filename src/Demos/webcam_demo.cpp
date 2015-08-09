#include "webcam.cpp"
#include "drawwindow.cpp"
#include "shapes.cpp"

// demo to show how webcam can be used to give feedback on what places still
// need to be painted ... assuming that the brush color is black

int colorClose(cv::Vec3b c1, cv::Vec3b c2, int max_distance=10) {

  if (abs(c1[0]-c2[0]) + abs(c1[1]-c2[1]) + abs(c1[2]-c2[2]) < max_distance) {
    return 1; 
  }
  return 0;

  cv::Vec3b d = c1-c2;
  double distance = cv::norm(d);
  //printf("D:%f\n",distance);
  if (distance>max_distance) return 0;
  return 1;
}

int main(void)
{
  // mostly to just have something to cycle waitKey (w/o a window, waitKey doesn't work?)
  DrawWindow INTRO = DrawWindow(400,80,"Webcam Tool Instructions"); 
  INTRO.drawText(10,10,"f=get webcam frame");
  INTRO.drawText(10,20,"F=get ave of 10 webcam frames");
  INTRO.drawText(10,30,"m=get mapped webcam frame");
  INTRO.drawText(10,40,"c=calibrate webcam(1-4,arrows,s=save,esc=done)");
  INTRO.drawText(10,50,"s=save mapping data");
  INTRO.drawText(10,60,"l=load mapping data");
  INTRO.show(); 

  int width=300;
  int height=400;
  Webcam W;
  W.setMapSize(width,height);

  DrawWindow DW = DrawWindow(width,height,"Desired Painting"); 
  Shapes S;
  RectangleShape *R = new RectangleShape();
  //R->setPenColor(0,0,0);
  R->setFill(1);
  R->setCorners(.4*width,.2*height,.65*width,.9*height);
  S.addShape(R);
  R = new RectangleShape();
  //R->setPenColor(0,0,0);
  R->setFill(1);
  R->setCorners(.2*width,.5*height,.8*width,.7*height);
  S.addShape(R);
  S.drawAll(&DW);
  DW.moveWindow(20,150);
  DW.show();

  cv::Mat frame;

  DrawWindow EW = DrawWindow(width,height,"Error Window");
  EW.moveWindow(2*(width+5)+20,150);

  char map_window[] = "Mapped Webcam";
  cv::namedWindow( map_window, CV_WINDOW_AUTOSIZE );
  
  while (1) {   
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop
    if (k == int('c')) { W.calibrateWebcam(); }
    
    if (k == int('s')) {  // make the desired painting be a snap of the webcam
      W.getMappedFrame(&frame);
      DW.grid = frame; 
      DW.show();
    }

    if (k == int('m')) {

      int done=0;
      while (!done) {
	W.getMappedFrame(&frame);
	
	cv::moveWindow(map_window,1*(width+5)+20,150);
	cv::imshow(map_window, frame);
	//cvtColor( frame, frame, CV_RGB2GRAY );
	
	cv::Vec3b pen_color = cv::Vec3b(0,0,0);

	int right=0;
	int wrong=0;
	EW.setCanvasColor(0,0,0);

	for (int i=0; i<width; i++) {
	  for (int j=0; j<height; j++) {
	    cv::Vec3b desired_color = DW.getColor(i,j);
	    //pen_color = DW.getColor(i,j);

	    if (colorClose(desired_color,pen_color,100)) { //if the captured frame maps to black
	      cv::Vec3b webcam_color = frame.at<cv::Vec3b>(cv::Point(i,j));
	      if (colorClose(desired_color,webcam_color,200)) {  //if the captured frame is < 200 from the actual
		right++;
		EW.setPenColor(desired_color[0],desired_color[1],desired_color[2]);
		EW.drawPixel(i,j);
	      } else { //if the captured frame is > 200 from the actual (FAILURE...)
		wrong++;
		EW.setPenColor(255,0,0);
		EW.drawPixel(i,j);
	      }
	    } else { //the pixel maps to white, and is assumed to be correct.
	      EW.setPenColor(0,255,0);
	      EW.drawPixel(i,j);
	    }
	  }
	}
	
	EW.setPenColor(0,0,0);
	char text [50];
	sprintf(text,"%.0f%%  OK:%i TODO:%i",(double)100.*right/(right+wrong),right,wrong);
	EW.drawText(10,10,text);
	EW.show();
	//printf("%i OK, %i bad pixels\n",right,wrong);
	
	int k = cv::waitKey(33);
	if (k==27 || k == int('m')) { done=1; }
      }
    }

    if (k == int('f')) {
      printf("showing frame\n");
      W.getFrame(&frame);
      cv::imshow("webcam", frame);
    }

    if (k == int('F')) {
      printf("showing merge of 10 frames\n");
      W.getFrame(&frame,10);
      cv::imshow("webcam", frame);
    }

    if (k == int('s')) { // "save"
      double *points = W.getWebcamZoom();
      printf("Here are the coordinates of 4 corners of webcam zoom\n");
      for (int i=0; i<4; i++) {
	printf("Corner %i = (%f,%f)\n",i,points[2*i],points[2*i+1]);
      }
    }

    if (k == int('l')) { // "load"
      double delx=.1;
      double dely=.2;
      W.getFrame(&frame);
      int width = frame.cols;
      int height = frame.rows;
      
      W.setWebcamZoom(delx*width,dely*height,
		      (1-delx)*width,dely*height,
		      (1-delx)*width,(1-dely)*height,
		      delx*width,(1-dely)*height);
      printf("zoom defined... type 'm' to see mapped capture\n");
    }
  }
}
