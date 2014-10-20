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
Mat canvas_image; 
int g_colors = 20; // kmeans
int g_max_colors = 64;
int g_blur_loops = 0;
int g_max_blur_loops = 64;
int g_color_distance = 0; // color picker error levels
int g_max_color_distance = 255;
int g_paint_color[3] = {255,255,255};
double *g_gradients;
RNG rng(12345);

// helper functions to move windows in a grid
int win_w (int n,int offset=0) { return 20+n*520+offset; }
int win_h (int n,int offset=0) { return 20+n*550+offset; }

// returns a random color
static Scalar randomColor( RNG& rng ) {
  int icolor = (unsigned) rng;
  return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

// creates an array of gradients for every pixel of an image.  grid_size=1,3,5,7 (for sobel operator)
void defineGradients(Mat* original, double gradArray[], int grid_size=3) {
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
  double *gradients;

  gradients = new double[s.height * s.width];   
  defineGradients(&gradients_image,gradients,3);
  g_gradients = gradients;
  
  // perhaps post process gradient to make each value the median of it's 9-neighbors
  for (int i=1; i<src.cols-1; i++) {
    for (int j=1; j<src.rows-1; j++) {
      
    }
  }

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
  int thickness=3;  //6
  double line_length=6; // 12
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
      
      double origDiff = calcDiffBetweenImages(&origimageROI,&paintROI);
      double newDiff = calcDiffBetweenImages(&origimageROI,&newpaintROI);
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

/** @function color_distance_callback */
void color_distance_callback(int, void* ) {
    cout << "color distance" << g_color_distance << endl;
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
  moveWindow("color",win_w(0),win_h(2)); 

  createTrackbar( " Distance:", "color", &g_color_distance, g_max_color_distance, color_distance_callback );
  // set global paint color
  g_paint_color[0] = b;
  g_paint_color[1] = g;
  g_paint_color[2] = r;
  //createTrackbar( " Colors:", "color", &g_colors, g_max_colors, colors_callback );
}

// show the region where the picked color exists in the kmeans_image
void showSingleColor(int use_kmeans=1) {
  Size s = src.size();
  Mat single_color_image = Mat::zeros( s.height, s.width, CV_8UC3 );

  //int min_diff=0;
  //if (!use_kmeans) { min_diff=10; }
  int n_pixels=0;

  for (int i=0; i<src.cols; i++) {
    for (int j=0; j<src.rows; j++) {
      Vec3b color;
      
      if (use_kmeans) {
	color = kmeans_image.at<Vec3b>(Point(i,j));
      } else {
	color = src.at<Vec3b>(Point(i,j));
      }
      //int diff = (color[0]-g_paint_color[0])^2 + (color[1]-g_paint_color[1])^2 + (color[2]-g_paint_color[2])^2;
      if (color[0] == g_paint_color[0] && color[1] == g_paint_color[1] && color[2] == g_paint_color[2]) {
	//if (diff<=min_diff) {
	single_color_image.at<Vec3b>(Point(i,j)) = color; // set pixel      
	n_pixels++;
      }
    }
  }
  printf("%d pixels found\n",n_pixels);
  
  namedWindow( "single_color_image_window", CV_WINDOW_AUTOSIZE );
  imshow( "single_color_image_window", single_color_image );
  moveWindow("single_color_image_window",win_w(0),win_h(1)); 
}


// try to "paint" by lots of tiny brushstrokes in the right direction for a single (globally defined) color
void singlePaintColorOnCanvas(int r=-1, int g=-1, int b=-1) {
  // if colors not defined, use global color
  if (r<0) { r=g_paint_color[2]; }
  if (g<0) { g=g_paint_color[1]; }
  if (b<0) { b=g_paint_color[0]; }

  // brush stroke parameters
  int thickness=3;  //6
  double line_length=6; // 12
  int lineType=8; // always keep 8 for solid antialiased line

  // get a list of valid pixels
  int n_pixels=0;
  int pixels[src.rows*src.cols][2];
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
  printf("%d pixels found\n",n_pixels);

  int num_candidates=n_pixels/(thickness*line_length/15);
  //int num_candidates=n_pixels/5;
  int num_strokes=0;

  for (int i=0; i<num_candidates; i++) {
    int index = rng.uniform(0,n_pixels);
    int x = pixels[index][0];
    int y = pixels[index][1];

    // check to see if it's the right color
    Vec3b color = kmeans_image.at<Vec3b>(Point(x,y));
    if (color[0] != b || color[1] != g || color[2] != r) {
      continue;
    } 

    double rad = g_gradients[src.rows*y + x];

    rad=rad+M_PI/2; // rotate 90;
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
      double origDiff = calcDiffBetweenImages(&origimageROI,&paintROI);
      double newDiff = calcDiffBetweenImages(&origimageROI,&newpaintROI);
      if (i%1000==0) {
	printf("%d/%d: NEW:%f ORIG:%f",num_strokes, i, newDiff,origDiff);
      }

      // if not, revert back to the original
      if (newDiff>=origDiff) {
	cv::Mat destinationROI = canvas_image( roi );
	paintROI.copyTo( destinationROI );
	if (i%1000==0) { printf(" WORSE\n"); }
      } else {
	if (i%1000==0) { printf(" BETTER\n"); }
	num_strokes++;
      }

      if (i%1000==0) {
	imshow( "canvas_window", canvas_image );
	waitKey(1);
      }
    }
  }
  imshow( "canvas_window", canvas_image );
  waitKey(1);
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
    printf("XSORTED %d: [%d,%d,%d] = NORM:%f\n",i,colors[i][0],colors[i][1],colors[i][2],norm(colors[i]));
    //g_paint_color[0] = colors[i][0];
    //g_paint_color[1] = colors[i][1];
    //g_paint_color[2] = colors[i][2];
    updateColorWindow(colors[i][2],colors[i][1],colors[i][0]); // sets global values
    showSingleColor();
    singlePaintColorOnCanvas(); 
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
    for (int i=0; i<5; i++) {
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


/** @function main */
int main( int argc, char** argv )
{

  printf("colors <image_filename>\n");
  printf("Commands:\n");
  printf("  p = autopaint, P = autopaint (3x strokes) \n");
  printf("  n = create a fresh (green) canvas \n");
  printf("  i = paint starting with darkest ('space'=interate,'i' again=do all in order \n");
  printf("      NOTE: make sure you press 'n' first to create a fresh canvas before press 'i'\n");
  printf("  s = paint only a single color in the canvas (choose color by clicking in any window) \n");
  printf("  c = show where single color on the kmeans image (choose color by clicking in any window) \n");

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

  /// Create Window
  char source_window[] = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  moveWindow(source_window,win_w(0),win_h(0,-30));
  imshow( source_window, src );

  createTrackbar( " Blur loops:", source_window, &g_blur_loops, g_max_blur_loops, colors_callback );
  createTrackbar( " Colors:", source_window, &g_colors, g_max_colors, colors_callback );
  colors_callback( 0, 0 );

  //set the callback function for any mouse event
  setMouseCallback("kmeans_window", kmeansMouseCallBackFunc, NULL);
  setMouseCallback(source_window, sourceMouseCallBackFunc, NULL);

  while (1) {
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

    } else if (k == int('n')) { // reset canvas
      Size s = src.size();
      canvas_image = Mat::zeros( s.height, s.width, CV_8UC3 );
      canvas_image.setTo(cv::Scalar(0,255,0)); // redVal,greenVal,blueVal
      namedWindow( "canvas_window", CV_WINDOW_AUTOSIZE );
      imshow( "canvas_window", canvas_image );
      moveWindow("canvas_window",win_w(1),win_h(1)); 
      setMouseCallback("canvas_window", canvasMouseCallBackFunc, NULL);

    } else if (k == int('s')) { // paint one color on canvas
      singlePaintColorOnCanvas();
    } else if (k == int('c')) { // show where single color exists on kmeans image
      showSingleColor(1);
    } else {
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

