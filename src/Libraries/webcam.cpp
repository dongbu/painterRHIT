#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "DrawWindow.cpp"
#include "windows.h"

// Packages simple drawing commands for the simulator display window
class Webcam {
protected:
	// put up here so that the webcam is always on (if not, takes time to warm up)
	// G.H. moved here so that VisualStudio could compile.
	cv::VideoCapture *cam0; // open the default camera
	cv::VideoCapture *cam1; // open cam 1

	int map_width;
	int map_height;
	int cam_id;
	int flip_webcam;
	int webcam_corner;
	int currentCamera;
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

	void setWebcamZoom(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
		zoomQuad[0] = cv::Point2f(x1, y1);
		zoomQuad[1] = cv::Point2f(x2, y2);
		zoomQuad[2] = cv::Point2f(x3, y3);
		zoomQuad[3] = cv::Point2f(x4, y4);
	}

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
		Webcam *self = static_cast<Webcam*>(userdata);
		if (event == cv::EVENT_LBUTTONDOWN) {
			printf("Setting zoom corner %d to %i,%i\n", self->webcam_corner, x, y);
			self->zoomQuad[self->webcam_corner].x = x * 2;
			self->zoomQuad[self->webcam_corner].y = y * 2;
		}
	}

	// sets the desired region of the webcam 
	void calibrateWebcam(int skip_reset = 0) {
		cv::Mat webcam;
		cv::Mat mapped_webcam; // this is the webcam mapped to the same dimensions as the final canvas pixels
		//    cv::Mat canvas; // this is the "canvas"
		char mapped_name[] = "Mapped Webcam";
		char webcam_name[] = "Webcam";
		int debug = 1;

		cv::namedWindow(webcam_name, 1);
		cv::moveWindow(webcam_name, 20, 20);
		cv::setMouseCallback(webcam_name, zoomMouseCallBackFunc, this);

		cv::namedWindow(mapped_name, 1);

		if (!skip_reset) { resetMapping(); }

		cv::Mat scaledWebcam;
		webcam_corner = 0;
		int done = 0;
		while (!done) {
			getFrame(&webcam, 3); // get a new frame from camera (blend 3 frames for better clarity)

			cv::resize(webcam, scaledWebcam, cv::Size(), 0.5, 0.5);
			cv::imshow(webcam_name, scaledWebcam);
			cv::moveWindow(mapped_name, 40 + scaledWebcam.cols, 20);

			getMappedFrame(&mapped_webcam);
			cv::imshow(mapped_name, mapped_webcam);

			int k = cv::waitKey(33);
			if (k == 27 || k == int('x')) { // Esc key to stop
				done = 1;
			}
			else if (k == int('1')) {
				webcam_corner = 0;
				if (debug) printf("Click on the desired region's upper left corner\n");
			}
			else if (k == int('2')) {
				webcam_corner = 1;
				if (debug) printf("Click on the desired region's upper right corner\n");
			}
			else if (k == int('3')) {
				webcam_corner = 2;
				if (debug) printf("Click on the desired region's lower right corner\n");
			}
			else if (k == int('4')) {
				webcam_corner = 3;
				if (debug) printf("Click on the desired region's lower left corner\n");
			}
			else if (k == int('x')) {
				done = 1;
			}
		}
		printf("Webcam calibration matrix defined.\n");
		cv::destroyWindow(mapped_name);
		cv::destroyWindow(webcam_name);
	}

	void showWebcam() {
		cv::Mat mapped_webcam;

		char mapped_name[] = "Mapped Webcam";
		cv::namedWindow(mapped_name, 1);

		int done = 0;
		while (!done) {
			getMappedFrame(&mapped_webcam);
			cv::imshow(mapped_name, mapped_webcam);
			int k = cv::waitKey(33); //stall
			if (k == 27 || k == int('x')) done = 1; //escape position
		}

		cv::destroyWindow(mapped_name);
	}

	cv::Mat getWebcamSnap(cv::Mat grid) {
		cv::Mat mapped_webcam;

		char mapped_name[] = "Mapped Webcam";
		cv::namedWindow(mapped_name, 1);

		int done = 0;
		int frozen = 0;

		//If s is pressed, frames stop updating.
		while (!done) {
			if (!frozen) {
				getMappedFrame(&mapped_webcam);
				cv::imshow(mapped_name, mapped_webcam);
			}

			int k = cv::waitKey(33); //stall
			if (k == int('s')) {
				frozen = 1;
				getMappedFrame(&mapped_webcam);
				grid = mapped_webcam;
				cv::imshow(mapped_name, mapped_webcam);
			}

			if (k == 27 || k == int('x')) done = 1; //escape position
		}

		cv::destroyWindow(mapped_name);
		return grid;
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
	void getMappedFrame(cv::Mat *mapped_frame, int loops = 0, int blackwhite = 0) {
		cv::Mat webcam;
		getFrame(&webcam, loops, blackwhite); // get a new frame from camera

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

	cv::Mat getMappedFrame() {
		cv::Mat mappedFrame;
		getMappedFrame(&mappedFrame);
		return mappedFrame;
	}

	void setMapSize(int w, int h) { // this is the projection of the webcam to an arbitrary size
		map_height = h;
		map_width = w;
		resetMapping();
	}

	void setFlip(int flip) { // set 1 to flip over the webcam
		flip_webcam = flip;
	}

	int getFrame(cv::Mat *frame, int loops = 0, int blackwhite = 0) {
		if (cam_id == 0) {
			if (cam0->isOpened()) {  // check if we succeeded
				cam0->read(*frame);
				if (flip_webcam) { flip(*frame, *frame, -1); } // horizontal and vertically
				if (blackwhite) {
					cv::Mat temp_webcam;
					cvtColor(*frame, temp_webcam, 7); // CV_BGR2GRAY=7 );
					*frame = temp_webcam.clone();
				}

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

	void switchWebcam() {
		currentCamera++;
		cam1 = new cv::VideoCapture(currentCamera);
		if (cam1->isOpened()) cam0 = cam1;
		else {
			cam0 = new cv::VideoCapture(0);
			currentCamera = 0;
		}
	}

	int colorCloseness(cv::Vec3b c1, cv::Vec3b c2) {
		return abs(c1[0] - c2[0]) + abs(c1[1] - c2[1]) + abs(c1[2] - c2[2]);
	}

	void judge(cv::Mat ideal) {
		int width, height, done;

		width = ideal.size().width;
		height = ideal.size().height;
		done = 0;

		DrawWindow EW = DrawWindow(width, height, "Governator");
		cv::Mat toGovernate = cv::Mat();

		done = 0;
		while (!done) {
			getMappedFrame(&toGovernate);
			EW.grid = toGovernate;

			int right = 0;
			int wrong = 0;

			for (int i = 0; i < width; i++) { //loop rows
				for (int j = 0; j < height; j++) { //loop columns
					cv::Vec3b desired_color = ideal.at<cv::Vec3b>(i, j); //what we're aiming for

					//don't paint over canvas we didn't put art on
					if (desired_color[0] != 255 && desired_color[1] != 255 && desired_color[2] != 255) {
						cv::Vec3b desired_color = ideal.at<cv::Vec3b>(i, j); //what we're aiming for
						cv::Vec3b webcam_color = toGovernate.at<cv::Vec3b>(i, j); //what the webcam sees

						int closeness = colorCloseness(desired_color, webcam_color);
						int cdiff = desired_color[0] - webcam_color[0] + desired_color[1] - webcam_color[1] + desired_color[2] - webcam_color[2];

						if (cdiff > 255) { cdiff = 255; }  //too far off?  just set to pure blue
						if (cdiff < -255) { cdiff = -255; } //too far off?  just set to pure red

						if (cdiff >= 0) {
							EW.setPenColor(0, 0, cdiff); //blue
						}
						else {
							EW.setPenColor(-cdiff, 0, 0); //red
						}

						EW.drawPixel(j, i); //for whatever reason, we're 90 degrees off.  (rotate, please).
						if (closeness < 50) { right++; }
						else  { wrong++; }
					}
				}
			}

			EW.setPenColor(200, 200, 200);
			EW.drawRectangle(0, 0, 200, 12, 1);
			EW.setPenColor(0, 0, 0);
			char text[50];
			sprintf(text, "%.0f%%  OK:%i TODO:%i", (double)100.*right / (right + wrong), right, wrong);
			EW.drawText(10, 10, text);
			EW.show();
			//Sleep(1); //why is this here?  Look into it later, I guess.

			int k = cv::waitKey(33);
			if (k == 27 || k == int('m')) { done = 1; }
		}
	}

	Webcam(int width = 600, int height = 600) { // constructor
		cam_id = 0;
		map_width = width;
		map_height = height;
		flip_webcam = 0;
		webcam_corner = 0;
		currentCamera = 0;
		cam0 = new cv::VideoCapture(currentCamera); // open the default camera
		if (!cam0->isOpened()) return;
		resetMapping();
	}

	~Webcam() {
	}

};

