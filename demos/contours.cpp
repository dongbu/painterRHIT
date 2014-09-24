#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include "kmeansSegment.hpp"
#include "kmeansSegment.cpp"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int g_thresh = 100;
int g_max_thresh = 255;
int g_colors = 10;
int g_max_colors = 64;
int g_min_contour_length = 10;
RNG rng(12345);


void colorReduce(cv::Mat& image, int div=64)
{    
    int nl = image.rows;                    // number of lines
    int nc = image.cols * image.channels(); // number of elements per line

    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);

        for (int i = 0; i < nc; i++)
        {
            // process each pixel
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

void reduceTo64Colors(IplImage *img, IplImage *img_quant) {
    int i,j;
    int height   = img->height;   
    int width    = img->width;    
    int step     = img->widthStep;

    uchar *data = (uchar *)img->imageData;
    int step2 = img_quant->widthStep;
    uchar *data2 = (uchar *)img_quant->imageData;

    for (i = 0; i < height ; i++)  {
        for (j = 0; j < width; j++)  {

          // operator XXXXXXXX & 11000000 equivalent to  XXXXXXXX AND 11000000 (=192)
          // operator 01000000 >> 2 is a 2-bit shift to the right = 00010000 
          uchar C1 = (data[i*step+j*3+0] & 192)>>2;
          uchar C2 = (data[i*step+j*3+1] & 192)>>4;
          uchar C3 = (data[i*step+j*3+2] & 192)>>6;

          data2[i*step2+j] = C1 | C2 | C3; // merges the 2 MSB of each channel
        }     
    }
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, g_thresh, g_thresh*2, 3 );
  /// Show in a window
  namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
  moveWindow("Canny",700,600);
  imshow( "Canny", canny_output );

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  // simplified contours
  vector<vector<Point> > contours_poly( contours.size() ); 
  for( int i = 0; i< contours.size(); i++ ) {
    approxPolyDP( Mat(contours[i]), contours_poly[i], 1.1, true ); 
  }

  //approxPolyDP(InputArray curve, OutputArray approxCurve, double epsilon, bool closed)

  /// Draw contours
  int num_drawn=0;
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ ) {
    //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    Scalar color = Scalar( 30+i%225, 30+i%225, 30+i%225);
    if (contours_poly[i].size()>g_min_contour_length) {
      num_drawn++;
      drawContours( drawing, contours_poly, i, color, 1, 8, hierarchy, 0, Point() );      
    }
  }

  char text[50];
  sprintf(text,"Lines: drew %i out of %lu",num_drawn,contours.size());
  putText(drawing, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  moveWindow("Contours",1250,600);
  imshow( "Contours", drawing );
  
  /**********************************/

  // only finds the most external contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  //perhaps have to throw out old drawing?
  num_drawn=0;
  drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )  {
       //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       Scalar color = Scalar( 30+i%225, 30+i%225, 30+i%225);
       if (contours[i].size()>g_min_contour_length) {
	 num_drawn++;
	 drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
       }
     }

  sprintf(text,"Lines: drew %i out of %lu",num_drawn,contours.size());
  putText(drawing, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);

  /// Show in a window
  namedWindow( "Contours EXTERNAL", CV_WINDOW_AUTOSIZE );
  moveWindow("Contours EXTERNAL",1800,600);
  imshow( "Contours EXTERNAL", drawing );
}

/** @function color_callback */
void colors_callback(int, void* )
{
  //  colorReduce(src);
  kmeansSegment kmeans(g_colors);
  cv::Mat result = kmeans.segment(src);

  char text[50];
  sprintf(text,"Colors:%i",g_colors);
  //  putText("kmeans_window", text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);
  putText(result, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);

  namedWindow( "kmeans_window", CV_WINDOW_AUTOSIZE );
  imshow( "kmeans_window", result );
  moveWindow("kmeans_window",650,100);

  // blur and try again... perhaps will make the transitionsmore smooth
  //result = adaptiveThreshold(result,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2)
  if (1) {
    for (int i=0; i<5; i++) {
      blur( result, result, Size(3,3) );
      result = kmeans.segment(result);
    }
  }

  namedWindow( "kmeans_window blur", CV_WINDOW_AUTOSIZE );
  imshow( "kmeans_window blur", result );
  moveWindow("kmeans_window blur",650,100);

  cvtColor( result, src_gray, CV_BGR2GRAY );

  // redraw the thresholds
  thresh_callback( 0, 0 );
}

void contour_length_callback(int, void* ) {
  // redraw the thresholds
  thresh_callback( 0, 0 );
}

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );
  if (src.empty()) {
    std::cout << "!!! Failed imread()\n ./contours <image filename>" << std::endl;
    return -1;
  }  

  //Mat image_64colors=src.clone();
  //reduceTo64Colors(src,image_64colors);
  //cvtColor( image_64colors, src_gray, CV_BGR2GRAY );

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  moveWindow(source_window,100,100);
  imshow( source_window, src );

  createTrackbar( " Canny thresh:", source_window, &g_thresh, g_max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  createTrackbar( " Colors:", source_window, &g_colors, g_max_colors, colors_callback );
  colors_callback( 0, 0 );

  createTrackbar( " Min Contour Length:", source_window, &g_min_contour_length, 100, contour_length_callback );

  waitKey(0);
  return(0);
}

