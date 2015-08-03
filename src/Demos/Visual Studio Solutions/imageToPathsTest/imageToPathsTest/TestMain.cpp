#include "regiontopaths.cpp"
#include "shapes.cpp"

int main(int argc, char** argv) {
	int w = 400;
	int h = 300;
	int border = 30; // extend paintable region outside the canvas so can draw over the edges
	RegionToPaths RTP = RegionToPaths(w, h, border);
	RTP.clear();

	// mark pixels to be painted (e.g., the region)
	MyRectangle *r = new MyRectangle();
	r->setCorners(100, 100, 200, 200);
	r->setFill();
	PixelRegion *p = r->toPixelRegion();
	std::vector<cv::Point> pts = p->getPoints();
	for (int i = 0; i < pts.size(); i++){ RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }

	Brush brush = Brush(30, 20, "ellipse");
	brush.setColor(20, 20, 40);
	RTP.defineBrush(&brush);

	RTP.definePaths();

	// get a vector of brush stroke points & draw blue lines on the path
	std::vector<std::vector<cv::Point> > brush_strokes = RTP.getBrushStrokes();
	for (int i = 0; i < brush_strokes.size(); i++) {
		printf("stroke %i\n", i + 1);
		for (int j = 0; j < brush_strokes.at(i).size(); j++) {
			printf("point [%i][%i]: (%i,%i) \n", i, j, brush_strokes.at(i).at(j).x,brush_strokes.at(i).at(j).y);
		}
	}

	printf("brush_strokes.size(): %i\n", brush_strokes.size());

	while (1) {
		int k = cv::waitKey(33);
	}
}
