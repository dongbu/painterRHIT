#pragma once

// Object to do pixel related things
#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <highgui.h> // WINDOW_AUTOSIZE (this one works with andrew)
#include <opencv/highgui.h> // WINDOW_AUTOSIZE (this one works with Gunnar)


class PixelTools {
protected:
public:

	// return pixel points between two points
	std::vector<cv::Point> getPixelsBetweenPoints(cv::Point p1, cv::Point p2) {
		return getPixelsBetweenPoints(p1.x, p1.y, p2.x, p2.y);
	}

	std::vector<cv::Point> getPixelsBetweenPoints(int p1x, int p1y, int p2x, int p2y) {
		// make a list of points to be tested
		std::vector<cv::Point> points;
		if (p1x == p2x && p1y == p2y) { // a point
			points.push_back(cv::Point(p1x, p1y));
		}
		else { // not a point
			int done = 0;

			if (p1x != p2x) { // not vertical
				double slope = (double)(p2y - p1y) / double(p2x - p1x);
				if (fabs(slope) < 1) { // not steep line - just skim along x axis
					for (int i = p1x; i <= p2x; i++) {
						int j = p1y + slope * (double)(i - p1x);
						points.push_back(cv::Point(i, j));
					}
					done = 1;
				}
			}
			if (!done && p1y != p2y) { // not horizontal - steep line - skip along y axis
				double slope = (double)(p2x - p1x) / double(p2y - p1y);
				for (int j = p1y; j <= p2y; j++) {
					int i = p1x + slope * (double)(j - p1y);
					points.push_back(cv::Point(i, j));
				}
			}
		}
		return points;
	}

	// returns 0-1 how close color (NOTE: Only used below by testLineQualityUNUSED
	double colorDistance(cv::Vec3b c1, cv::Vec3b c2) {
		int norm = std::abs(c1[0] - c2[0]) + std::abs(c1[1] - c2[1]) + std::abs(c1[2] - c2[2]);
		//printf("N1:%i\n",norm);
		return (double)norm / (255. * 3.);
	}

	// return what fraction of pixels of a line between two points are inside color closeness 
	double testLineQualityUNUSED(cv::Mat *grid, cv::Point p1, cv::Point p2, cv::Vec3b color, double desired_distance,
		std::vector<cv::Point> *bad_pixels) {

		// get a list of points to be tested
		std::vector<cv::Point> points = getPixelsBetweenPoints(p1, p2);

		int num_pixels = 0;
		int num_far_color_pixels = 0;
		cv::Vec3b grid_color;

		// test the points
		for (int i = 0; i < (int)points.size(); i++) {
			grid_color = grid->at<cv::Vec3b>(points[i]);
			double distance = colorDistance(grid_color, color);

			num_pixels++;
			if (distance > desired_distance) {
				num_far_color_pixels++;
				if (bad_pixels) { bad_pixels->push_back(points[i]); }
			}
			//printf("%i,%i = g[%i %i %i] %f close %i/%i\n",points[i].x,points[i].y,grid_color[0],grid_color[1],grid_color[2],distance,num_far_color_pxels,(int)points.size());
		}

		if (num_pixels) {
			return 1 - (double)num_far_color_pixels / (double)num_pixels;
		}
		else {
			return 1.0;
		}
	}

	// scans an image and returns the pixels at the border of a colored region (optionally, returns the interior)
	void defineBoundary(cv::Mat *image, cv::Scalar region_color,
		std::vector<cv::Point> *boundary, std::vector<cv::Point> *interior = NULL,
		int offsetx = 0, int offsety = 0) {
		int width = image->cols;
		int height = image->rows;
		// determine the boundary pixels
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				cv::Scalar color = image->at<cv::Vec3b>(cv::Point(i, j)); // hxxx

				int is_boundary = 0;
				if (color == region_color) {
					// see if pixel is at boundary of region to be painted
					if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
						is_boundary = 1;
					}
					else {
						// check for neighbors
						int left = std::max(i - 1, 0);
						int right = std::min(i + 1, width - 1);
						int top = std::max(j - 1, 0);
						int bottom = std::min(j + 1, height - 1);
						//int pixels=(1+right-left)*(1+bottom-top) - 1; // how many neighbor pixels to compare

						for (int n = left; n <= right; n++) {
							for (int m = top; m <= bottom; m++) {
								cv::Scalar neighbor_color = image->at<cv::Vec3b>(cv::Point(n, m));
								if (neighbor_color[0] != region_color[0] ||
									neighbor_color[1] != region_color[1] ||
									neighbor_color[2] != region_color[2]) {
									is_boundary = 1;
									//boundary->push_back(cv::Point(i,j));
									n = right;
									m = bottom;
								}
							}
						}
					}
					if (is_boundary) {
						boundary->push_back(cv::Point(i + offsetx, j + offsety));
					}
					else if (interior) {
						interior->push_back(cv::Point(i + offsetx, j + offsety));
					}
				}
			}
		}
	}

	// given a region of pixels, returns the pixels at the border of a colored region (optionally, returns the interior)
	void defineBoundary(std::vector<cv::Point> region, cv::Scalar region_color,
		std::vector<cv::Point> *boundary, std::vector<cv::Point> *interior = NULL) {
		int minx = region[0].x;
		int miny = region[0].y;
		int maxx = 0;
		int maxy = 0;
		for (int i = 0; i < (int)region.size(); i++) {
			minx = std::min(minx, region[i].x);
			miny = std::min(minx, region[i].y);
			maxx = std::max(maxx, region[i].x);
			maxy = std::max(maxx, region[i].y);
		}
		cv::Mat image = cv::Mat::zeros(cv::Size(maxx - minx, maxy - miny), CV_8UC3);
		//cv::Mat image = cv::Mat( maxy-miny, maxx-minx, CV_8UC3 ); // 3 channel color

		if (region_color[0] == 0 && region_color[0] == 0 && region_color[0] == 0) {
			image.setTo(cv::Scalar(1, 1, 1));
		}
		else {
			image.setTo(cv::Scalar(0, 0, 0));
		}

		cv::Vec3b region_color_vec(region_color[0], region_color[1], region_color[2]);
		for (int i = 0; i < (int)region.size(); i++) {
			image.at<cv::Vec3b>(region[i]) = region_color_vec;
		}
		defineBoundary(&image, region_color, boundary, interior, minx, miny);
	}

	PixelTools() {

	}
};
