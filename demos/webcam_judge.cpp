#include "webcam.cpp"
#include "drawwindow.cpp"
#include "shapes.cpp"

// use webcam to score how close a painting is to the desired result

int colorCloseness(cv::Vec3b c1, cv::Vec3b c2) {
  return abs(c1[0]-c2[0]) + abs(c1[1]-c2[1]) + abs(c1[2]-c2[2]);
}

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
  INTRO.drawText(10,10,"m=snap a webcam frame for desired painting");
  INTRO.drawText(10,20,"m=start judging");
  INTRO.drawText(10,30,"c=calibrate webcam(1-4,arrows,s=save,esc=done)");
  INTRO.show(); 

  int width=280;
  int height=200;
  int blackwhite = 0; 
  cv::Mat frame;
  cv::Mat bwframe;

  Webcam W;
  W.setMapSize(width,height);

  DrawWindow DW = DrawWindow(width,height,"Desired Painting"); 
  DW.moveWindow(20,150);
  W.getMappedFrame(&frame);
  cvtColor( frame, bwframe, CV_BGR2GRAY );
  if (blackwhite) {
    DW.grid = bwframe.clone(); 
  } else {
    DW.grid = frame.clone(); 
  }
  DW.show();

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
      cvtColor( frame, bwframe, CV_BGR2GRAY );
      if (blackwhite) {
	DW.grid = bwframe.clone(); 
      } else {
	DW.grid = frame.clone(); 
      }
      DW.show();
    }

    if (k == int('m')) {

      int done=0;
      while (!done) {
	W.getMappedFrame(&frame);
	cvtColor( frame, bwframe, CV_BGR2GRAY );
	
	cv::moveWindow(map_window,1*(width+5)+20,150);
	if (blackwhite) {
	  cv::imshow(map_window, bwframe);
	} else {
	  cv::imshow(map_window, frame);
	}
	//cvtColor( frame, frame, CV_RGB2GRAY );
	
	int right=0;
	int wrong=0;
	EW.setCanvasColor(0,0,0);

	for (int i=0; i<width; i++) {
	  for (int j=0; j<height; j++) {
	    cv::Vec3b desired_color = DW.getColor(i,j);
	    cv::Vec3b webcam_color = frame.at<cv::Vec3b>(cv::Point(i,j));
	    int closeness = colorCloseness(desired_color,webcam_color);
	    int cdiff = desired_color[0]-webcam_color[0] + desired_color[1]-webcam_color[1] + desired_color[2]-webcam_color[2];

	    if (0 && i==33 && j==22) { 
	      printf("%i %i,%i [%i %i %i] [%i %i %i] =>%i\n",cdiff, i,j,
		     desired_color[0],desired_color[1],desired_color[2],
		     webcam_color[0],webcam_color[1],webcam_color[2],
		     closeness); 
	    }
	    int c = 128 + cdiff/(2); 
	    c = closeness;
	    if (c>255) { c=255; }  
	    if (c<0) { c=0; }

	    EW.setPenColor(c,c,c);
	    EW.drawPixel(i,j);
	    if (closeness<50) {
	      right++;
	    } else {
	      wrong++;
	    }
	  }
	}

	EW.setPenColor(200,200,200);
	EW.drawRectangle(0,0,200,12,1);
	EW.setPenColor(0,0,0);
	char text [50];
	sprintf(text,"%.0f%%  OK:%i TODO:%i",(double)100.*right/(right+wrong),right,wrong);
	EW.drawText(10,10,text);
	EW.show();
	DW.show();
	//printf("%i OK, %i bad pixels\n",right,wrong);
	
	int k = cv::waitKey(33);
	if (k==27 || k == int('m')) { done=1; }
      }
    }

  }
}
