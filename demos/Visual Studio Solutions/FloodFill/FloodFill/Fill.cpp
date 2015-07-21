#include "Fill.h"

Fill::Fill()
{
	grid = imread("../../images/BlueSquare.png");
	floodColor = grid.at<Vec3b>(Point(100, 100));
	processed = Mat(grid.size().width, grid.size().height, CV_64F, cvScalar(0.));

	flood(Point(grid.size().width / 2, grid.size().height / 2));
	imshow("Display Image", grid);
	waitKey(0);
}

void Fill::flood(Point p) {
	std::vector<Point> pointVec;
	pointVec.push_back(p);
	while (pointVec.size() > 0)
	{
		p = pointVec.back();
		pointVec.pop_back();
		grid.at<Vec3b>(p);
		Vec3b curPix = grid.at<Vec3b>(p);

		bool skip = false;

		if ((floodColor[0] == curPix[0] && floodColor[1] == curPix[1] && floodColor[2] == curPix[2])) {
			curPix[0] = 36;
			curPix[1] = 255;
			curPix[2] = 92;
			grid.at<Vec3b>(p) = curPix;
		}
		else skip = true;

		if (!skip && p.y - 1 > 0 && processed.at<double>(p.x, p.y - 1) != 1) { //recurse down
			processed.at<double>(p.x, p.y - 1) = 1;
			pointVec.push_back(Point(p.x, p.y - 1));
		}
		if (!skip && p.x - 1 > 0 && processed.at<double>(p.x - 1, p.y) != 1) {	//recurse left
			processed.at<double>(p.x - 1, p.y) = 1;
			pointVec.push_back(Point(p.x - 1, p.y));
		}
		if (!skip && p.y + 1 < grid.size().height && processed.at<double>(p.x, p.y + 1) != 1) { //recurse up
			processed.at<double>(p.x - 1, p.y) = 1;
			pointVec.push_back(Point(p.x, p.y + 1));
		}
		if (!skip && p.x + 1 < grid.size().width && processed.at<double>(p.x + 1, p.y) != 1) { //recurse right
			processed.at<double>(p.x + 1, p.y) != 1;
			pointVec.push_back(Point(p.x + 1, p.y));
		}
	}
}

Fill::~Fill()
{
}

int main(int argc, char** argv)
{
	new Fill();
}