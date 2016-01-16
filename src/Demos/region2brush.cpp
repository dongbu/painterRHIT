#include "drawwindow.cpp"
#include "regiontopaths.cpp"

int main(void)
{
  int width=400;
  int height=300;

  Brush brush = Brush(12,5,"ellipse");
  brush.setColor(0,0,0);
  
  brush.setDrawMode("paint");
  brush.loadPaintPixel(40);

  DrawWindow DW = DrawWindow(width,height,"Desired Painting"); 
  DW.moveWindow(20,150);

  DrawWindow RW = DrawWindow(width,height,"RegionToPaths Grid"); 
  RW.moveWindow(1*width,150);

  DrawWindow BW = DrawWindow(width,height,"BrushStrokes"); 
  BW.moveWindow(2*width,150);

  RegionToPaths RTP = RegionToPaths(width,height);

  // define overpaintable region
  DW.setPenColor(0,0,200);
  for (int i=.2*width; i<.6*width; i++) { //loop rows
    for (int j=.4*height; j<.8*height; j++) { //loop columns
      DW.drawPixel(i,j); 
      RTP.addOverpaintablePixel(i,j);
    }
  }
  
  // define desird region
  DW.setPenColor(0,0,0);
  for (int i=.3*width; i<.6*width; i++) { //loop rows
    for (int j=.2*height; j<.9*height; j++) { //loop columns
      DW.drawPixel(i,j); 
      RTP.addDesiredPixel(i,j);
    }
  }

  DW.show();
  
  RTP.putGridOnWindow(&RW);
  RW.show();

  // find brush strokes to cover that region
  RTP.defineBrush(&brush);
  RTP.definePaths();
  RTP.defineBrushStrokes();
  RTP.drawBrushStrokes(&BW);

  BW.show();

  while (1) {   
    int k = cv::waitKey(33);
    if (k==27) { return(0); }  // Esc key to stop
  }

}
