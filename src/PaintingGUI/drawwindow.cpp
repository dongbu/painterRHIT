#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/highgui.h> 

// Packages simple drawing commands for the simulator display window
class DrawWindow {
protected:
	int width, height;
	cv::Scalar canvas_color;
	cv::Scalar pen_color;
	cv::Vec3b pen_color_vec;

	int window_created;
	int pen_thickness;
	int line_type;
	std::string window_name;
	int winx;
	int winy;

public:
	cv::Mat grid;
	std::vector<cv::Point> poly_points; // will automatically allocate member if needed

	//set the color of the canvas
	void setCanvasColor(int r, int g, int b) {
		canvas_color = cv::Scalar(b, g, r); // yah, in this order
	}

	//clear window
	void clearWindow() { grid.setTo(canvas_color); }

	// clears window and set pixels to r g b
	void clearWindow(int r, int g, int b) {
		setCanvasColor(r, g, b);
		clearWindow();
	}
	//get color at specified pixel
	cv::Vec3b getColor(int i, int j) {
		return grid.at<cv::Vec3b>(cv::Point(i, j));
	}

	//void setLineType(std::string type) {
	void setLineType(const char* type) {
		if (strcmp(type, "solid") == 0) { line_type = 8; }
		// 8 for solid antialiased line
	}

	//set thickness of lines being drawn.
	void setLineThickness(int thickness = 1) {
		pen_thickness = thickness;
	}

	//set color of pen drawing lines.
	void setPenColor(int r, int g, int b) {
		pen_color = cv::Scalar(b, g, r); // yah, in this order
		pen_color_vec[0] = b;
		pen_color_vec[1] = g;
		pen_color_vec[2] = r;
	}
	//set color of pens being drawn.
	void setPenColor(cv::Vec3b c) { setPenColor(c[2], c[1], c[0]); }
	//set color of pens being drawn.
	void setPenColor(cv::Scalar c) { setPenColor(c[0], c[1], c[2]); }

	//draw a line between two cv::Points
	void drawLine(cv::Point pt1, cv::Point pt2) {
		cv::line(grid, pt1, pt2, pen_color, pen_thickness, line_type);
	}

	//draw line given two sets of coordinates
	void drawLine(int x1, int y1, int x2, int y2) {
		drawLine(cv::Point(x1, y1), cv::Point(x2, y2));
	}

	// going to play w/ having window draw lines with brushes
	//void drawLine(Brush *brush, int x1, int y1, int x2, int y2) {
	//  brush->drawLine(this, cv::Point(x1,y1),cv::Point(x2,y2));
	//}

	//draw rectangle given two points and fill status.
	void drawRectangle(cv::Point pt1, cv::Point pt2, int fill = 0) {
		int thickness = pen_thickness;
		if (fill) { thickness = -1; }
		cv::rectangle(grid, pt1, pt2, pen_color, thickness);
	}

	//draw rectangle given two sets of coordinates and fill status.
	void drawRectangle(int x1, int y1, int x2, int y2, int fill = 0) {
		drawRectangle(cv::Point(x1, y1), cv::Point(x2, y2), fill);
	}

	//draw circle given two points and fill status.
	void drawCircle(cv::Point pt, int r, int fill = 0) {
		int thickness = pen_thickness;
		if (fill) { thickness = -1; }
		cv::circle(grid, pt, r, pen_color, thickness, line_type);
	}

	//draw circle given two sets of coordinates and fill status.
	void drawCircle(int x, int y, int r, int fill = 0) {
		drawCircle(cv::Point(x, y), r, fill);
	}

	//draw ellipse using a point.
	void drawEllipse(cv::Point pt, cv::Size axes, double angle, int fill = 0) {
		int thickness = pen_thickness;
		if (fill) { thickness = -1; }
		cv::ellipse(grid, pt, axes, angle, 0., 360., pen_color, thickness, line_type);
	}

	//draw ellipse using coordinates
	void drawEllipse(int x, int y, int major_axis, int minor_axis, double angle, int fill = 0) {
		drawEllipse(cv::Point(x, y), cv::Size(major_axis / 2, minor_axis / 2), angle, fill);
	}

	//draw text using points
	void drawText(cv::Point pt, const char *text) {
		cv::putText(grid, text, pt, cv::FONT_HERSHEY_DUPLEX, 0.3, pen_color, 1, CV_AA);
	}

	//draw text using coordinates
	void drawText(int x1, int y1, const char *text) {
		drawText(cv::Point(x1, y1), text);
	}

	//begin polypoints
	void startPolyPoints() {
		while (poly_points.size()) { poly_points.pop_back(); }
	}

	//add to polypoints
	void addPolyPoint(int x, int y) {
		poly_points.push_back(cv::Point(x, y));
	}

	//draw completed set of pollypoints
	void drawPolyPoints() {
		unsigned int n = poly_points.size();
		int npt[] = { n };

		/** Create some points */
		cv::Point **points = new cv::Point*[1];
		points[0] = new cv::Point[n];

		for (size_t i = 0; i < n; i++) {
			points[0][i] = poly_points[i];
		}
		const cv::Point* ppt[1] = { points[0] };

		fillPoly(grid, ppt, npt, 1, pen_color, line_type);
		delete[] points[0];
		delete[] points;
	}

	//draw single pixel.
	void drawPixel(cv::Point pt) {
		grid.at<cv::Vec3b>(pt) = pen_color_vec;
	}

	//draw single pixel.
	void drawPixel(int x, int y) {
		if (x >= 0 && y >= 0 && x < grid.cols && y < grid.rows) drawPixel(cv::Point(x, y));
	}

	//get single pixel.
	cv::Vec3b getPixel(cv::Point pt) {
		return grid.at<cv::Vec3b>(pt);
	}

	//get single pixel.
	cv::Vec3b getPixel(int x, int y) {
		if (x >= 0 && y >= 0 && x < grid.cols && y < grid.rows) {
			return getPixel(cv::Point(x, y));
		}
		else {
			return getPixel(cv::Point(0, 0)); // yah silly 
		}
	}

	//draw a region of pixels.
	void drawRegion(std::vector<cv::Point> pixels) { // could be changed to pass reference but I've not figured out how to access vector
		for (unsigned int i = 0; i < pixels.size(); i++) {
			grid.at<cv::Vec3b>(pixels[i]) = pen_color_vec;
		}
	}

	// blur the window
	void blur(int size = 3) {
		cv::blur(grid, grid, cv::Size(size, size));
	}

	// put random specks all over the window 
	void speckle(double fraction = 0.1) {
		cv::RNG rng;
		int n = fraction * grid.cols * grid.rows;
		for (int i = 0; i < n; i++) {
			setPenColor(0, rng.uniform(0, 255), rng.uniform(0, 255));
			drawPixel(rng.uniform(0, width), rng.uniform(0, height));
		}
	}

	//used for mouse listeners.
	void defineMouseCallback(cv::MouseCallback onMouse) {
		cv::setMouseCallback(window_name, onMouse, NULL);
	}

	//move the window to a point on the screen.
	void moveWindow(int x, int y) {
		cv::moveWindow(window_name, x, y);
		winx = x;
		winy = y;
	}

	//move the window a relative distance.
	void moveWindowDelta(int dx, int dy) {
		winx += dx;
		winy += dy;
		cv::moveWindow(window_name, winx, winy);
	}

	//hide the window.
	void hideWindow() { cv::moveWindow(window_name, 5000, 5000); }

	//show the window.
	void showWindow() {
		cv::moveWindow(window_name, winx, winy);
	}

	//bring window to foreground. (does not work yet).
	void topWindow() {
		// BringWindowToTop(cv::getWindowHandle(window_name));
	}

	// creates the window
	void popWindow() {
		cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		window_created = 1;
	}

	//shows window (and creates if not existing).
	void show() {
		if (!window_created) { popWindow(); }
		cv::imshow(window_name, grid);
	}

	//constructor
	DrawWindow(int w, int h, std::string name, int hide_window = 0) { // constructor
		width = w;
		height = h;
		window_name = name;
		window_created = 0;
		if (!hide_window) { popWindow(); }
		winx = 0;
		winy = 0;

		grid = cv::Mat(height, width, CV_8UC3); // 3 channel color
		setLineType("solid");
		setLineThickness(2);
		setPenColor(0, 0, 0);
		clearWindow(255, 255, 255);
	}

	//destructor
	~DrawWindow() {
		cv::destroyWindow(window_name);
	}

};

