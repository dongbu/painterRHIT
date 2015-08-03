
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <assert.h>

using namespace std;
using namespace cv;

pair<Mat, Mat> discoverColors(Mat image, int numColors) {
	Mat src = image;
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y, x)[z];

	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, numColors, labels,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 1000, 0.001),
		attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++) {
			int cluster_idx = labels.at<int>(y + x*src.rows, 0);
			new_image.at<Vec3b>(y, x)[0] = (uchar)centers.at<float>(cluster_idx, 0);
			new_image.at<Vec3b>(y, x)[1] = (uchar)centers.at<float>(cluster_idx, 1);
			new_image.at<Vec3b>(y, x)[2] = (uchar)centers.at<float>(cluster_idx, 2);
		}

	imshow("clustered image", new_image);
	return make_pair(centers, labels);
}

vector<pair<vector<Point>, int> > imageToStroke(Mat image) {
	vector<pair<vector<Point>, int> > output;

	vector<vector<Point> > paths;
	vector<int> color_indicies;

	//Convert to greyscale, and do a blur to reduce noise.
	Mat greyscale, blurred;
	cvtColor(image, greyscale, COLOR_RGB2GRAY);
	blur(greyscale, blurred, Size((int)(greyscale.cols / 200.0), (int)(greyscale.cols / 200.0)));

	//Get the Edges, magnitude and direction
	Mat edgeMagnitude, edgeDirection, grad_x, grad_y;
	Scharr(blurred, grad_x, CV_32F, 1, 0, 3);
	Scharr(blurred, grad_y, CV_32F, 0, 1, 3);
	cartToPolar(grad_x, grad_y, edgeMagnitude, edgeDirection);

	//Display the edges 
	//*
	vector<Mat> channels;
	Mat value = Mat::ones(edgeMagnitude.size(), CV_32F);
	channels.push_back((edgeDirection / CV_2PI) * 360); 
	channels.push_back(value);
	double max;
	minMaxIdx(edgeMagnitude, 0, &max);
	channels.push_back(edgeMagnitude / max);
	Mat edgeImage;
	merge(channels, edgeImage);
	cvtColor(edgeImage, edgeImage, COLOR_HSV2RGB);
	imshow("edges", edgeImage);
	//*/

	//Discover the colors we'll be using
	pair<Mat, Mat> colors_labels = discoverColors(image, 4);
	Mat colors = colors_labels.first;
	//Also store which color each pixel is nearest to, to save time
	//when we need to figure out what color to paint an area
	Mat labels = colors_labels.second;
	//////////////////////////////////////REMEMBER ME//////////////////////////////////////////////////////////
	labels = labels.reshape(0, image.size().width);
	//////////////////////////////////////size().width 0--> size().height //////////////////////////////////////////////////////////


	//segment the iamge into small areas
	const float n = 3;
	//Loop through n*n cells in image (cells may have varying dimensions)
	for (int i = 0; i <= n + 1; i++) {
		int colStart = i*floor(greyscale.cols / n);
		int colEnd = ((i + 1)*floor(greyscale.cols / n)) > greyscale.cols ? greyscale.cols : (i + 1)*floor(greyscale.cols / n);
		for (int j = 0; j <= n + 1; j++) {
			int rowStart = j*floor(greyscale.rows / n);
			int rowEnd = ((j + 1)*floor(greyscale.rows / n)) > greyscale.rows ? greyscale.rows : ((j + 1)*floor(greyscale.rows / n));
			//Loop through pixels in the cell
			int numPixels = (rowEnd - rowStart) * (colEnd - colStart);
			float avgDir = 0;
			vector<int> colorPop(colors.size().height);
			for (int col = colStart; col < colEnd; col++) {
				
				for (int row = rowStart; row < rowEnd; row++) {
					
					avgDir += edgeDirection.at<float>(row, col) * edgeMagnitude.at<float>(row, col) / numPixels;
					int num = labels.at<float>(row, col);
					colorPop[num] += 1;
				}
			}
			int colorIndex = 0;
			//Find which color was most popular in that cell
			for (int k = 0; k < colorPop.size(); k++) {
				if (colorPop[k] >= colorPop[colorIndex]) {
					colorIndex = k;
				}
			}

			//Add the line with the correct color and direction to output
			vector<Point> tmp;
			double lineLength = 0.01;
			tmp.push_back(Point((i - n / 2) / n + lineLength*cos(avgDir), (j - n / 2) / n + lineLength*sin(avgDir)));
			tmp.push_back(Point((i - n / 2) / n, (j - n / 2) / n));
			output.push_back(make_pair(tmp, colorIndex));
		}
	}
	return output;
}