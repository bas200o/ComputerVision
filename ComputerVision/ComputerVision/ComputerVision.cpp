// Load, Convert and adjust blob area sizes
// Bas Winkelhof, dd 25-4-2020

#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "blobdetectionavans.h"

using namespace cv;

/// Globale Variabelen voor de slider 
int area_slider_max = 0;
int default_area_sider = 4000;
int areasizevalue = 100;
int minAreaSize = 0;
int maxAreaSize;

Mat blobImage;
Mat binary16S;

// Callback function for both trackbars
void on_trackbar(int, void*)
{
	vector<Point2d*> firstPixelVec;
	vector<Point2d*> posVec;
	vector<int> areaVec;

	labelBLOBsInfo(binary16S, blobImage, firstPixelVec, posVec, areaVec, minAreaSize, maxAreaSize);
	show16SImageStretch(blobImage, "Blob Image");
}


int main(int argc, char* argv[])
{
	Mat image;
	// See: project properties -> configuration properties -> debugging -> command arguments
	if (argc != 2)
	{
		std::cout << "no image found in arguments using default image." << std::endl;
		image = imread("../Images/ballen.jpg");
	}
	else
	{
		image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	// Ckeck image data
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Covert the image to grayscale
	std::cout << "Converting image to grayscale... ";
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	std::cout << "oke" << std::endl;

	// Set Threshold for image grayscale
	Mat binaryImage;
	int thresholdvalue = 200;
	threshold(gray_image, binaryImage, thresholdvalue, 1, CV_THRESH_BINARY_INV);

	// Covert image to a 16 bit bithdepth image
	binaryImage.convertTo(binary16S, CV_16S);

	vector<Point2d*> firstpixelVec;
	vector<Point2d*> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary16S, blobImage, firstpixelVec, posVec, areaVec);

	std::cout <<std::endl << "*******************************************" << std::endl << std::endl;

	// Print console info
	std::cout << "Found BLOBs = " << firstpixelVec.size() << std::endl;
	for (int i = 0; i < firstpixelVec.size(); i++) {
		std::cout << "BLOB " << i + 1 << std::endl;
		std::cout << "area = " << areaVec[i] << std::endl;

		if (areaVec[i] > area_slider_max)
			area_slider_max = areaVec[i];
	}

	maxAreaSize = area_slider_max;

	namedWindow("Blob Image");
	createTrackbar("Area Min", "Blob Image", &minAreaSize, area_slider_max, on_trackbar);
	createTrackbar("Area Max", "Blob Image", &maxAreaSize, area_slider_max, on_trackbar);
	on_trackbar(minAreaSize, 0);

	std::cin.get();
	return 0;
}