#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>       /* pow */
#include <Windows.h> // sleep
#include "kmeansSegment.cpp"

// silly comment #1 #2

using namespace cv;
using namespace std;

//cv::namedWindow("myWindow", CV_WINDOW_NORMAL)
//cv::setWindowProperties("myWindow", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN)
//http://stackoverflow.com/questions/6512094/how-to-display-an-image-in-full-screen-borderless-window-in-opencv

int M_PI = 3.141459265359;

void showSingleColor(int use_kmeans);

Mat src; 
Mat kmeans_image;
Mat canvas_image; 
int g_photo_width = 512;
int g_photo_height = 512;
int g_colors = 20; // kmeans
int g_max_colors = 64;
int g_blur_loops = 0; // after doing a kmeans
int g_max_blur_loops = 264;
int g_gradient_blur_loops = 0; // when defining a gradient
int g_max_gradient_blur_loops = 64;

// for canny thresholding
int g_thresh = 100;
int g_max_thresh = 255;
int g_min_contour_length = 10;

int g_pass_num = 1; // used to define paint thickness (not used much now)
int g_line_width = 3; // initial value
int g_line_length = 6; // initial value

int g_color_distance = 0; // color picker error levels
int g_max_color_distance = 255;
int g_single_color_display_type = 0;
int g_paint_color[3] = {255,255,255};
Scalar g_default_canvas_color = cv::Scalar(0,255,0);

double *g_gradients;
RNG rng(12345);

// helper functions to move windows in a grid
int win_w (int n,int offset=0) { return 20+n*520+offset; }
int win_h (int n,int offset=0) { return 20+n*550+offset; }

// simple stroke
class LineStroke {
public:
  double length;   // length of a stroke
  double width;    // width of stroke
  double direction;  // radian
  int r,g,b;
  int i,j; // pixels
  double score; // how much improvement it makes
  int prev,next; // index of prev and next line stroke (default = -1 which means none)
  int start, end; // 1 = start / end of a multiple segment stroke
  int painted; // 1 = has been painted

  LineStroke() { // constructor
    r=0; g=0; b=0; i=0; j=0; score=0; direction=0; length=2; width=1; prev=-1; next=-1; start=1; end=1; painted=0;
  }
};

// returns a random color
//static Scalar randomColor( RNG& rng ) {
//  int icolor = (unsigned) rng;
//  return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
//}

// returns a number that estimates the difference between 2 images
// if canvas_mat is defined, then discount pixels in it that have the default canvas rgb
// canvas_mat is basically a mask... show regions that have yet to be painted
double calcDiffBetweenImages(Mat* mat1, Mat* mat2, Mat* canvas_mat=NULL) {
  double diff=0;

  double weight = 1.0; // how much to weigh a pixels 'difference'
  for (int i=0; i<mat1->cols; i++) {
    for (int j=0; j<mat2->rows; j++) {

      // discount "improving" places that have yet to be painted (this code shows limited benefit)
      // the goal is to avoid strokes that partially paint over other paint
      weight=1.0;
      if (canvas_mat) {
	Vec3b canvas_color = canvas_mat->at<Vec3b>(Point(i,j));
	if (canvas_color[0]==g_default_canvas_color[0] &&
	    canvas_color[1]==g_default_canvas_color[1] &&
	    canvas_color[2]==g_default_canvas_color[2]) { 
	  weight=.1; 
	}
      }

      Vec3b color1 = mat1->at<Vec3b>(Point(i,j));
      Vec3b color2 = mat2->at<Vec3b>(Point(i,j));

      double distance = sqrt((color1[0]-color2[0])*(color1[0]-color2[0]) + (color1[1]-color2[1])*(color1[1]-color2[1]) + (color1[2]-color2[2])*(color1[2]-color2[2]));
      diff += distance * weight;
      //double distance = cv::norm(color1-color2);
    }
  }
  return diff;
}

// creates an array of gradients for every pixel of an image.  grid_size=1,3,5,7 (for sobel operator)
void defineGradients(Mat* original, double gradArray[], int grid_size=3, int num_blurs=0) {
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
  
  for(int i = 0; i < grad_x.rows * grad_x.cols; i++)  {
    double directionRAD = atan2((double)pixelY[i], (double)pixelX[i]);
    //int directionDEG = (int)(180 + directionRAD / M_PI * 180);
    gradArray[i]=directionRAD;
    // if (i%1000==0) { printf("%5i: (%f,%f) => Rad: %5.3f, Deg: %5.1d\n",i, (double)pixelY[i], (double)pixelX[i],directionRAD,directionDEG); }
  }

  //  num_blurs = 1;
  if (num_blurs>0) {  // "tame" gradients
    // post process gradient to make each average
    double *new_gradients; // new blurred gradient
    new_gradients = new double[s.height * s.width];   

    for (int loop=0; loop<num_blurs; loop++) {
    
      for (int i=1; i<src.cols-1; i++) {
	for (int j=1; j<src.rows-1; j++) {
	
	  int index = s.width*j + i;

	  // add the 9 neighbors
	  int c=0;
	  double sumx=0;
	  double sumy=0;
	
	  for (int n=-1; n==1; n++) {
	    for (int m=-1; m==1; m++) {
	    
	      if (i+n >= 0 && i+n<s.width &&
		  j+m >= 0 && j+m<s.height && !(n==0 && m==0)) {
		c++;
		sumx += cos(gradArray[index + n * s.width + m]);
		sumy += sin(gradArray[index + n * s.width + m]);
	      }
	    }
	  }
	  if (c==0) { c=1; } // just for div zero bugs
	
	  double dx = 0.6 * cos(gradArray[index]) + 0.4 * sumx/c;
	  double dy = 0.6 * sin(gradArray[index]) + 0.4 * sumy/c;

	  //debug
	  //dx = cos(gradArray[index]);
	  //dy = sin(gradArray[index]);
	
	  new_gradients[index]=atan2(dy, dx);
	}
      }
    
      // copy back the blurred gradient
      for (int i = 0; i < grad_x.rows * grad_x.cols; i++) {
	  gradArray[i]=new_gradients[i];
      }
      //delete [] new_gradients;
    }
  }
}


// look at gradients
void showGradients() {
  Mat gradients_image = src.clone();

  Size s = gradients_image.size();
  double *gradients;

  gradients = new double[s.height * s.width];   
  defineGradients(&gradients_image,gradients,3,g_gradient_blur_loops);
  g_gradients = gradients;
  
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
    moveWindow("graident_window",win_w(3),win_h(0)); 
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
  moveWindow("lines_window",win_w(2),win_h(0)); 

}

// try to "paint" by lots of tiny brushstrokes in the right direction
void autoPaint(int candidate_multiplier=1) {
  Size s = src.size();
  Mat paint_image = Mat::zeros( s.height, s.width, CV_8UC3 );
  namedWindow( "paint_window", CV_WINDOW_AUTOSIZE );
  imshow( "paint_window", paint_image );
  moveWindow("paint_window",win_w(2),win_h(1)); 

  // brush stroke parameters
  int thickness=g_line_width;
  double line_length=g_line_length;
  int lineType=8; // always keep 8 for solid antialiased line

  int num_candidates=candidate_multiplier*src.rows*src.cols/(thickness*line_length/15);
  int num_strokes=0;

  for (int i=0; i<num_candidates; i++) {
    int x = rng.uniform(0,src.cols);
    int y = rng.uniform(0,src.rows);
    double rad = g_gradients[src.rows*y + x];
   
    // get paint brush pixel color (lets try kmeans one)
    Vec3b color = kmeans_image.at<Vec3b>(Point(x,y));

    rad=rad+M_PI/2; // rotate 90;
    double dx1=.2*line_length*cos(rad);
    double dy1=.2*line_length*sin(rad);
    double dx2=.8*line_length*cos(rad);
    double dy2=.8*line_length*sin(rad);

    if (0 && rad==M_PI/2) { //means no idea which way is gradient
      // lets draw using bright green just for fun...
      //cv::line( paint_image, Point(x-dy1,y-dy1), Point(x+dy2,y+dy2), Scalar(0, 255, 0), thickness, lineType );
    } else { 
      // save a region around where you are planning to paint
      cv::Rect roi = cv::Rect(fmax(0,x-2*line_length),fmax(0,y-2*line_length),
			      fmin(src.cols-x+line_length,3.5*line_length),
			      fmin(src.rows-y+line_length,3.5*line_length));
      Mat paintROI = paint_image(roi).clone();

      // paint a line
      cv::line( paint_image, Point(x-dx1,y-dy1), Point(x+dx2,y+dy2), Scalar( color[0], color[1], color[2] ), thickness, lineType );
      Mat newpaintROI = paint_image(roi).clone();

      // see if it's better than before
      Mat origimageROI = src(roi).clone();
      
      double origDiff = calcDiffBetweenImages(&origimageROI,&paintROI,&paintROI);
      double newDiff = calcDiffBetweenImages(&origimageROI,&newpaintROI,&paintROI);
      if (i%100==0) {
	printf("%d/%d: NEW:%f ORIG:%f",num_strokes, i, newDiff,origDiff);
      }

      // if not, revert back to the original
      if (newDiff>=origDiff) {

	int debug=0;
	if (debug) {
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

	  printf("GOING TO REVERT %d %d\n",x,y);
	  cv::rectangle( paint_image, Point(x-dx1,y-dy1), Point(x+dx2,y+dy2), Scalar( 255,0,0 ));
	  imshow( "paint_window", paint_image );
	  waitKey(0);
	}
	
	cv::Mat destinationROI = paint_image( roi );
	paintROI.copyTo( destinationROI );
	
	if (debug) {
	  printf("REVERTED\n");
	  imshow( "paint_window", paint_image );
	  waitKey(0);
	}

	if (i%100==0) { printf(" WORSE\n"); }
      } else {
	if (i%100==0) { printf(" BETTER\n"); }
	num_strokes++;
      }

      if (i%1000==0) {
	imshow( "paint_window", paint_image );
	waitKey(1);
      }
    }
  }
}

void line_callback(int, void* ) {
  printf("line width: %i, line lenght: %i\n", g_line_width, g_line_length);
}

void color_distance_callback(int, void* ) {
  printf("Color distance: %i\n",g_color_distance);
  showSingleColor(0);
}

void updateColorWindow(int r, int g, int b) 
{
  Mat color_image(240, 320, CV_8UC3); 
  color_image = Scalar(b,g,r);

  char text[50];
  sprintf(text,"RGB[%i,%i,%i]",r,g,b);
  putText(color_image, text, cvPoint(10,10), FONT_HERSHEY_DUPLEX, 0.3, cvScalar(0,0,0), 1, CV_AA);

  namedWindow( "color", CV_WINDOW_AUTOSIZE );
  imshow( "color", color_image );
  //moveWindow("color",win_w(0),win_h(2)); 

  createTrackbar( " Distance:", "color", &g_color_distance, g_max_color_distance, color_distance_callback );
  // set global paint color
  g_paint_color[0] = b;
  g_paint_color[1] = g;
  g_paint_color[2] = r;
  //createTrackbar( " Colors:", "color", &g_colors, g_max_colors, colors_callback );
}

// BEGIN PROJECTOR FUNCTIONS

int g_projector_calibrated = 0;
Mat g_projector_lambda( 2, 4, CV_32FC1 );
int g_projector_pixels_width = 1920;
int g_projector_pixels_height = 1000; //should be 1080... but reduce to have image header
Mat g_projector_image;

Point2f projectorInputQuad[4]; // Input Quadilateral or Image plane coordinates
Point2f projectorOutputQuad[4]; // Input Quadilateral or Image plane coordinates

void getProjectorQuadPointsFromFile() {
  ifstream infile; 
  infile.open("projector_4_points.txt"); 
  for (int i=0; i<4; i++) {
    infile >> projectorOutputQuad[i].x;
    infile >> projectorOutputQuad[i].y;
  }
  g_projector_lambda = getPerspectiveTransform( projectorInputQuad, projectorOutputQuad );
}

void calibrate_projector() {
  int w = src.cols; //g_photo_width;
  int h = src.rows; //g_photo_height;
  int offset_x = floor((g_projector_pixels_width - w)/2);
  int offset_y = floor((g_projector_pixels_height - h)/2);
  int g_projector_corner = 0;

  projectorInputQuad[0] = Point2f( offset_x, offset_y );
  projectorInputQuad[1] = Point2f( offset_x + w, offset_y);
  projectorInputQuad[2] = Point2f( offset_x + w, offset_y + h);
  projectorInputQuad[3] = Point2f( offset_x, offset_y + h ); 

  for (int i=0; i<4; i++) {
    projectorOutputQuad[i] = projectorInputQuad[i];
  }

  namedWindow( "projector_window", CV_WINDOW_AUTOSIZE );

  g_projector_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_8UC3);
  rectangle( g_projector_image, Point(offset_x,offset_y), Point(offset_x+w,offset_y+h), Scalar( 0, 55, 255 ), +1, 4 );

  float delta = 16.0;
  int projector_done=0;
  while (!projector_done)
    {
      Mat projector_corrected_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_8UC3);

      // Set the lambda matrix the same type and size as webcam
      g_projector_lambda = Mat::zeros( g_projector_image.rows, g_projector_image.cols, g_projector_image.type() );
		
      // Get the Perspective Transform Matrix i.e. lambda
      g_projector_lambda = getPerspectiveTransform( projectorInputQuad, projectorOutputQuad );

      // Apply the Perspective Transform for the webcam MIGHT NEED TEMP IMAGE
      warpPerspective(g_projector_image, projector_corrected_image, g_projector_lambda, projector_corrected_image.size() );

      imshow( "projector_window", projector_corrected_image );
	
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
	projectorOutputQuad[g_projector_corner].x += delta;
      } else if (k == 63234) { // left arrow
	printf("moving %i.x to left by %.3f\n",g_projector_corner,delta);
	projectorOutputQuad[g_projector_corner].x += -delta;
      } else if (k == 63232) { // up arrow
	printf("moving %i.y up by %.3f\n",g_projector_corner,delta);
	projectorOutputQuad[g_projector_corner].y += -delta;
      } else if (k == 63233) { // down arrow
	printf("moving %i.y down by %.3f\n",g_projector_corner,delta);
	projectorOutputQuad[g_projector_corner].y += delta;

      } else if (k == 43) { // +
	delta = delta * 2;
	printf("Changed delta to %.1f\n",delta);
      } else if (k == 45) { // -
	delta = delta * 0.5;
	printf("Changed delta to %.1f\n",delta);

      } else if (k == int('r')) { 
	printf("loading projector calibration quad points.\n");
	getProjectorQuadPointsFromFile();
      } else if (k == int('s')) { 
	printf("saving projector 4 points (for calibration matrix) to ...\n");
	ofstream outfile;
	outfile.open ("projector_4_points.txt");
	for (int i=0; i<4; i++) {
	  outfile << projectorOutputQuad[i].x << endl;
	  outfile << projectorOutputQuad[i].y << endl;
	}
	outfile.close();

      } else if (k == int('x')) { 
	projector_done=1;
      } else if (k == int('d')) { 
	projector_done=1;
	g_projector_calibrated = 1;
      }
      
    }
  printf("Projector calibration matrix defined.\n");
  print(g_projector_lambda);

}

// show projector window
void showProjectorWindow(Mat image) {
  g_projector_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_8UC3);

  int w = image.cols;
  int h = image.rows;
  int offset_x = floor((g_projector_pixels_width - w)/2);
  int offset_y = floor((g_projector_pixels_height - h)/2);

  image.copyTo(g_projector_image(Rect(offset_x,offset_y,w,h)));
  rectangle( g_projector_image, Point(offset_x,offset_y), Point(offset_x+w,offset_y+h), Scalar( 0, 55, 255 ), +1, 4 );

  namedWindow( "projector_window", CV_WINDOW_AUTOSIZE );
  if (g_projector_calibrated) {
    Mat projector_corrected_image = Mat::zeros(g_projector_pixels_height, g_projector_pixels_width, CV_8UC3);
    warpPerspective(g_projector_image, projector_corrected_image, g_projector_lambda, projector_corrected_image.size() );
    imshow( "projector_window", projector_corrected_image );
  } else {
    imshow( "projector_window", g_projector_image );
  }

}

// END PROJECTOR FUNCTIONS

// show the region where the picked color exists in the kmeans_image
void showSingleColor(int use_kmeans=1) {
  Size s = src.size();
  Mat single_color_image = Mat::zeros( s.height, s.width, CV_8UC3 );

  int min_diff=0;
  if (!use_kmeans) { min_diff=g_color_distance; }
  int n_pixels=0;

  Vec3b default_color;
  default_color[0]=255; default_color[1]=255; default_color[2]=255;
  if (g_single_color_display_type == 2) { // black on white background
    default_color[0]=0; default_color[1]=0; default_color[2]=0;
    single_color_image.setTo(cv::Scalar(255,255,255)); // redVal,greenVal,blueVal
  } 
  if (g_single_color_display_type == 3) { // green on white background
    default_color[0]=0; default_color[1]=255; default_color[2]=0;
    single_color_image.setTo(cv::Scalar(255,255,255)); // redVal,greenVal,blueVal
  } 
  if (g_single_color_display_type == 4) { // white on white background
    single_color_image.setTo(cv::Scalar(255,255,255)); // redVal,greenVal,blueVal
  } 

  for (int i=0; i<src.cols; i++) {
    for (int j=0; j<src.rows; j++) {
      Vec3b color;
      
      if (use_kmeans) {
	color = kmeans_image.at<Vec3b>(Point(i,j));
      } else {
	color = src.at<Vec3b>(Point(i,j));
      }
      int diff = sqrt((color[0]-g_paint_color[0])*(color[0]-g_paint_color[0]) + 
		      (color[1]-g_paint_color[1])*(color[1]-g_paint_color[1]) + 
		      (color[2]-g_paint_color[2])*(color[2]-g_paint_color[2]));
      //if (color[0] == g_paint_color[0] && color[1] == g_paint_color[1] && color[2] == g_paint_color[2]) {
      if (diff<=min_diff) {
	single_color_image.at<Vec3b>(Point(i,j)) = color; // set pixel      
	if (g_single_color_display_type > 0) {
	  single_color_image.at<Vec3b>(Point(i,j)) = default_color;
	} 

	n_pixels++;
      }
    }
  }
  printf("%d pixels found\n",n_pixels);
  
  namedWindow( "single_color_image_window", CV_WINDOW_AUTOSIZE );
  imshow( "single_color_image_window", single_color_image );
  moveWindow("single_color_image_window",win_w(0),win_h(1)); 

  ///ABC removed as not going to use a projector for now
  ///showProjectorWindow(single_color_image);
}


// try to "paint" by lots of tiny brushstrokes in the right direction for a single (globally defined) color
void singlePaintColorOnCanvas(int r=-1, int g=-1, int b=-1) {
  // if colors not defined, use global color
  if (r<0) { r=g_paint_color[2]; }
  if (g<0) { g=g_paint_color[1]; }
  if (b<0) { b=g_paint_color[0]; }

  // brush stroke parameters
  int thickness=g_line_width;
  double line_length=g_line_length;
  int lineType=8; // always keep 8 for solid antialiased line

  // get a list of valid pixels
  int n_pixels=0;

  //ABC: appears pixels definition was changed
  //int pixels[src.rows*src.cols][2];
  int** pixels = new int*[src.rows*src.cols];
  for (int i = 0; i < src.rows*src.cols; ++i) { pixels[i] = new int[2]; }

  for (int i=0; i<src.cols; i++) {
    for (int j=0; j<src.rows; j++) {
      Vec3b color = kmeans_image.at<Vec3b>(Point(i,j));
      if (color[0] == b && color[1] == g && color[2] == r) {
		pixels[n_pixels][0]=i;
		pixels[n_pixels][1]=j;
		n_pixels++;
      }
    }
  }

  // ABC: unknown why this was added as it breaks the rest of this function
  //for (int i = 0; i < src.rows*src.cols; ++i) {
	//  delete[] pixels[i];
  //}
  //delete[] pixels;

  printf("%d pixels found\n",n_pixels);
  
  // assume we're only going to test some of the
  int num_candidates=n_pixels/(thickness*line_length/15);
  printf("%d candidates found\n", num_candidates);

  // test all the candiates to see how they improve things alone
  LineStroke *strokes = new LineStroke[num_candidates];
  for (int i=0; i<num_candidates; i++) {
    int index = rng.uniform(0,n_pixels);
    int x = strokes[i].i = pixels[index][0];
    int y = strokes[i].j = pixels[index][1];
    strokes[i].direction = g_gradients[src.rows*y + x] + M_PI/2; // rotate 90;

    strokes[i].r = r;
    strokes[i].g = g;
    strokes[i].b = b;
    strokes[i].length = line_length;
    strokes[i].width = thickness;     
  
   // do quick score = count how many pixels of kmeans image in the stroke are the right color
   int c = 0;
   for (int n=-strokes[i].width; n<=strokes[i].width; n++) { // go along width
	   for (int m = -strokes[i].length; m <= strokes[i].length; m++) { // go along length
		 double cosine = cos(strokes[i].direction);
		 double sine = sin(strokes[i].direction);
		 int dx = floor(m * cosine - n * sine);
		 int dy = floor(m * sine + n * cosine);
		 int kx = x + dx;
		 int ky = y + dy;
   	     if (kx >= 0 && kx < src.cols && ky >= 0 && ky < src.rows) {
		   Vec3b color = kmeans_image.at<Vec3b>(Point(x + dx, y + dy));
		   if (color[0] == strokes[i].b && color[1] == strokes[i].g && color[2] == strokes[i].r) { c++; }
		 }
	  }
    }
    strokes[i].score = c;
  }

  struct compare_LineStroke_by_score {
    bool operator() (const LineStroke & lhs, const LineStroke & rhs) { return lhs.score > rhs.score; }
  };

  struct compare_LineStroke_by_location {
    bool operator() (const LineStroke & lhs, const LineStroke & rhs) { 
      double dlhs = lhs.i + 2 * lhs.j;
      double drhs = rhs.i + 2 * rhs.j;
      return dlhs > drhs;
    }
  };

  // sort by best candidates first 
  //for (int i=0; i<10; i++) { printf("CANDIDATE %i : %f\n",i,strokes[i].score); }
  std::sort(strokes, strokes+num_candidates, compare_LineStroke_by_score());
  //std::sort(strokes, strokes+num_candidates, compare_LineStroke_by_location());
  //for (int i=0; i<10; i++) { printf("SORT CANDIDATE %i : %f\n",i,strokes[i].score); }

  // connect all the strokes that have the end of one near the beginning of next
  printf("Connecting %i strokes\n", num_candidates);
  for (int i=0; i<=num_candidates; i++) {
    continue; // comment this out later for real

    if (strokes[i].painted == 0) { // unattached stroke... see if can attach
      strokes[i].painted = 1;

      int prev_index=i;
      int done=0;
      while (!done) {
	// calc the coordinate of the end of the previous stroke
	double endx = strokes[prev_index].i + strokes[prev_index].length * cos(strokes[prev_index].direction);
	double endy = strokes[prev_index].j + strokes[prev_index].length * sin(strokes[prev_index].direction);
	double width = strokes[prev_index].width;
	
	// find the best next stroke (combination of score and start of next stroke = end of this one)
	int best_index=-1; // none found
	double best_score=-9999999;
	for (int j=0; j<=num_candidates; j++) {
	  if (prev_index != j && strokes[j].painted == 0) { // a candidate to be the next one
	    double startx = strokes[j].i;
	    double starty = strokes[j].j;
	    
	    double distance = sqrt((startx - endx)*(startx - endx) + (starty - endy)*(starty - endy));
	    if (distance<width/3) { // inside the end of the stroke (assuming it ends in a circle)
	      double score = (width/3 - distance) * strokes[j].score;
	      if (score > best_score) {
		best_score = score;
		best_index = j;
	      }
	    }
	  }
	}

	if (best_index>-1) { // found a stroke to attach
	  strokes[prev_index].end = 0;
	  strokes[prev_index].next = best_index;
	  strokes[best_index].prev = prev_index;
	  strokes[best_index].painted = 1; // mark it used
	  prev_index = best_index; 
	
	} else { // didn't find a stroke to attach... so call this one an end
	  strokes[prev_index].end = 1;
	  done = 1;
	}
      }
    }
  }
  // order them so they are clustered
  //std::sort(strokes, strokes+num_candidates, compare_LineStroke_by_location());
  
  // apply the strokes
  printf("Painting strokes\n");

  int num_strokes_attempts=0;
  int num_strokes=0;
  for (int i=0; i<num_candidates; i++) { strokes[i].painted=0; }
  
  int done=0;
  int this_index = -1;
  while (!done) {
    // find stroke 
    if (this_index == -1) { // first loop
      this_index = 0; // pick the first one
    } else {
      // find the closest unpainted stroke that is nearest from where you're at.
      double min_distance=99999999999999;
      double min_index=-1;
      double endx = strokes[this_index].i + strokes[this_index].length * cos(strokes[this_index].direction);
      double endy = strokes[this_index].j + strokes[this_index].length * sin(strokes[this_index].direction);
      for (int i=0; i<num_candidates; i++) {
	if (strokes[i].painted == 0 && strokes[i].prev == -1) { // hasn't been painted yet and it's a start of a stroke
	  double startx = strokes[i].i;
	  double starty = strokes[i].j;
	  double distance = sqrt((startx - endx)*(startx - endx) + (starty - endy)*(starty - endy));
	  if (distance < min_distance) {
	    min_distance=distance;
	    min_index = i; 
	  }
	}
      }
      if (min_index == -1) { done=1; }
      this_index = min_index;
    }
  
    if (!done) {
      int stroke_done=0;
      while (!stroke_done) {
	num_strokes_attempts++;

	// paint this stroke
	int x = strokes[this_index].i;
	int y = strokes[this_index].j;

	//printf("PAINTING stroke #%i (%i,%i) ... next is %i\n",this_index , x, y, strokes[this_index].next);

	// check to see if it's the right color... mostly for debug
	Vec3b color = kmeans_image.at<Vec3b>(Point(x,y));
	if (color[0] != b || color[1] != g || color[2] != r) { 
	  printf("HEY...wrong color [%i,%i,%i] not [%i,%i,%i] index:%i next:%i x,y=%i,%i\n",r,b,g,color[2],color[1],color[0],this_index,strokes[this_index].next, x,y); stroke_done=1; continue; 
	}
	
	//double rad = g_gradients[src.rows*y + x];
	double rad = strokes[this_index].direction;
	
	double dx1=.2*line_length*cos(rad);
	double dy1=.2*line_length*sin(rad);
	double dx2=.8*line_length*cos(rad);
	double dy2=.8*line_length*sin(rad);
	
	if (0 && rad==M_PI/2) { //means no idea which way is gradient
	  //cv::line( paint_image, Point(x-dy1,y-dy1), Point(x+dy2,y+dy2), Scalar(0, 255, 0), thickness, lineType );
	} else { 
	  // save a region around where you are planning to paint
	  cv::Rect roi = cv::Rect(fmax(0,x-1.5*line_length),fmax(0,y-1.5*line_length),
				  fmin(src.cols-x+line_length,3*line_length),
				  fmin(src.rows-y+line_length,3*line_length));
	  //roi = cv::Rect(0,0,src.cols,src.rows);
	  Mat paintROI = canvas_image(roi).clone();
	  
	  // paint a line
	  cv::line( canvas_image, Point(x-dx1,y-dy1), Point(x+dx2,y+dy2), 
		    Scalar( g_paint_color[0], g_paint_color[1], g_paint_color[2] ), thickness, lineType );
	  Mat newpaintROI = canvas_image(roi).clone();
	  
	  // see if it's better than before
	  Mat origimageROI = src(roi).clone();
	  double origDiff = calcDiffBetweenImages(&origimageROI,&paintROI,&paintROI);
	  double newDiff = calcDiffBetweenImages(&origimageROI,&newpaintROI,&paintROI);
	  if (num_strokes_attempts % 1000==0) { printf("%d/%d: NEW:%f ORIG:%f",num_strokes, num_strokes_attempts, newDiff,origDiff); }
	  
	  // if not, revert back to the original
	  if (newDiff>=origDiff) {
	    cv::Mat destinationROI = canvas_image( roi );
	    paintROI.copyTo( destinationROI );
	    if (num_strokes_attempts % 1000==0) { printf(" WORSE\n"); }
	  } else {
	    if (num_strokes_attempts % 1000==0) { printf(" BETTER\n"); }
	    num_strokes++;
	  }
	  
	  if (num_strokes % 1000 == 0) {
	    imshow( "canvas_window", canvas_image );
	    waitKey(1);
	  }
	}
	strokes[this_index].painted = 1;

	// check for next stroke
	if (strokes[this_index].next >= 0) { // move to the next stroke in the series
	  this_index = strokes[this_index].next;

	} else { // no more to paint in this series
	  stroke_done = 1;
	}
      }
    }

  }
  printf("Done painting strokes\n");
  imshow( "canvas_window", canvas_image );
  waitKey(1);

  delete [] strokes;
  //delete pixels;
}

// given an image, return a sorted list of colors (darkest first)
void sortColorsInImage(Mat *image, std::vector<Vec3b>& sorted_colors) {
  Vec3b colors[2048]; // max number of colors
  long colors_count[2048] = {0};
  int n_colors=0;

  // scan image and count colors
  for (int i=0; i<image->cols; i++) {
    for (int j=0; j<image->rows; j++) {
      int found=-1;
      Vec3b color = kmeans_image.at<Vec3b>(Point(i,j));
      for (int k=0; k<n_colors; k++) {
	if (color[0] == colors[k][0] && color[1] == colors[k][1] && color[2] == colors[k][2]) {
	  found=k;
	  colors_count[k]++;
	  continue;
	}
      }
      if (found == -1) {
	colors[n_colors]=color;
	n_colors++;
      }
    }
  }  

  for (int i=0; i<n_colors; i++) {
    //printf("%d: [%d,%d,%d] = %ld NORM:%f\n",i,colors[i][0],colors[i][1],colors[i][2],colors_count[i],norm(colors[i]));
  }

  // create a sorted list
  int* used_colors = new int [n_colors]; // don't forget to delete this later
  for (int i = 0; i < n_colors; i++) { used_colors[i]=0; }

  for (int i=0; i<n_colors; i++) {
    // find the darkest unused color
    int darkest=255*255*255;
    int darkest_index=-1;
    for (int j=0; j<n_colors; j++) {
      if (!used_colors[j] && norm(colors[j])<=darkest) {
	darkest=norm(colors[j]);
	darkest_index=j;
      }
    }
    if (darkest_index>=0) { // should always be true
      used_colors[darkest_index]=1;
      sorted_colors.push_back(colors[darkest_index]);
    }
  }
  delete [] used_colors;

  for (int i=0; i<n_colors; i++) {
    printf("SORTED %d: [%d,%d,%d] = NORM:%f\n",i,sorted_colors[i][0],sorted_colors[i][1],sorted_colors[i][2],norm(sorted_colors[i]));
  }
}

void paintByIncreasedLightnessOfColors() {
  std::vector<Vec3b> colors; // uninitialized.  Let sortColorsInImage fill it.
  
  sortColorsInImage(&kmeans_image,colors);

  int do_wait=1;
  for (int i=0; i<colors.size(); i++) {
    printf("Painting color #%d: [%d,%d,%d] = brightness norm:%f\n",i,colors[i][0],colors[i][1],colors[i][2],norm(colors[i]));
    //g_paint_color[0] = colors[i][0];
    //g_paint_color[1] = colors[i][1];
    //g_paint_color[2] = colors[i][2];
	printf("painting %i A (%i total)\n", i, colors.size());
	updateColorWindow(colors[i][2], colors[i][1], colors[i][0]); // sets global values
    showSingleColor();
	printf("painting %i B\n", i);
    singlePaintColorOnCanvas(); 
	printf("painting %i C\n", i);
	if (do_wait) {
      int k = waitKey(0);
      if (k==int('i')) { do_wait=0; } // just zip through all the colors
    }
  }
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
    showSingleColor(1);
    Sleep(2000);

  } else if  ( event == EVENT_RBUTTONDOWN ) {
    cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if  ( event == EVENT_MBUTTONDOWN ) {
    cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  } else if ( event == EVENT_MOUSEMOVE ) {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    int b= kmeans_image.at<Vec3b>(y,x)[0];
    int g= kmeans_image.at<Vec3b>(y,x)[1];
    int r= kmeans_image.at<Vec3b>(y,x)[2];
    cout << "kmeans R(" << r << ")" << "G(" << g << ")" << "B(" << b << ")" << endl;
    updateColorWindow(r,g,b);
    showSingleColor(1);
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
    showSingleColor(0);
  }
}

// handle mouse clicks in canvas window
void canvasMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    int b= kmeans_image.at<Vec3b>(y,x)[0];
    int g= kmeans_image.at<Vec3b>(y,x)[1];
    int r= kmeans_image.at<Vec3b>(y,x)[2];
    cout << "canvas R(" << r << ")" << "G(" << g << ")" << "B(" << b << ")" << endl;
    updateColorWindow(r,g,b);
    showSingleColor(0);
  }
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  Mat src_gray;
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Detect edges using canny
  Canny( src_gray, canny_output, g_thresh, g_thresh*2, 3 );
  
  /// Show in a window
  namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
  moveWindow("Canny",700,600);
  imshow( "Canny", canny_output );

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  // only finds the most external contours
  //findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

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

    Scalar color = Scalar(255,255,255);
    if (g_single_color_display_type % 2 == 0) { color = Scalar(0,255,0);}

    g_min_contour_length = 0;
    if (contours_poly[i].size()>g_min_contour_length) {
      num_drawn++;
      drawContours( drawing, contours_poly, i, color, 1, 8, hierarchy, 0, Point() );      
    }

    // test printing out some contour points
    if (i==3) {
      for (int j=0; j<contours_poly[i].size();j++) {
	//cout << contours[i][j] << endl; //do whatever
      }
    }
  }

  //char text[50];
  //sprintf(text,"Lines: drew %i out of %lu",num_drawn,contours.size());
  //putText(drawing, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);
  printf("Lines: drew %i out of %lu",num_drawn,contours.size());

  /// Show in a window
  //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  //moveWindow("Contours",1250,600);
  //imshow( "Contours", drawing );
  showProjectorWindow(drawing);
 
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

  // blur and try again... perhaps will make the transitionsmore smooth
  //result = adaptiveThreshold(result,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2)
  if (g_blur_loops) {
    for (int i=0; i<2; i++) {
      blur( kmeans_image, kmeans_image, Size(3,3) );
      kmeans_image = kmeans.segment(kmeans_image);
    }
  }

  printf("Colors:%i Blur Loops:%i\n",g_colors,g_blur_loops);
  //char text[50];
  //sprintf(text,"Colors:%i Blur Loops:%i",g_colors,g_blur_loops);
  //putText(kmeans_image, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 1, CV_AA);

  namedWindow( "kmeans_window", CV_WINDOW_AUTOSIZE );
  imshow( "kmeans_window", kmeans_image );
  moveWindow("kmeans_window",win_w(1),win_h(0));

  std::vector<Vec3b> colors; // uninitialized.  Let sortColorsInImage fill it.
  sortColorsInImage(&kmeans_image,colors); // prints out which colors are in image
  showGradients(); 
}

/// WEBCAM FUNCTIONS ///////////////////////

int g_webcam_calibrated = 0;
int g_flip_webcam = 1; 
int g_webcamID = 1;
int g_webcam_corner = 0;	
int g_canvas_pixels_width = 1000;
int g_canvas_pixels_height = 800;
Point2f webcamQuad[4]; // Input Quadilateral or Image plane coordinates
Point2f canvasCornersQuad[4]; // Output Quadilateral or World plane coordinates

// Lambda Matrix
Mat webcam_lambda( 2, 4, CV_32FC1 );

// handle mouse clicks in the webcam window
void webcamWindowMouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN ) {
    printf("Setting canvas corner %i to %i, %i\n",g_webcam_corner+1, x, y);
    webcamQuad[g_webcam_corner] = Point2f( x,y );
  }
}

void calibrate_webcam () {
  VideoCapture cap0(g_webcamID); // open the default camera
  if(!cap0.isOpened()) {  // check if we succeeded
    printf("Camera0 not open\n");
    return;
  }

  VideoCapture cap1(1); // open the default camera
  if(!cap1.isOpened()) {  // check if we succeeded
    printf("Camera1 not open\n");
    return;
  }
  Mat webcam1;
  cap1 >> webcam1; // get a new frame from camera
  namedWindow("webcam1",1);
  imshow("webcam1", webcam1);
  
  namedWindow("webcam0",1);
  moveWindow("webcam0",100,30);
  setMouseCallback("webcam0", webcamWindowMouseCallBackFunc, NULL);


  namedWindow("webcam corrected",1);
  moveWindow("webcam corrected",100,300);
             
  Mat webcam;
  cap0 >> webcam; // get a new frame from camera

  // this is the "canvas"
  Mat webcam_corrected = Mat::zeros(g_canvas_pixels_height, g_canvas_pixels_width, webcam.type() );

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  webcamQuad[0] = Point2f( 0,0 );
  webcamQuad[1] = Point2f( webcam.cols-0,0);
  webcamQuad[2] = Point2f( webcam.cols-0,webcam.rows-0);
  webcamQuad[3] = Point2f( 0,webcam.rows-0  ); 
  printf("Input is %i by %i\n",webcam.cols,webcam.rows);
  printf("Click on upper left corner of canvas (press 1,2,3,4 to pick corner)\n");
  printf("Click 'd' when done.  Click 'x' to cancel.\n");

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
      for (int i=0; i<3; i++) {
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
      } else if (k == int('d')) { 
	webcam_done=1;
	g_webcam_calibrated = 1;
      }
      if (k>0) { printf("K:%d\n",k); }
      
    }
  printf("Webcam calibration matrix defined.\n");
  print(webcam_lambda);
}

/// END OF WEBCAM FUNCTIONS ///////////////////


/** @function main */
int main( int argc, char** argv )
{
  printf("colors <image_filename>\n");
  printf("Commands:\n");
  printf("  p = autopaint, P = autopaint (3x strokes) \n");
  printf("  n = create/reset a fresh (green) canvas \n");
  printf("  i = paint starting with darkest ('space'=interate,'i' again=do all in order \n");
  printf("  s = paint only a single color in the canvas (choose color by clicking in any window) \n");
  printf("  c = show where single color on the kmeans image (choose color by clicking in any window) \n");

  char* image = "images/eggs-512.jpg"; // lena.jpg";
  if (argv[1]) {
    image = argv[1];
  }

  /// Load source image and convert it to gray
  src = imread( image, 1 );
  if (src.empty()) {
    std::cout << "!!! Failed imread()\n ./colors <image filename>" << std::endl;
    return -1;
  }  

  //g_photo_width = src.cols;
  //g_photo_height = src.rows;
  //if (argv[2]) { g_photo_width = atoi(argv[2]); }
  //  if (argv[3]) { g_photo_height = atoi(argv[3]); }
  printf("Photo width = %i\n", g_photo_width); 
  printf("Photo height = %i\n", g_photo_height); 

  std::cout << CV_VERSION << std::endl;

  /// Create Window
  char source_window[] = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  //   setWindowProperty(source_window, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN); //#FULLSCREEN)
   setWindowProperty(source_window, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN); //#FULLSCREEN)

  moveWindow(source_window,win_w(0),win_h(0,-30));
  imshow( source_window, src );

  namedWindow( "controls", CV_WINDOW_AUTOSIZE );
  createTrackbar( " Canny thresh:", "controls", &g_thresh, g_max_thresh, thresh_callback );
  createTrackbar( " Blur loops:", "controls", &g_blur_loops, g_max_blur_loops, colors_callback ); 
  createTrackbar( " Colors:", "controls", &g_colors, g_max_colors, colors_callback );
  createTrackbar( " Gradient blur loops:", "controls", &g_gradient_blur_loops, g_max_gradient_blur_loops, colors_callback );
  createTrackbar( " Line width:", "controls", &g_line_width, 20, line_callback );
  createTrackbar( " Line length:", "controls", &g_line_length, 40, line_callback );

  thresh_callback( 0, 0 );
  colors_callback( 0, 0 );

  //set the callback function for any mouse event
  setMouseCallback("kmeans_window", kmeansMouseCallBackFunc, NULL);
  setMouseCallback(source_window, sourceMouseCallBackFunc, NULL);

  // create the default canvas
  Size s = src.size();
  canvas_image = Mat::zeros( s.height, s.width, CV_8UC3 );
  canvas_image.setTo(g_default_canvas_color); // redVal,greenVal,blueVal
  namedWindow( "canvas_window", CV_WINDOW_AUTOSIZE );
  imshow( "canvas_window", canvas_image );
  moveWindow("canvas_window",win_w(1),win_h(1)); 
  setMouseCallback("canvas_window", canvasMouseCallBackFunc, NULL);

  while (1) {

    if (g_webcam_calibrated) {
      // continue to show the webcam?
    }

    int k = waitKey(33);
    if (k==27) { // Esc key to stop
      return(0);
    } else if (k==-1) { //  normally -1 returned,so don't print it
    } else if (k == int('p')) { 
	autoPaint(1);
    } else if (k == int('P')) { 
	autoPaint(3);
    } else if (k == int('i')) { 
      paintByIncreasedLightnessOfColors();
      g_pass_num++;

    } else if (k == int('n')) { // reset canvas
      canvas_image.setTo(g_default_canvas_color); // redVal,greenVal,blueVal
  
    } else if (k == int('s')) { // paint one color on canvas
      singlePaintColorOnCanvas();
    } else if (k == int('c')) { // show where single color exists on kmeans image
      showSingleColor(1);

    } else if (k == int('W')) { // calibrate webcam
      calibrate_webcam();
    } else if (k == int('w')) { // calibrate projector
      calibrate_projector();

    } else if (k == 32) { // space bar
      printf("Setting single color display type to %i\n",g_single_color_display_type);
      g_single_color_display_type++;
      if (g_single_color_display_type>4) { g_single_color_display_type=0; }
      showSingleColor();

    } else {
    printf("K:%i\n",k);
      //print k # else print its value
      //Upkey : 2490368
      //DownKey : 2621440
      //LeftKey : 2424832
      //RightKey: 2555904
      //Space : 32
      //Delete : 3014656
    }
  }

  return(0);
}

