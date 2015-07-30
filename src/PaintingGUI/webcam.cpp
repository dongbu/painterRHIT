#pragma once
#include "stdafx.h"

// Packages simple drawing commands for the simulator display window
class Webcam {
protected:
	cv::VideoCapture *cam0; // open the default camera
	cv::VideoCapture *cam1; // open cam 1

	int map_width;
	int map_height;
	int cam_id;
	int flip_webcam;
	int webcam_corner;
	cv::Point2f webcamQuad[4]; // four corners of webcam input
	cv::Point2f zoomQuad[4]; // four corners of which region of the webcam is desired
	cv::Point2f canvasQuad[4]; // four corners of desired mapped output

public:

	// reset the map used to map webcam to another matrix/frame
	void resetMapping() {
		// The 4 points where the mapping is to be done , from top-left in clockwise order
		canvasQuad[0] = cv::Point2f(0, 0);
		canvasQuad[1] = cv::Point2f(map_width, 0);
		canvasQuad[2] = cv::Point2f(map_width, map_height);
		canvasQuad[3] = cv::Point2f(0, map_height);

		cv::Mat frame;
		getFrame(&frame); // get a new frame from camera
		zoomQuad[0] = cv::Point2f(0, 0);
		zoomQuad[1] = cv::Point2f(frame.cols - 0, 0);
		zoomQuad[2] = cv::Point2f(frame.cols - 0, frame.rows - 0);
		zoomQuad[3] = cv::Point2f(0, frame.rows - 0);

	}

	//sets the zoom of the webcam (the name kinda gives it away)
	void setWebcamZoom(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
		zoomQuad[0] = cv::Point2f(x1, y1);
		zoomQuad[1] = cv::Point2f(x2, y2);
		zoomQuad[2] = cv::Point2f(x3, y3);
		zoomQuad[3] = cv::Point2f(x4, y4);
	}

	//recieves the zoom
	double * getWebcamZoom() {
		static double map[8];
		map[0] = zoomQuad[0].x;
		map[1] = zoomQuad[0].y;
		map[2] = zoomQuad[1].x;
		map[3] = zoomQuad[1].y;
		map[4] = zoomQuad[2].x;
		map[5] = zoomQuad[2].y;
		map[6] = zoomQuad[3].x;
		map[7] = zoomQuad[3].y;
		return map;
	}

	// used by calibrateWebcam to have user click on the 4 corners of webcam's desired region
	static void zoomMouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
		if (flags){}
		Webcam *self = static_cast<Webcam*>(userdata);
		if (event == cv::EVENT_LBUTTONDOWN) {
			self->zoomQuad[self->webcam_corner].x = x * 2;
			self->zoomQuad[self->webcam_corner].y = y * 2;
			self->webcam_corner++;
		}
	}

	// turns the clicked points into a usable square
	void correctZoomQuad() {
		float minX, maxX, minY, maxY;
		minX = minY = std::max(map_width, map_height);
		maxX = minY = 0;

		for (int i = 0; i < 4; i++) {
			if (zoomQuad[i].x < minX) minX = zoomQuad[i].x;
			if (zoomQuad[i].x > maxX) maxX = zoomQuad[i].x;
			if (zoomQuad[i].y < minY) minY = zoomQuad[i].y;
			if (zoomQuad[i].y > maxY) maxY = zoomQuad[i].y;
		}

		zoomQuad[0].x = maxX;
		zoomQuad[0].y = minY;
		zoomQuad[1].x = minX;
		zoomQuad[1].y = minY;
		zoomQuad[2].x = minX;
		zoomQuad[2].y = maxY;
		zoomQuad[3].x = maxX;
		zoomQuad[3].y = maxY;

	}

	// sets the desired region of the webcam 
	cv::Mat calibrateWebcam(int skip_reset = 0) {
		cv::Mat webcam;
		cv::Mat mapped_webcam; // this is the webcam mapped to the same dimensions as the final canvas pixels (what we're returning)
		//    cv::Mat canvas; // this is the "canvas"
		//char mapped_name[] = "Mapped Webcam"; //REMEMBER ME
		char webcam_name[] = "Webcam";
		cv::namedWindow(webcam_name, 1);
		cv::moveWindow(webcam_name, 20, 20);
		cv::setMouseCallback(webcam_name, zoomMouseCallBackFunc, this);

		//cv::namedWindow(mapped_name, 1); //REMEMBER ME

		if (!skip_reset) { resetMapping(); }

		cv::Mat scaledWebcam;
		webcam_corner = 0;
		int done = 0;

		while (!done) {
			getFrame(&webcam, 3); // get a new frame from camera (blend 3 frames for better clarity)
			cv::resize(webcam, scaledWebcam, cv::Size(), 0.5, 0.5);
			//cv::imshow(webcam_name, scaledWebcam); //REMEMBER ME
			//getMappedFrame(&mapped_webcam); //REMEMBER ME
			//cv::imshow(mapped_name, mapped_webcam); //REMEMBER ME

			int k = cv::waitKey(33);
			if (k == 27 || k == int('x')) done = 1; // Esc key to stop
			else if (webcam_corner == 1) { //one point clicked
				cv::circle(scaledWebcam, cv::Point(zoomQuad[0].x / 2, zoomQuad[0].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
			}
			else if (webcam_corner == 2) { //two points clicked
				cv::circle(scaledWebcam, cv::Point(zoomQuad[0].x / 2, zoomQuad[0].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[1].x / 2, zoomQuad[1].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
			}
			else if (webcam_corner == 3) { //three points clicked
				cv::circle(scaledWebcam, cv::Point(zoomQuad[0].x / 2, zoomQuad[0].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[1].x / 2, zoomQuad[1].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[2].x / 2, zoomQuad[2].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
			}
			else if (webcam_corner == 4) { //four points clicked
				cv::circle(scaledWebcam, cv::Point(zoomQuad[0].x / 2, zoomQuad[0].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[1].x / 2, zoomQuad[1].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[2].x / 2, zoomQuad[2].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				cv::circle(scaledWebcam, cv::Point(zoomQuad[3].x / 2, zoomQuad[3].y / 2), 5, cv::Scalar(0, 255, 0), 2, 8);
				done = 1;
			}
			cv::imshow(webcam_name, scaledWebcam);
		}

		cv::destroyWindow(webcam_name); //delete webcam window

		//		if (webcam_corner != 4) return; //make sure we didn't quit halfway through.
		//correctZoomQuad();				//turning given points into an acutal square
		getMappedFrame(&mapped_webcam); //generating return
		return mapped_webcam;			//bail.  baIL.  BAIL!
	}

	// returns a 2x4 array which is the mapping of a frame to the canvas mapping
	cv::Mat getMapLambda(cv::Mat *frame) {
		cv::Mat frame_lambda(2, 4, CV_32FC1);

		// Set the lambda matrix the same type and size as webcam
		frame_lambda = cv::Mat::zeros(frame->rows, frame->cols, frame->type());

		// Get the Perspective Transform Matrix i.e. lambda
		frame_lambda = cv::getPerspectiveTransform(webcamQuad, canvasQuad);

		return frame_lambda;
	}

	// sets frame to a mapping of the webcam 
	void getMappedFrame(cv::Mat *mapped_frame) {
		cv::Mat webcam;
		getFrame(&webcam); // get a new frame from camera


		// zoom into the webcam to where ever the desired region

		cv::Mat zoom_lambda(2, 4, CV_32FC1);

		// Set the lambda matrix the same type and size as webcam
		zoom_lambda = cv::Mat::zeros(webcam.rows, webcam.cols, webcam.type());

		// Get the Perspective Transform Matrix i.e. lambda
		zoom_lambda = cv::getPerspectiveTransform(zoomQuad, webcamQuad);

		// Apply the Perspective Transform just found to the src image
		cv::warpPerspective(webcam, webcam, zoom_lambda, webcam.size());

		// create the mapped_frame 
		*mapped_frame = cv::Mat::zeros(map_height, map_width, webcam.type());

		cv::Mat webcam_lambda = getMapLambda(&webcam);


		// Apply the Perspective Transform just found to the src image
		cv::warpPerspective(webcam, *mapped_frame, webcam_lambda, mapped_frame->size());
	}

	//sets a default frame to a mapping of the webcam.
	cv::Mat getMappedFrame() {
		cv::Mat mappedFrame;
		getMappedFrame(&mappedFrame);
		return mappedFrame;
	}

	//sets the size of the map.
	void setMapSize(int w, int h) { // this is the projection of the webcam to an arbitrary size
		map_height = h;
		map_width = w;
		resetMapping();
	}

	//do you want your webcam to be a camera, or a mirror?
	void setFlip(int flip) { // set 1 to flip over the webcam
		flip_webcam = flip;
	}

	//get the frame
	int getFrame(cv::Mat *frame, int loops = 0) {
		if (cam_id == 0) {
			if (cam0->isOpened()) {  // check if we succeeded
				cam0->read(*frame);
				if (flip_webcam) { flip(*frame, *frame, -1); } // horizontal and vertically

				if (loops > 0) {
					cv::Mat temp_webcam;
					for (int i = 0; i < loops; i++) {
						//printf("get frame loop %i\n",i);
						getFrame(&temp_webcam); // get a new frame from camera
						cv::addWeighted(temp_webcam, .1, *frame, .9, 0.0, *frame);
					}
				}

				// The 4 points that select quadilateral on the input , from top-left in clockwise order
				// These four pts are the sides of the rect box used as input (used for mapping to another frame/matrix)
				webcamQuad[0] = cv::Point2f(0, 0);
				webcamQuad[1] = cv::Point2f(frame->cols - 0, 0);
				webcamQuad[2] = cv::Point2f(frame->cols - 0, frame->rows - 0);
				webcamQuad[3] = cv::Point2f(0, frame->rows - 0);

				return 1;
			}
			else {
				printf("Cam 0 didn't open\n");
			}
		}
		else {
			if (cam1->isOpened()) {  // check if we succeeded
				cam1->read(*frame);
				if (flip_webcam) { flip(*frame, *frame, -1); } // horizontal and vertically

				if (loops > 0) {
					cv::Mat temp_webcam;
					for (int i = 0; i < loops; i++) {
						getFrame(&temp_webcam); // get a new frame from camera
						cv::addWeighted(temp_webcam, .1, *frame, .9, 0.0, *frame);
					}
				}
				return 1;
			}
			else {
				printf("Cam 1 didn't open\n");
			}
		}
		return 0;
	}

	Webcam() { // constructor
		cam_id = 0;
		map_width = 600;
		map_height = 400;
		flip_webcam = 0;
		webcam_corner = 0;
		cam0 = new cv::VideoCapture(0); // open the default camera
		cam1 = new cv::VideoCapture(1); // open cam 1
		if (!cam0->isOpened()) return;
		resetMapping();
	}
	
	//deconstructor
	~Webcam() {
	}

};

