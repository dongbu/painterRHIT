#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/highgui.h>


// Packages simple drawing commands for the simulator display window
/**
 * @brief The DrawWindow class
 */
class DrawWindow {
protected:
    int width, height;
    cv::Scalar canvas_color;
    cv::Scalar pen_color;
    cv::Vec3b pen_color_vec;

    int pen_thickness;
    int line_type;
    std::string window_name;
    int winx;
    int winy;

public:
    cv::Mat grid;
    std::vector<cv::Point> poly_points; // will automatically allocate member if needed

    /**
   * @brief sets the color of the canvas.
   * @param r
   * @param g
   * @param b
   */
    void setCanvasColor(int r, int g, int b) {
        canvas_color = cv::Scalar(b,g,r); // yah, in this order
    }

    /**
   * @brief clears the window to a single color.
   */
    void clearWindow() { grid.setTo(canvas_color); show(); }

    /**
   * @brief clears window and sets pixels to r g b
   * @param r
   * @param g
   * @param b
   */
    void clearWindow(int r, int g, int b) {
        setCanvasColor(r,g,b);
        clearWindow();
    }

    /**
   * @brief set the type of the line.
   * @param type
   */
    void setLineType(const char* type) {
        if (strcmp(type,"solid") == 0) { line_type=8; }
        // 8 for solid antialiased line
    }

    /**
   * @brief set line thickness
   * @param thickness
   */
    void setLineThickness(int thickness=1) {
        pen_thickness = thickness;
    }

    /**
   * @brief set color of drawing pen.
   * @param r
   * @param g
   * @param b
   */
    void setPenColor(int r, int g, int b) {
        pen_color = cv::Scalar(b,g,r); // yah, in this order
        pen_color_vec[0]= pen_color[0]; // yes, ugly :(
        pen_color_vec[1]= pen_color[1];
        pen_color_vec[2]= pen_color[2];
    }

    /**
   * @brief draw line to grid.
   * @param pt1
   * @param pt2
   */
    void drawLine(cv::Point pt1, cv::Point pt2) {
        cv::line( grid, pt1, pt2, pen_color, pen_thickness, line_type );
    }

    /**
   * @brief draw line to grid.
   * @param x1
   * @param y1
   * @param x2
   * @param y2
   */
    void drawLine(int x1, int y1, int x2, int y2) {
        drawLine(cv::Point(x1,y1),cv::Point(x2,y2));
    }

    /**
   * @brief draw rectangle to grid.
   * @param pt1
   * @param pt2
   * @param fill
   */
    void drawRectangle(cv::Point pt1, cv::Point pt2, int fill=0) {
        int thickness=pen_thickness;
        if (fill) { thickness=-1; }
        cv::rectangle( grid, pt1, pt2, pen_color, thickness);
    }

    /**
   * @brief draw rectangle to grid
   * @param x1
   * @param y1
   * @param x2
   * @param y2
   * @param fill
   */
    void drawRectangle(int x1, int y1, int x2, int y2, int fill=0) {
        drawRectangle(cv::Point(x1,y1),cv::Point(x2,y2),fill);
    }

    /**
   * @brief draw circle to grid.
   * @param pt
   * @param r
   * @param fill
   */
    void drawCircle(cv::Point pt, int r, int fill=0) {
        int thickness=pen_thickness;
        if (fill) { thickness=-1; }
        cv::circle( grid, pt, r, pen_color, thickness, line_type);
    }

    /**
   * @brief draw circle to grid.
   * @param x
   * @param y
   * @param r
   * @param fill
   */
    void drawCircle(int x, int y, int r, int fill=0) {
        drawCircle(cv::Point(x,y),r,fill);
    }

    /**
   * @brief draw Ellipse to grid.
   * @param pt
   * @param axes
   * @param angle
   * @param fill
   */
    void drawEllipse(cv::Point pt, cv::Size axes, double angle, int fill=0) {
        int thickness=pen_thickness;
        if (fill) { thickness=-1; }
        cv::ellipse(grid,pt,axes,angle, 0.,360.,pen_color, thickness, line_type);
    }

    /**
   * @brief draw Ellipse to grid.
   * @param x
   * @param y
   * @param major_axis
   * @param minor_axis
   * @param angle
   * @param fill
   */
    void drawEllipse(int x, int y, int major_axis, int minor_axis, double angle, int fill=0) {
        drawEllipse(cv::Point(x,y),cv::Size(major_axis/2,minor_axis/2),angle,fill);
    }

    /**
   * @brief draw text to grid
   * @param pt
   * @param text
   */
    void drawText(cv::Point pt, const char *text) {
        cv::putText(grid, text, pt, cv::FONT_HERSHEY_DUPLEX, 0.3, pen_color, 1, CV_AA);
    }

    /**
   * @brief draw text to grid.
   * @param x1
   * @param y1
   * @param text
   */
    void drawText(int x1, int y1, const char *text) {
        drawText(cv::Point(x1,y1),text);
    }

    /**
   * @brief start poly points
   */
    void startPolyPoints() {
        while (poly_points.size()) { poly_points.pop_back(); }
    }
    /**
   * @brief add point to poly points.
   * @param x
   * @param y
   */
    void addPolyPoint(int x, int y) {
        poly_points.push_back(cv::Point(x,y));
    }
    /**
   * @brief draw polypoints
   */
    void drawPolyPoints() {
        int n = poly_points.size();
        int npt[] = { n };

        /** Create some points */
        cv::Point **points = new cv::Point*[1];
        points[0]=new cv::Point[n];

        for (int i=0; i<n; i++) {
            points[0][i] = poly_points[i];
        }
        const cv::Point* ppt[1] = { points[0] };

        fillPoly( grid, ppt, npt, 1, pen_color, line_type);
        delete [] points[0];
        delete [] points;
    }

    /**
   * @brief draw single pixel.
   * @param pt
   */
    void drawPixel(cv::Point pt) {
        grid.at<cv::Vec3b>(pt) = pen_color_vec;
    }

    /**
   * @brief draw single pixel
   * @param x
   * @param y
   */
    void drawPixel(int x,int y) { drawPixel(cv::Point(x,y)); }

    /**
   * @brief draw a region of pixels.
   * @param pixels
   */
    void drawRegion(std::vector<cv::Point> pixels) { // could be changed to pass reference
        for (size_t i=0; i<pixels.size(); i++) {
            grid.at<cv::Vec3b>(pixels[i]) = pen_color_vec;
        }
    }
    
    /**
  * @brief define mouse callback function.
  * @param onMouse
  */
    void defineMouseCallback (cv::MouseCallback onMouse) {
        cv::setMouseCallback(window_name, onMouse, NULL);
    }

    /**
   * @brief moves the window.
   * @param x
   * @param y
   */
    void moveWindow(int x, int y) {
        cv::moveWindow(window_name,x,y);
        winx = x;
        winy = y;
    }

    /**
   * @brief moves the window delta.
   * @param dx
   * @param dy
   */
    void moveWindowDelta(int dx, int dy) {
        winx += dx;
        winy += dy;
        cv::moveWindow(window_name,winx,winy);
    }

    /**
   * @brief hides the window.
   */
    void hideWindow() { cv::moveWindow(window_name,5000,5000); }

    /**
   * @brief shows the window.
   */
    void showWindow() { cv::moveWindow(window_name,winx,winy); }

    /**
   * @brief brings window to top.
   */
    void popWindow() {
        // BringWindowToTop(cv::getWindowHandle(window_name));
    }

    /**
   * @brief show
   */
    void show() { cv::imshow(window_name, grid); }

    /**
   * @brief constructor
   * @param w
   * @param h
   * @param name
   */
    DrawWindow(int w, int h, std::string name) { // constructor
        width=w;
        height=h;
        window_name = name;
        cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE );
        winx=0;
        winy=0;

        grid = cv::Mat( height, width, CV_8UC3 ); // 3 channel color
        setLineType("solid");
        setLineThickness(2);
        setPenColor(0,0,0);
        clearWindow(255,255,255);
    }

	DrawWindow(int w, int h) { //constructor
		width = w;
		height = h;
		winx = 0;
		winy = 0;

		grid = cv::Mat(height, width, CV_8UC3); // 3 channel color
		setLineType("solid");
		setLineThickness(2);
		setPenColor(0, 0, 0);
		clearWindow(255, 255, 255);
	}

    /**
   * @brief destructor
   */
  ~DrawWindow() {
    cv::destroyWindow(window_name);
  }

};

