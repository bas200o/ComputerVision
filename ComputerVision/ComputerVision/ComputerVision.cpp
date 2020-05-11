// ************************************************************************************************************************************************
// ********************************************** Camera ******************************************************************************************
// ************************************************************************************************************************************************

// Dit programma leest continue een camera beeld in en toont deze op het scherm
// Bij sommige camera's is het beeld gespiegeld. Dit kan worden opgelost 
// met de flip-functie van OpenCV.
// 
// Jan Oostindie, dd 22-2-2015

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "blobdetectionavans.h"

using namespace cv;

//Variables for slider
const int threshold_slider_max = 255;
int threshold_slider = 100;
int thresholdvalue = 100;
Mat binary_image;
Mat gray_image;

/**
Gets a image from the camera when the user presses ESC
*/
Mat getCameraImg()
{
	//Open camera 0 
	VideoCapture cap(0);

	//Mat object for return type
	Mat img;

	while (1)
	{
		namedWindow("Press esc to take picture", CV_WINDOW_AUTOSIZE);

		bool success = cap.read(img);

		flip(img, img, 3);

		imshow("Press esc to take picture", img);

		if (!success)
		{
			std::cout << "not able to read img" << std::endl;
			return Mat();
		}

		if (waitKey(1) == 27) {
			destroyWindow("Press esc to take picture");
			break;
		}

	}
	return img;
}


// Callback function for trackbar events
void on_trackbar(int, void*)
{
	thresholdvalue = threshold_slider;
	threshold(gray_image, binary_image, thresholdvalue, 1, CV_THRESH_BINARY);
	imshow("Press esc to threshold picture", binary_image * 255);
}


int main(int argc, char* argv[])
{
	Mat image = getCameraImg();
	namedWindow("Press esc to threshold picture", CV_WINDOW_AUTOSIZE);

	//convert to grey value image
	cvtColor(image, gray_image, CV_BGR2GRAY);
	
	//convert to binary image
	threshold(gray_image, binary_image, 100, 1, CV_THRESH_BINARY_INV);

	imshow("Press esc to threshold picture", binary_image*255);
	createTrackbar("Threshold", "Press esc to threshold picture", &threshold_slider, threshold_slider_max, on_trackbar);
	on_trackbar(threshold_slider, 0);

	while (true)
	{
		if (waitKey(1) == 27) {
			destroyWindow("Press esc to threshold picture");
			break;
		}
	}

	namedWindow("cameraPicture", CV_WINDOW_AUTOSIZE);
	imshow("cameraPicture", binary_image);

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	Mat dilation_dst, erosion_dst;

	for (int i = 1; i < 10; i++) {
		erode(binary_image, erosion_dst, element);

		imshow("cameraPicture", erosion_dst * 255);
		binary_image = erosion_dst;
	}

	Mat blobImage;
	Mat binary16S;
	erosion_dst.convertTo(binary16S, CV_16S);

	vector<Point2d*> firstpixelVec;
	vector<Point2d*> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary16S, blobImage, firstpixelVec, posVec, areaVec);

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Print console info
	std::cout << "Found BLOBs = " << firstpixelVec.size() << std::endl;
	for (int i = 0; i < firstpixelVec.size(); i++) {
		std::cout << "BLOB " << i + 1 << std::endl;
		std::cout << "area = " << areaVec[i] << std::endl;
	}

	waitKey(0);

	return 0;
}