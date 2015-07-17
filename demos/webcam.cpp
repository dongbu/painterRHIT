#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::VideoCapture cam0(0); // open the default camera
cv::VideoCapture cam1(0); // open cam 1

// Packages simple drawing commands for the simulator display window
class Webcam {
protected:
  cv::VideoCapture *cam0ptr;
  cv::VideoCapture *cam1ptr;
  cv::VideoCapture *camptr;
  int map_width;
  int map_height;
  int cam_id;
  int flip_webcam; 
  int webcam_corner;
  cv::Point2f webcamQuad[4]; // Input Quadilateral or Image plane coordinates
  cv::Point2f canvasCornersQuad[4]; // Output Quadilateral or World plane coordinates
  cv::Point2f zoomQuad[4]; // Output Quadilateral or World plane coordinates

public:

  // handle mouse clicks in the webcam window
  // note: had to put "static" in front to get setMouseCallback to take it... but it doesn't
  // then allow you to set values
  static void mouseCallbackFunc(int event, int x, int y, int flags, void* userdata)
  {
    printf("clicked on %i,%i\n",x,y);
    if  ( event == cv::EVENT_LBUTTONDOWN ) {
      //printf("Setting canvas corner %i to %i, %i\n",webcam_corner+1, x, y);
      //webcamQuad[webcam_corner] = cv::Point2f( x,y );
    }
  }

  // reset the map used to map webcam to another matrix/frame
  void resetMapping() {   
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    canvasCornersQuad[0] = cv::Point2f( 0,0 );
    canvasCornersQuad[1] = cv::Point2f( map_width,0);
    canvasCornersQuad[2] = cv::Point2f( map_width,map_height);
    canvasCornersQuad[3] = cv::Point2f( 0,map_height );
    for (int i=0; i<4; i++) { zoomQuad[i] = cv::Point2f( 0,0 ); }
  }

  void setMapping(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    canvasCornersQuad[0] = cv::Point2f( x1, y1 );
    canvasCornersQuad[1] = cv::Point2f( x2, y2 );
    canvasCornersQuad[2] = cv::Point2f( x3, y3 );
    canvasCornersQuad[3] = cv::Point2f( x4, y4 );
  }

  double * getMapping() {
    static double map[8];
    map[0] = canvasCornersQuad[0].x;
    map[1] = canvasCornersQuad[0].y;
    map[2] = canvasCornersQuad[1].x;
    map[3] = canvasCornersQuad[1].y;
    map[4] = canvasCornersQuad[2].x;
    map[5] = canvasCornersQuad[2].y;
    map[6] = canvasCornersQuad[3].x;
    map[7] = canvasCornersQuad[3].y;
    return map;
  }

  static void zoomMouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    Webcam *self = static_cast<Webcam*>(userdata);
    if  ( event == cv::EVENT_LBUTTONDOWN ) {
      printf("Setting zoom corner %d to %i,%i\n",x,y,self->webcam_corner);
      self->zoomQuad[self->webcam_corner].x=x;
      self->zoomQuad[self->webcam_corner].y=y;
    }
  }

  void calibrateWebcam () {
    cv::Mat webcam;
    cv::Mat mapped_webcam; // this is the webcam mapped to the same dimensions as the final canvas pixels
    cv::Mat canvas; // this is the "canvas"
    char window_name[] = "Mapped Webcam";
    int debug = 1;

    cv::namedWindow(window_name,1);

    cv::namedWindow("Canvas",1);

    //cv::moveWindow(window_name,100,300);

    cv::namedWindow("webcam",1);
    cv::setMouseCallback("webcam", zoomMouseCallBackFunc, this);

    resetMapping();

    float delta = 16.0;
    int done=0;
    while (!done)
      {
	// blend a few webcam images together
	getFrame(&webcam,3); // get a new frame from camera (blend 3 frames for better clarity)
	// imshow("webcam0", webcam);
	cv::imshow("webcam", webcam);

	getMappedFrame(&mapped_webcam);
	cv::imshow(window_name, mapped_webcam);

	int k = cv::waitKey(33);
	if (k==27) { // Esc key to stop
	  done=1;
	} else if (k == int('1')) {
	  webcam_corner = 0;
	  if (debug) printf("Use arrows to move upper left corner\n");
	} else if (k == int('2')) {
	  webcam_corner = 1;
	  if (debug) printf("Use arrows to move upper right corner\n");
	} else if (k == int('3')) {
	  webcam_corner = 2;
	  if (debug) printf("Use arrows to move lower right corner\n");
	} else if (k == int('4')) {
	  webcam_corner = 3;
	  if (debug) printf("Use arrows to move lower left corner\n");

	} else if (k == 63235) { // right arrow
	  if (debug) printf("Moving %i.x to right by %.3f\n",webcam_corner,delta);
	  webcamQuad[webcam_corner].x += delta;
	  //canvasCornersQuad[webcam_corner].x += delta;
	} else if (k == 63234) { // left arrow
	  if (debug) printf("Moving %i.x to left by %.3f\n",webcam_corner,delta);
	  webcamQuad[webcam_corner].x += -delta;
	  //canvasCornersQuad[webcam_corner].x += -delta;
	} else if (k == 63232) { // up arrow
	  if (debug) printf("Moving %i.y up by %.3f\n",webcam_corner,delta);
	  webcamQuad[webcam_corner].y += -delta;
	  //canvasCornersQuad[webcam_corner].y += -delta;
	} else if (k == 63233) { // down arrow
	  if (debug) printf("Moving %i.y down by %.3f\n",webcam_corner,delta);
	  webcamQuad[webcam_corner].y += delta;
	  //canvasCornersQuad[webcam_corner].y += delta;
	} else if (k == 43) { // +
	  delta = delta * 2;
	  if (debug) printf("Changed delta to %.1f\n",delta);
	} else if (k == 45) { // -
	  delta = delta * 0.5;
	  if (debug) printf("Changed delta to %.1f\n",delta);

	} else if (k == int('x')) { 
	  done=1;
	}
	//if (k>0) { printf("K:%d\n",k); }
      }
    printf("Webcam calibration matrix defined.\n");
    cv::destroyWindow(window_name);
  }

  // returns a 2x4 array which is the mapping of a frame to the canvas mapping
  cv::Mat getMapLambda(cv::Mat *frame) {
    cv::Mat frame_lambda( 2, 4, CV_32FC1 );

    // Set the lambda matrix the same type and size as webcam
    frame_lambda = cv::Mat::zeros( frame->rows, frame->cols, frame->type() );
		
    // Get the Perspective Transform Matrix i.e. lambda
    frame_lambda = cv::getPerspectiveTransform( webcamQuad, canvasCornersQuad );

    return frame_lambda;
  }

  // sets frame to a mapping of the webcam 
  void getMappedFrame(cv::Mat *mapped_frame) {
    cv::Mat webcam;
    getFrame(&webcam); // get a new frame from camera

    // zoom into the webcam to where ever the canvas corners are
    if (1) {
      cv::Mat zoom_lambda( 2, 4, CV_32FC1 );
      // Set the lambda matrix the same type and size as webcam
      zoom_lambda = cv::Mat::zeros( webcam.rows, webcam.cols, webcam.type() );
      // Get the Perspective Transform Matrix i.e. lambda
      zoom_lambda = cv::getPerspectiveTransform( zoomQuad,webcamQuad );
      // Apply the Perspective Transform just found to the src image
      //cv::warpPerspective(canvas,mapped_webcam,zoom_lambda,mapped_webcam.size() );
      cv::warpPerspective(webcam,webcam,zoom_lambda,webcam.size() );
    }
    
    // create the mapped_frame 
    *mapped_frame = cv::Mat::zeros(map_height, map_width, webcam.type() );

    cv::Mat webcam_lambda = getMapLambda(&webcam);
	
    // Apply the Perspective Transform just found to the src image
    cv::warpPerspective(webcam, *mapped_frame,webcam_lambda,mapped_frame->size() );
  }

  cv::Mat getMappedFrame() {
    cv::Mat mappedFrame;
    getMappedFrame(&mappedFrame);
    return mappedFrame;
  }

  void setMapSize(int w, int h) { // this is the projection of the webcam to an arbitrary size
    map_height=h;
    map_width=w;
    resetMapping();
  }

  void setFlip(int flip) { // set 1 to flip over the webcam
    flip_webcam = flip; 
  }

  cv::Mat capture() { // unused
    cv::Mat frame;
    *camptr >> frame; // get a new frame from camera
    return frame;
  }

  int getFrame(cv::Mat *frame, int loops=0) {
    if (cam_id==0) {
      if(cam0.isOpened()) {  // check if we succeeded
	cam0 >> *frame;
	if (flip_webcam) { flip(*frame,*frame,-1); } // horizontal and vertically

	if (loops>0) {
	  cv::Mat temp_webcam;
	  for (int i=0; i<loops; i++) {
	    //printf("get frame loop %i\n",i);
	    getFrame(&temp_webcam); // get a new frame from camera
	    cv::addWeighted( temp_webcam, .1, *frame, .9, 0.0, *frame);
	  }
	}

	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	// These four pts are the sides of the rect box used as input (used for mapping to another frame/matrix)
	webcamQuad[0] = cv::Point2f( 0,0 );
	webcamQuad[1] = cv::Point2f( frame->cols-0,0);
	webcamQuad[2] = cv::Point2f( frame->cols-0,frame->rows-0);
	webcamQuad[3] = cv::Point2f( 0,frame->rows-0  ); 

	return 1;
      } else {
	printf("Cam 0 didn't open\n");
      }
    } else {
      if(cam1.isOpened()) {  // check if we succeeded
	cam1 >> *frame;
	if (flip_webcam) { flip(*frame,*frame,-1); } // horizontal and vertically

	if (loops>0) {
	  cv::Mat temp_webcam;
	  for (int i=0; i<loops; i++) {
	    getFrame(&temp_webcam); // get a new frame from camera
	    cv::addWeighted( temp_webcam, .1, *frame, .9, 0.0, *frame);
	  }
	}
	return 1;
      } else {
	printf("Cam 1 didn't open\n");
      }
    }
    return 0;
  }

  Webcam() { // constructor
    cam_id=0;
    map_width=600;
    map_height=400;
    flip_webcam=0;
    webcam_corner = 0;	

    resetMapping();

    if (0) { // was trying to open webcam only when create this object... but cam not persistent after constructor
      cv::VideoCapture cam0(0); // open the default camera
      cam0ptr = &cam0;
      if(cam0.isOpened()) {  // check if we succeeded
	printf("Camera0 opened ok\n");
	camptr = cam0ptr;
      } else {
	printf("Camera0 not open\n");
      }
      
      int cam1_on=1;
      cv::VideoCapture cam1(1); // open the default camera
      cam1ptr = &cam1;
      if(cam1.isOpened()) {  // check if we succeeded
	printf("Camera1 opened ok\n");
	camptr = cam1ptr;
      } else {
	cam1_on=0;
	printf("Camera1 not open\n");
      }
    }
  }

  ~Webcam() {
  }

};

