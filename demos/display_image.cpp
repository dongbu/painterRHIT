#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{

  if ( argc != 2 ) {
    printf( "$0 <imagename> \n" );
    return -1;
  }

  Mat image = imread( argv[1], 1 );

  if (!image.data ) {
    printf( "No image data \n" );
    return -1;
  }

  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", image );

  waitKey(0);

  return 0;
}
