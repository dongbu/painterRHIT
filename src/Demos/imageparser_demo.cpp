#include "imageparser.cpp"

int main(void)
{
  std::string g_image_filename="images/lena.jpg";
  //char* g_image_filename="images/eggs-512.jpg";
  cv::Mat src = cv::imread( g_image_filename, 1 );
  if (src.empty()) {
    std::cout << "!!! Failed imread()\n ./colors <image filename>" << std::endl;
    return -1;
  }  

  DrawWindow WIPC = DrawWindow(src.cols,src.rows,"IPC countours"); // w,h
  WIPC.moveWindow(0,20);
  Shapes SIPC;
  ImageParserContours IPC;
  if (1) {
    //IPC.setDebug(1);
    IPC.setMinContourLength(5);
    IPC.setCannyThreshold(50);
    IPC.parseImage(src);
    IPC.useRandomColors(1); // shows each contour as a random color
    //IPC.draw(); // 
    IPC.defineShapes(&SIPC);
    //IPC.printImageData(1);
    
    WIPC.clearWindow(230,230,230); // default background is white
    SIPC.drawAll(&WIPC);
    WIPC.show();
  }

  Shapes SIPK;
  ImageParserKmeans IPK;
  DrawWindow WIPK = DrawWindow(src.cols,src.rows,"IPK grouped pixel regions"); // w,h
  WIPK.moveWindow(1*src.cols,20);

  if (1) {
    //IPK.setDebug(1);
    IPK.setMinPixelsInRegion(5);
    IPK.parseImage(src);
    //IPK.useRandomColors(1); // shows each region as a random color
    //IPK.draw();
    IPK.defineShapes(&SIPK);
    //  IPC.printImageData(1);
    
    WIPK.clearWindow(230,230,230); // default background is white
    SIPK.drawAll(&WIPK);
    WIPK.show();
  }


  Shapes SIPK2;
  ImageParserKmeans IPK2;
  DrawWindow WIPK2 = DrawWindow(src.cols,src.rows,"IPK split pixel regions"); // w,h
  WIPK2.moveWindow(2*src.cols,20);
  
  if (1) {
    IPK2.parseImage(src,0);
    IPK2.defineShapes(&SIPK2);
    WIPK2.clearWindow(230,230,230); // default background is white
    SIPK2.drawAll(&WIPK2);
    WIPK2.show();
  }

  while (1) {
    int k = cv::waitKey(33);
    if (k==27) { // Esc key to stop
      return(0);
    }
  }

   return 0;
}
