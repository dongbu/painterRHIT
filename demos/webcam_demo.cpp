#include "webcam.cpp"
#include "drawwindow.cpp"

int main(void)
{
  // mostly to just have something to cycle waitKey (w/o a window, waitKey doesn't work?)
  DrawWindow DW = DrawWindow(400,80,"Webcam Tool Instructions"); 
  DW.drawText(10,10,"f=get webcam frame");
  DW.drawText(10,20,"F=get ave of 10 webcam frames");
  DW.drawText(10,30,"m=get mapped webcam frame");
  DW.drawText(10,40,"c=calibrate webcam(1-4,arrows,s=save,esc=done)");
  DW.drawText(10,50,"s=save mapping data");
  DW.drawText(10,60,"l=load mapping data");
  DW.show(); 

  int width=300;
  int height=400;
  Webcam W;
  W.setMapSize(width,height);

  cv::Mat frame;

  while (1) {   
    int k = cv::waitKey(33);
    //if (k>0) { printf("key = %d\n",k); }
    if (k==27) { return(0); }  // Esc key to stop
    if (k == int('c')) { W.calibrateWebcam(); }
    if (k == int('m')) {
      printf("showing mapped frame\n");
      W.getMappedFrame(&frame);
      cv::imshow("mapped F", frame);
    }
    if (k == int('f')) {
      printf("showing frame\n");
      W.getFrame(&frame);
      cv::imshow("webcam", frame);
    }
    if (k == int('F')) {
      printf("showing merge of 10 frames\n");
      W.getFrame(&frame,10);
      cv::imshow("webcam", frame);
    }
    if (k == int('s')) { // "save"
      double *points = W.getMapping();
      printf("Here are the coordinates of 4 corner mapping\n");
      for (int i=0; i<4; i++) {
	      printf("Coord %i = (%f,%f)\n",i,points[2*i],points[2*i+1]);
      }
    }
    if (k == int('l')) { // "load"
      double delx=.1;
      double dely=.2;
      
      W.setMapping(delx*width,dely*height,
		   (1-delx)*width,dely*height,
		   (1-delx)*width,(1-dely)*height,
		   delx*width,(1-dely)*height);
      printf("mapping loaded... type 'm' to see mapped capture\n");
    }
  }
}
