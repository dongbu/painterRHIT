#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

/// Global Variables

// webcam
Point2f webcamQuad[4]; // Input Quadilateral or Image plane coordinates
Point2f canvasCornersQuad[4]; // Output Quadilateral or World plane coordinates

// projector
Point2f inputQuadP[4]; // Input Quadilateral or Image plane coordinates
Point2f outputQuadP[4]; // Output Quadilateral or World plane coordinates

// canvas error
Point2f projPhotoOnProjectorQuad[4]; // Input Quadilateral or Image plane coordinates
Point2f projPhotoOnCanvasQuad[4]; // Input Quadilateral or Image plane coordinates
Point2f paintBoxOnCanvasQuad[4]; // Output Quadilateral or World plane coordinates

int g_webcam_corner = 0;	
int g_projector_corner = 0;	
int g_canvas_pixels_width = 1000;
int g_canvas_pixels_height = 800;

int g_flip_webcam = 1;

// 1080p resolution
int g_projector_pixels_width = 1920;
int g_projector_pixels_height = 1000; //should be 1080... but reduce to have image header

// handle mouse clicks in the webcam window
void webcamWindowMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    printf("Setting canvas corner %i to %i, %i\n",g_webcam_corner+1, x, y);
    webcamQuad[g_webcam_corner] = Point2f( x,y );

  } else if  ( event == EVENT_RBUTTONDOWN ) {
    cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if  ( event == EVENT_MBUTTONDOWN ) {
    cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if ( event == EVENT_MOUSEMOVE ) {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
  }
}

// handle mouse clicks in the webcam window
void projectorWindowMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    printf("Setting projector corner %i to %i, %i\n",g_projector_corner+1, x, y);
    projPhotoOnCanvasQuad[g_projector_corner] = Point2f( x,y );
  }
}

int main(int, char**)
{

  int camID = 0;
  VideoCapture cap0(camID); // open the default camera
  if(!cap0.isOpened()) {  // check if we succeeded
    printf("Camera0 not open\n");
    return -1;
  }
  
  namedWindow("webcam0",1);
  moveWindow("webcam0",100,30);
  setMouseCallback("webcam0", webcamWindowMouseCallBackFunc, NULL);

  namedWindow("webcam corrected",1);
  moveWindow("webcam corrected",100,300);
             
  Mat webcam;
  cap0 >> webcam; // get a new frame from camera

  // this is the "canvas"
  Mat webcam_corrected = Mat::zeros(g_canvas_pixels_height, g_canvas_pixels_width, webcam.type() );

  // Lambda Matrix
  Mat webcam_lambda( 2, 4, CV_32FC1 );

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  webcamQuad[0] = Point2f( 0,0 );
  webcamQuad[1] = Point2f( webcam.cols-0,0);
  webcamQuad[2] = Point2f( webcam.cols-0,webcam.rows-0);
  webcamQuad[3] = Point2f( 0,webcam.rows-0  ); 
  printf("Input is %i by %i\n",webcam.cols,webcam.rows);
  printf("Click on upper left corner of canvas (press 1,2,3,4 to pick corner)\n");

  // The 4 points where the mapping is to be done , from top-left in clockwise order
  canvasCornersQuad[0] = Point2f( 0,0 );
  canvasCornersQuad[1] = Point2f( g_canvas_pixels_width,0);
  canvasCornersQuad[2] = Point2f( g_canvas_pixels_width,g_canvas_pixels_height);
  canvasCornersQuad[3] = Point2f( 0,g_canvas_pixels_height );

  int webcam_done=0;
  while (!webcam_done)
    {
      // blend a few webcam images together
      cap0 >> webcam; // get a new frame from camera
      Mat temp_webcam;
      for (int i=0; i<10; i++) {
	cap0 >> temp_webcam; // get a new frame from camera
	addWeighted( temp_webcam, .1, webcam, .9, 0.0, webcam);
      }

      if (g_flip_webcam) {
	flip(webcam,webcam,-1); // horizontal and vertically
      }
      imshow("webcam0", webcam);

      // Set the lambda matrix the same type and size as webcam
      webcam_lambda = Mat::zeros( webcam.rows, webcam.cols, webcam.type() );
		
      // Get the Perspective Transform Matrix i.e. lambda
      webcam_lambda = getPerspectiveTransform( webcamQuad, canvasCornersQuad );
	
      // Apply the Perspective Transform just found to the src image
      warpPerspective(webcam, webcam_corrected,webcam_lambda,webcam_corrected.size() );
      imshow("webcam corrected", webcam_corrected);

      int k = waitKey(33);
      if (k==27) { // Esc key to stop
	break;
      } else if (k == int('1')) {
	g_webcam_corner = 0;
	printf("Click on upper left corner\n");
      } else if (k == int('2')) {
	g_webcam_corner = 1;
	printf("Click on upper right corner\n");
      } else if (k == int('3')) {
	g_webcam_corner = 2;
	printf("Click on lower right corner\n");
      } else if (k == int('4')) {
	g_webcam_corner = 3;
	printf("Click on lower left corner\n");

      } else if (k == int('s')) { 
	printf("saving webcam calibration matrix to ...\n");
	print(webcam_lambda);

      } else if (k == int('x')) { 
	webcam_done=1;
      }
      if (k>0) { printf("K:%d\n",k); }
      
    }

  Mat checkerboard = imread( "images/2000px-Checkerboard_pattern-5x5orange.png", 1 );
  Mat projector_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_32F);
  resize(checkerboard, projector_image, projector_image.size(), 0, 0, CV_INTER_CUBIC);
  Mat projector_corrected = projector_image.clone(); 

  namedWindow("projector",1);
  imshow("projector", projector_image);
  setMouseCallback("webcam corrected", projectorWindowMouseCallBackFunc, NULL);
  namedWindow("projector corrected",1);
  imshow("projector corrected", projector_corrected);

  // Lambda Matrix
  Mat error_lambda( 2, 4, CV_32FC1 );

  // These four pts are the sides of the original projected painting on the canvas
  projPhotoOnCanvasQuad[0] = Point2f( 0,0 );
  projPhotoOnCanvasQuad[1] = Point2f( projector_corrected.cols-0,0);
  projPhotoOnCanvasQuad[2] = Point2f( projector_corrected.cols-0,projector_corrected.rows-0);
  projPhotoOnCanvasQuad[3] = Point2f( 0,projector_corrected.rows-0  ); 
  //printf("Input is %i by %i\n",input.cols,input.rows);
  printf("Click on upper left corner of canvas (press 1,2,3,4 to pick corner)\n");

  // The 4 points where we want the painting on the canvas
  //paintBoxOnCanvasQuad[0] = Point2f( 100,100 );
  //paintBoxOnCanvasQuad[1] = Point2f( min_dimension-100,100);
  //paintBoxOnCanvasQuad[2] = Point2f( min_dimension-100,min_dimension-100);
  //paintBoxOnCanvasQuad[3] = Point2f( 100,min_dimension-100  ); 
  for (int i=0; i<4; i++) {
    paintBoxOnCanvasQuad[i] = projPhotoOnCanvasQuad[i];
  }

  //  paintBoxOnCanvasQuad[0] = Point2f( 100,100 );
  //paintBoxOnCanvasQuad[1] = Point2f( 300,100);
  //paintBoxOnCanvasQuad[2] = Point2f( 300,300);
  //paintBoxOnCanvasQuad[3] = Point2f( 100,300);

  float delta = 1.0;
  int projector_done=0;
  while (!projector_done)
    {
      // blend a few webcam images together
      cap0 >> webcam; // get a new frame from camera
      Mat temp_webcam;
      for (int i=0; i<-3; i++) {
	cap0 >> temp_webcam; // get a new frame from camera
	addWeighted( temp_webcam, .1, webcam, .9, 0.0, webcam);
      }

      if (g_flip_webcam) {
	flip(webcam,webcam,-1); // horizontal and vertically
      }
      imshow("webcam0", webcam);

      // Apply the Perspective Transform for the webcam
      warpPerspective(webcam, webcam_corrected,webcam_lambda,webcam_corrected.size() );
      imshow("webcam corrected", webcam_corrected);

      // Set the lambda matrix the same type and size as webcam
      error_lambda = Mat::zeros( webcam_corrected.rows, webcam_corrected.cols, webcam_corrected.type() );
		
      // Get the Perspective Transform Matrix i.e. lambda
      error_lambda = getPerspectiveTransform( paintBoxOnCanvasQuad, projPhotoOnCanvasQuad );

      // Apply the Perspective Transform for the webcam
      warpPerspective(projector_image, projector_corrected,error_lambda,projector_corrected.size() );
      imshow("projector corrected", projector_corrected);
	
      int k = waitKey(33);
      if (k==27) { // Esc key to stop
	break;
      } else if (k == int('1')) {
	g_projector_corner = 0;
	printf("Click on upper left corner\n");
      } else if (k == int('2')) {
	g_projector_corner = 1;
	printf("Click on upper right corner\n");
      } else if (k == int('3')) {
	g_projector_corner = 2;
	printf("Click on lower right corner\n");
      } else if (k == int('4')) {
	g_projector_corner = 3;
	printf("Click on lower left corner\n");

      } else if (k == 63235) { // right arrow
	printf("moving %i.x to right by %.3f\n",g_projector_corner,delta);
	projPhotoOnCanvasQuad[g_projector_corner].x += delta;
      } else if (k == 63234) { // left arrow
	printf("moving %i.x to left by %.3f\n",g_projector_corner,delta);
	projPhotoOnCanvasQuad[g_projector_corner].x += -delta;
      } else if (k == 63232) { // up arrow
	printf("moving %i.y up by %.3f\n",g_projector_corner,delta);
	projPhotoOnCanvasQuad[g_projector_corner].y += -delta;
      } else if (k == 63233) { // down arrow
	printf("moving %i.y down by %.3f\n",g_projector_corner,delta);
	projPhotoOnCanvasQuad[g_projector_corner].y += delta;

      } else if (k == 43) { // +
	delta = delta * 2;
	printf("Changed delta to %.1f\n",delta);
      } else if (k == 45) { // -
	delta = delta * 0.5;
	printf("Changed delta to %.1f\n",delta);

      } else if (k == int('s')) { 
	printf("saving projector calibration matrix to ...\n");
	print(error_lambda);

      } else if (k == int('x')) { 
	projector_done=1;
      }
      
    }

  /////////////////////////////////////////
  /////////////////////////////////////////
  // UNUSED
  /////////////////////////////////////////
  /////////////////////////////////////////

  checkerboard = imread( "images/2000px-Checkerboard_pattern-5x5orange.png", 1 );
  projector_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_32F);
  resize(checkerboard, projector_image, projector_image.size(), 0, 0, CV_INTER_CUBIC);
  projector_corrected = projector_image.clone(); 

  namedWindow("projector",1);
  imshow("projector", projector_image);
  setMouseCallback("webcam corrected", projectorWindowMouseCallBackFunc, NULL);
  namedWindow("projector corrected",1);
  imshow("projector corrected", projector_corrected);

  // Lambda Matrix
  Mat projector_inverse_lambda( 2, 4, CV_32FC1 );
  Mat projector_lambda( 2, 4, CV_32FC1 );
  //Mat error_lambda( 2, 4, CV_32FC1 );

  // These four pts are the sides of the original projected painting on the canvas
  projPhotoOnCanvasQuad[0] = Point2f( 0,0 );
  projPhotoOnCanvasQuad[1] = Point2f( webcam_corrected.cols-0,0);
  projPhotoOnCanvasQuad[2] = Point2f( webcam_corrected.cols-0,webcam_corrected.rows-0);
  projPhotoOnCanvasQuad[3] = Point2f( 0,webcam_corrected.rows-0  ); 
  //printf("Input is %i by %i\n",input.cols,input.rows);
  printf("Click on upper left corner of canvas (press 1,2,3,4 to pick corner)\n");

  // The 4 points where we want the painting on the canvas
  int min_dimension = webcam_corrected.cols;
  if (webcam_corrected.rows < min_dimension) { min_dimension = webcam_corrected.rows; }
  paintBoxOnCanvasQuad[0] = Point2f( 100,100 );
  paintBoxOnCanvasQuad[1] = Point2f( min_dimension-100,100);
  paintBoxOnCanvasQuad[2] = Point2f( min_dimension-100,min_dimension-100);
  paintBoxOnCanvasQuad[3] = Point2f( 100,min_dimension-100  ); 

  //  paintBoxOnCanvasQuad[0] = Point2f( 100,100 );
  //paintBoxOnCanvasQuad[1] = Point2f( 300,100);
  //paintBoxOnCanvasQuad[2] = Point2f( 300,300);
  //paintBoxOnCanvasQuad[3] = Point2f( 100,300);

  projector_done=0;
  while (!projector_done)
    {
      // blend a few webcam images together
      cap0 >> webcam; // get a new frame from camera
      Mat temp_webcam;
      for (int i=0; i<10; i++) {
	cap0 >> temp_webcam; // get a new frame from camera
	addWeighted( temp_webcam, .1, webcam, .9, 0.0, webcam);
      }

      if (g_flip_webcam) {
	flip(webcam,webcam,-1); // horizontal and vertically
      }
      imshow("webcam0", webcam);

      // Apply the Perspective Transform for the webcam
      warpPerspective(webcam, webcam_corrected,webcam_lambda,webcam_corrected.size() );
      imshow("webcam corrected", webcam_corrected);

      // Set the lambda matrix the same type and size as webcam
      projector_lambda = Mat::zeros( projector_image.rows, projector_image.cols, projector_image.type() );
      projector_inverse_lambda = Mat::zeros( projector_image.rows, projector_image.cols, projector_image.type() );
      error_lambda = Mat::zeros( webcam_corrected.rows, webcam_corrected.cols, webcam_corrected.type() );
		
      // Get the Perspective Transform Matrix i.e. lambda
      projector_lambda = getPerspectiveTransform( projPhotoOnCanvasQuad, projPhotoOnProjectorQuad );
      projector_inverse_lambda = getPerspectiveTransform( projPhotoOnProjectorQuad, projPhotoOnCanvasQuad );
      error_lambda = getPerspectiveTransform( paintBoxOnCanvasQuad, projPhotoOnCanvasQuad );

      // Apply the Perspective Transform for the webcam
      warpPerspective(projector_image, projector_corrected,error_lambda,projector_corrected.size() );
      warpPerspective(projector_image, projector_corrected,error_lambda,projector_corrected.size() );
      warpPerspective(projector_image, projector_corrected,error_lambda,projector_corrected.size() );
      imshow("projector corrected", projector_corrected);
	
      int k = waitKey(33);
      if (k==27) { // Esc key to stop
	break;
      } else if (k == int('1')) {
	g_projector_corner = 0;
	printf("Click on upper left corner\n");
      } else if (k == int('2')) {
	g_projector_corner = 1;
	printf("Click on upper right corner\n");
      } else if (k == int('3')) {
	g_projector_corner = 2;
	printf("Click on lower right corner\n");
      } else if (k == int('4')) {
	g_projector_corner = 3;
	printf("Click on lower left corner\n");

      } else if (k == int('s')) { 
	printf("saving projector calibration matrix to ...\n");
	print(projector_lambda);

      } else if (k == int('x')) { 
	projector_done=1;
      }
      
    }


  waitKey();


  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}

