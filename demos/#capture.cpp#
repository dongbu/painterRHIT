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
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
Mat fuzzy;
Mat edges;
Mat aveedges;
Mat grey;

/// Function header
void thresh_callback(int, void* );

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow("edges",1);
    moveWindow("edges",100,30);

    namedWindow("aveedges",1);
    moveWindow("aveedges",100,300);

    createTrackbar( " Canny thresh:", "aveedges", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );

    namedWindow("contours",1);
    moveWindow("contours",100,500);

    //    cap >> aveedges; // get a new frame from camera
      
    int c=0;

    for(;;)
    {
      c++;
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
	grey=edges.clone();

        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

	if (c==1) {
	  aveedges = edges.clone();
	}
	
	addWeighted( edges, .1, aveedges, .9, 0.0, aveedges);
	addWeighted( aveedges, .9, grey, .1, 0.0, aveedges);
        imshow("aveedges", aveedges);

	fuzzy=aveedges.clone();

	//        imshow("edges", edges);
        if(waitKey(10) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  //Canny( fuzzy, canny_output, thresh, thresh*2, 3 );
  Canny( edges, canny_output, thresh, thresh*2, 3 );

  /// Find contours
  return;
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
