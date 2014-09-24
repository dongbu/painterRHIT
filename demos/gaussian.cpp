#include <cv.h>
#include <highgui.h>
 
int main(int argc, char* argv[])
{
    //Load an image to transform !!
  IplImage* img = cvLoadImage( "images/lena.jpg" );
  
  // Define two window for showing input image and processed output of an Image.
  cvNamedWindow( "example_input" );
  cvNamedWindow( "example_output" );
  cvNamedWindow( "example_output2");
  cvMoveWindow("example_input", 10, 100);    // to move the window to position 100 100.
  cvMoveWindow("example_output", 600, 100);    // to move the window to position 100 100.
  cvMoveWindow("example_output2", 1200, 100);    // to move the window to position 100 100.
  
  // Show the original image
  cvShowImage("example_input", img);
  
  // Create an image for the output
  IplImage* out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
  IplImage* out2 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
  
  // Perform a Gaussian blur ( Convolving with 11 X 11 Gaussian).
  cvSmooth( img, out, CV_GAUSSIAN, 11, 11 );
  cvSmooth( img, out2, CV_GAUSSIAN, 21, 21 );
  
  // Show the processed image
  cvShowImage("example_output", out);
  cvShowImage("example_output2", out2);
  
  cvWaitKey(0);
  cvReleaseImage( &img );
  cvReleaseImage( &out );
  cvReleaseImage( &out2 );
  cvDestroyWindow( "example_input" );
  cvDestroyWindow( "example_output" );
  cvDestroyWindow( "example_output2" );
  return 0;
}
