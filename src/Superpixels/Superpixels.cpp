/*
 * Superpixels.cpp.
 *
 * Written by: Pascal Mettes.
 * Modified by: Benjamin Paul.
 *
 * This file creates an over-segmentation of a provided image based on the SLIC
 * superpixel algorithm, as implemented in slic.h and slic.cpp.
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>
using namespace std;
using namespace cv;

#include "slic.h"

int main(int argc, char *argv[]) {

    if ( argc != 4 )
    {
        printf("usage: Superpixels.exe <Image_Path> <Number of Superpixels> <Weight>\n");
        return -1;
    }

    /* Load the image and convert to Lab color space. */
    IplImage *image = cvLoadImage(argv[1], 1);
    IplImage *lab_image = cvCloneImage(image);
    IplImage *meanedImage = cvCloneImage(image);
    cvCvtColor(image, lab_image, CV_BGR2Lab);

    /* get the number of superpixels and weight-factors from the user. */
    int w = image->width, h = image->height;
    int nr_superpixels = atoi(argv[2]);
    int nc = atoi(argv[3]);

    double step = sqrt((w * h) / (double) nr_superpixels);


    /* Perform the SLIC superpixel algorithm. */
    Slic slic;
    slic.generate_superpixels(lab_image, step, nc);
    slic.create_connectivity(lab_image);

    /* Display the contours and show the result. */
    slic.colour_with_cluster_means(meanedImage);
    slic.display_contours(lab_image, CV_RGB(255,0,0));
//    namedWindow( "input");
//    imshow("input",Mat(image));
//    namedWindow("LAB color space");
//    imshow("LAB color space", Mat(lab_image));
//    namedWindow("result");
//    imshow("result", Mat(meanedImage));

    cvShowImage("input", image);
    cvShowImage("LAB color space", lab_image);
    cvShowImage("result", meanedImage);

    cvWaitKey(0);
//    cvSaveImage(argv[4], image);
}
