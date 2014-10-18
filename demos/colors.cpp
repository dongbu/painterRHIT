#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "kmeansSegment.cpp"

using namespace cv;
using namespace std;

Mat src; 
Mat kmeans_image;
int g_colors = 10; // kmeans
int g_max_colors = 64;
int g_blur_loops = 0;
int g_max_blur_loops = 64;
int g_color_distance = 0; // color picker error levels
int g_max_color_distance = 255;
RNG rng(12345);


// returns a random color
static Scalar randomColor( RNG& rng ) {
  int icolor = (unsigned) rng;
  return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

// creates an array of gradients for every pixel of an image.  grid_size=1,3,5,7 (for sobel operator)
void getGradients(Mat* original, double gradArray[], int grid_size=3) {
  Mat original_Mat = original->clone();
  
  // Convert it to gray
  cvtColor( original_Mat, original_Mat, CV_RGB2GRAY );
  //blur(original_Mat, original_Mat, Size(7,7));
  
  /// Generate grad_x and grad_y
  Size s = original_Mat.size();
  Mat grad_x = Mat::zeros(s.height, s.width, CV_16S); 
  Mat grad_y = Mat::zeros(s.height, s.width, CV_16S);
  
  /// Gradient X
  Sobel(original_Mat, grad_x, CV_16S, 1, 0, grid_size);
  
  /// Gradient Y
  Sobel(original_Mat, grad_y, CV_16S, 0, 1, grid_size);
  
  short* pixelX = grad_x.ptr<short>(0);
  short* pixelY = grad_y.ptr<short>(0);
  
  for(int i = 0; i < grad_x.rows * grad_x.cols; i++) 
    {
      double directionRAD = atan2((double)pixelY[i], (double)pixelX[i]);
      //int directionDEG = (int)(180 + directionRAD / M_PI * 180);
      gradArray[i]=directionRAD;
      
      if (i%1000==0) {
	//printf("%5i: (%f,%f) => Rad: %5.3f, Deg: %5.1d\n",i, (double)pixelY[i], (double)pixelX[i],directionRAD,directionDEG);
      }
    }
}

// returns a number that estimates the difference between 2 images
double calcDiffBetweenImages(Mat* mat1, Mat* mat2) {
  double diff=0;
  for (int i=0; i<mat1->cols; i++) {
    for (int j=0; j<mat2->rows; j++) {
      Vec3b color1 = mat1->at<Vec3b>(Point(i,j));
      Vec3b color2 = mat2->at<Vec3b>(Point(i,j));

      //cv::Vec3b d = color1-color2; // Vec4d?
      //double distance = cv::norm(d);
      double distance = sqrt((color1[0]-color2[0])*(color1[0]-color2[0]) + (color1[1]-color2[1])*(color1[1]-color2[1]) + (color1[2]-color2[2])*(color1[2]-color2[2]));
      diff += distance;
    }
  }
  return diff;
}


// look at gradients
void showGradients() {
  Mat gradients_image = src.clone();

  Size s = gradients_image.size();
  double gradients[s.height * s.width]; 

  // gradients will be in radians
  getGradients(&gradients_image,gradients,3);

  // perhaps post process gradient to make each value the median of it's 9-neighbors

  // show the gradients on the image (debug)
  if (1) {
    for (int i=0; i<src.cols; i++) {
      for (int j=0; j<src.rows; j++) {
	Vec3b color = gradients_image.at<Vec3b>(Point(i,j));
	double rad = gradients[src.rows*j + i];
	color[0]=255*(rad+M_PI)/(2*M_PI);
	color[1]=255*(rad+M_PI)/(2*M_PI);
	color[2]=255*(rad+M_PI)/(2*M_PI);    
	gradients_image.at<Vec3b>(Point(i,j)) = color; // set pixel      
      }
    }
  
    int num_lines=src.rows*src.cols/100;
    for (int i=0; i<num_lines; i++) {
      int x = rng.uniform(0,src.cols);
      int y = rng.uniform(0,src.rows);  
      double rad = gradients[src.rows*y + x];
      
      int thickness=1;
      int lineType=8;
      if (rad>0) { // 0 means no idea which way is gradient
	rad=rad*M_PI/2; // rotate 90;
	cv::line( gradients_image, Point(x,y), Point(x+5*cos(rad),y+5*sin(rad)), Scalar( 0, 0, 0 ), thickness, lineType );
      }
    }

    namedWindow( "gradient_window", CV_WINDOW_AUTOSIZE );
    imshow( "graident_window", gradients_image );
    moveWindow("graident_window",1000,100); 
  }

  // try to "paint" by lots of lines in the right direction
  Mat lines_image = Mat::zeros( s.height, s.width, CV_8UC3 );
  int num_lines=src.rows*src.cols/10;
  for (int i=0; i<num_lines; i++) {
    int x = rng.uniform(0,src.cols);
    int y = rng.uniform(0,src.rows);
    double rad = gradients[src.rows*y + x];
   
    // get pixel color (lets try kmeans one)
    Vec3b color = kmeans_image.at<Vec3b>(Point(x,y));

    int thickness=1; // change to 1 to see graident
    int lineType=8;
    rad=rad+M_PI/2; // rotate 90;
    double dx=2.5*cos(rad);
    double dy=2.5*sin(rad);

    if (rad==M_PI/2) { //means no idea which way is gradient
      // lets draw using bright green just for fun...
      //cv::line( lines_image, Point(x-dx,y-dy), Point(x+dx,y+dy), Scalar(0, 255, 0), thickness, lineType );
    } else { 
      cv::line( lines_image, Point(x-dx,y-dy), Point(x+3*dx,y+3*dy), Scalar( color[0], color[1], color[2] ), thickness, lineType );
    }
  }

  namedWindow( "lines_window", CV_WINDOW_AUTOSIZE );
  imshow( "lines_window", lines_image );
  moveWindow("lines_window",1000,700); 


  // try to "paint" by lots of lines in the right direction
  Mat paint_image = Mat::zeros( s.height, s.width, CV_8UC3 );
  namedWindow( "paint_window", CV_WINDOW_AUTOSIZE );
  imshow( "paint_window", paint_image );
  moveWindow("paint_window",1000,700); 

  num_lines=src.rows*src.cols/10;
  int num_strokes=0;

  for (int i=0; i<num_lines; i++) {
    int x = rng.uniform(0,src.cols);
    int y = rng.uniform(0,src.rows);
    double rad = gradients[src.rows*y + x];
   
    // get paint brush pixel color (lets try kmeans one)
    Vec3b color = kmeans_image.at<Vec3b>(Point(x,y));

    int thickness=5; 
    int lineType=8;
    rad=rad+M_PI/2; // rotate 90;
    double line_length=10;
    double dx1=.2*line_length*cos(rad);
    double dy1=.2*line_length*sin(rad);
    double dx2=.8*line_length*cos(rad);
    double dy2=.8*line_length*sin(rad);

    if (0 && rad==M_PI/2) { //means no idea which way is gradient
      // lets draw using bright green just for fun...
      //cv::line( paint_image, Point(x-dy1,y-dy1), Point(x+dy2,y+dy2), Scalar(0, 255, 0), thickness, lineType );
    } else { 
      // save a region around where you are planning to paint
      cv::Rect roi = cv::Rect(fmax(0,x-line_length),fmax(0,y-line_length),
			      fmin(src.cols-x+line_length,2*line_length),
			      fmin(src.rows-y+line_length,2*line_length));
      Mat paintROI= paint_image(roi).clone();

      // paint a line
      cv::line( paint_image, Point(x-dx1,y-dy1), Point(x+dx2,y+dy2), Scalar( color[0], color[1], color[2] ), thickness, lineType );
      Mat newpaintROI= paint_image(roi).clone();

      // see if it's better than before
      Mat origimageROI= src(roi).clone();
       
      double origDiff = calcDiffBetweenImages(&origimageROI,&paintROI);
      double newDiff = calcDiffBetweenImages(&origimageROI,&newpaintROI);
      if (i%100==0) {
	printf("%d/%d: NEW:%f ORIG:%f",num_strokes, i, newDiff,origDiff);
      }

      // if not, revert back to the original
      if (newDiff>=origDiff) {

	if (0) {
	  namedWindow( "paintROI", CV_WINDOW_AUTOSIZE );
	  imshow( "paintROI", paintROI );
	  moveWindow("paintROI",1500,100); 
	  
	  namedWindow( "newpaintROI", CV_WINDOW_AUTOSIZE );
	  imshow( "newpaintROI", newpaintROI );
	  moveWindow("newpaintROI",1500,150); 
	  
	  namedWindow( "srcROI", CV_WINDOW_AUTOSIZE );
	  imshow( "srcROI", origimageROI );
	  moveWindow("srcROI",1500,200); 
	  //waitKey(0);
	}

	//printf("GOING TO REVERT %f %f\n",x,y);
	cv::rectangle( paint_image, Point(x-dx1,y-dy1), Point(x+dx2,y+dy2), Scalar( 255,0,0 ));
	//imshow( "paint_window", paint_image );
	//waitKey(0);

	cv::Mat destinationROI = paint_image( roi );
	paintROI.copyTo( destinationROI );

	//printf("REVERTED\n");
	//imshow( "paint_window", paint_image );
	//waitKey(0);
	if (i%100==0) { printf(" WORSE\n"); }
      } else {
	if (i%100==0) { printf(" BETTER\n"); }
	num_strokes++;
      }

      imshow( "paint_window", paint_image );
      waitKey(1);
    }
  }

}


/** @function color_distance_callback */
void color_distance_callback(int, void* ) {
    cout << "color distance" << g_color_distance << endl;
}

void updateColorWindow(int r, int g, int b) 
{
  Mat color_image(240, 320, CV_8UC3); 
  color_image = Scalar(b,g,r);
  namedWindow( "color", CV_WINDOW_AUTOSIZE );
  imshow( "color", color_image );
  createTrackbar( " Distance:", "color", &g_color_distance, g_max_color_distance, color_distance_callback );
  //createTrackbar( " Colors:", "color", &g_colors, g_max_colors, colors_callback );
}


// handle mouse clicks in kmeans
void kmeansMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

    int b= kmeans_image.at<Vec3b>(y,x)[0];
    int g= kmeans_image.at<Vec3b>(y,x)[1];
    int r= kmeans_image.at<Vec3b>(y,x)[2];
    cout << "kmeans R(" << r << ")" << "G(" << g << ")" << "B(" << b << ")" << endl;
    updateColorWindow(r,g,b);

  } else if  ( event == EVENT_RBUTTONDOWN ) {
    cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if  ( event == EVENT_MBUTTONDOWN ) {
    cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if ( event == EVENT_MOUSEMOVE ) {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
  }
}

// handle mouse clicks in source window
void sourceMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

    int b= src.at<Vec3b>(y,x)[0];
    int g= src.at<Vec3b>(y,x)[1];
    int r= src.at<Vec3b>(y,x)[2];
    cout << "source R(" << r << ")" << "G(" << g << ")" << "B(" << b << ")" << endl;
    updateColorWindow(r,g,b);
  }
}

/** @function colors_callback */
void colors_callback(int, void* )
{
  kmeansSegment kmeans(g_colors);
  //Mat result = kmeans.segment(src);

  // KMEANS_RANDOM_CENTERS Select random initial centers in each attempt.
  // KMEANS_PP_CENTERS Use kmeans++ center initialization by Arthur and Vassilvitskii [Arthur2007].
  // KMEANS_USE_INITIAL_LABELS During the first (and possibly the only) attempt,
  ///kmeans.set_criteria(KMEANS_RANDOM_CENTERS);

  kmeans_image = kmeans.segment(src);

  char text[50];

  // blur and try again... perhaps will make the transitionsmore smooth
  //result = adaptiveThreshold(result,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2)
  if (g_blur_loops) {
    for (int i=0; i<5; i++) {
      blur( kmeans_image, kmeans_image, Size(3,3) );
      kmeans_image = kmeans.segment(kmeans_image);
    }
  }

  sprintf(text,"Colors:%i Blur Loops:%i",g_colors,g_blur_loops);
  putText(kmeans_image, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 1, CV_AA);

  namedWindow( "kmeans_window", CV_WINDOW_AUTOSIZE );
  imshow( "kmeans_window", kmeans_image );
  moveWindow("kmeans_window",650,100);

  showGradients();
  //cvtColor( result, src_gray, CV_BGR2GRAY );
}


/** @function main */
int main( int argc, char** argv )
{
  char* image="images/lena.jpg";
  if (argv[1]) {
    image = argv[1];
  }

  /// Load source image and convert it to gray
  src = imread( image, 1 );
  if (src.empty()) {
    std::cout << "!!! Failed imread()\n ./colors <image filename>" << std::endl;
    return -1;
  }  

  /// Convert image to gray and blur it
  //cvtColor( src, src_gray, CV_BGR2GRAY );
  //blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char source_window[] = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  moveWindow(source_window,100,100);
  imshow( source_window, src );

  createTrackbar( " Blur loops:", source_window, &g_blur_loops, g_max_blur_loops, colors_callback );
  createTrackbar( " Colors:", source_window, &g_colors, g_max_colors, colors_callback );
  colors_callback( 0, 0 );

  //set the callback function for any mouse event
  setMouseCallback("kmeans_window", kmeansMouseCallBackFunc, NULL);
  setMouseCallback(source_window, sourceMouseCallBackFunc, NULL);

  waitKey(0);
  return(0);
}

