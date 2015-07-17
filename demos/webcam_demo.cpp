#include "webcam.cpp"
#include "drawwindow.cpp"
#include "shapes.cpp"

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
  Rectangle *R = new Rectangle();
  //R->setPenColor(0,0,0);
  R->setFill(1);
  R->setCorners(.4*width,.2*height,.65*width,.9*height);
  S.addShape(R);
  R = new Rectangle();
  //R->setPenColor(0,0,0);
  R->setFill(1);
  R->setCorners(.2*width,.5*height,.8*width,.7*height);
  S.addShape(R);
  S.drawAll(&DW);
  DW.moveWindow(20,150);
  DW.show();

  cv::Mat frame;
  cv::Mat errors;

  char map_window[] = "Mapped Webcam";
  cv::namedWindow( map_window, CV_WINDOW_AUTOSIZE );
  char errors_window[] = "Painting Errors";
  cv::namedWindow( errors_window, CV_WINDOW_AUTOSIZE );
  
  while (1) {   
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop
    if (k == int('c')) { W.calibrateWebcam(); }

    if (k == int('m')) {
      printf("showing mapped frame\n");
      W.getMappedFrame(&frame);

      //cv::RNG rng(12345);
      //for (int i=0; i<1000; i++) { frame.at<cv::Vec3b>(cv::Point(rng.uniform(0,width),rng.uniform(0,height))) = cv::Vec3b(0,0,0);       }
      cv::moveWindow(map_window,1*(width+5)+20,150);
      cv::imshow(map_window, frame);

      cv::Vec3b pen_color = cv::Vec3b(0,0,0);

      int right=0;
      int wrong=0;
      errors = cv::Mat::zeros(height, width, frame.type() );
      for (int i=0; i<width; i++) {
	for (int j=0; j<height; j++) {
	  cv::Vec3b desired_color = DW.getColor(i,j);
	  if (colorClose(desired_color,pen_color,10)) { 
	    cv::Vec3b webcam_color = frame.at<cv::Vec3b>(cv::Point(i,j));
	    if (colorClose(desired_color,webcam_color,10+30)) { 
	      right++;
	      errors.at<cv::Vec3b>(cv::Point(i,j)) = desired_color;
	    } else {
	      wrong++;
	      errors.at<cv::Vec3b>(cv::Point(i,j)) = cv::Vec3b(0,0,255);
	    }
	  } else {
	    errors.at<cv::Vec3b>(cv::Point(i,j)) = cv::Vec3b(0,255,0); // untouched
	  }
	}
      }

      printf("%i OK, %i bad pixels\n",right,wrong);

      cv::moveWindow(errors_window,2*(width+5)+20,150);
      cv::imshow(errors_window, errors);
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
