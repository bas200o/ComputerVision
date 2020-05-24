/*
	This program uses a usb camera to read objects from the video feed.

	Bas Winkelhof, 2020-5-12
*/

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "ColorDetection.h"

using namespace cv;

int main(int argc, char* argv[])
{
	std::cout << "Visionapp3 by Bas Winkelhof" << std::endl;

	VideoCapture cap(1);

	if (!cap.isOpened())
	{
		std::cout << "Failed to open video input" << std::endl;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::cout << "Frame size : " << dWidth << " x " << dHeight << std::endl;

	namedWindow("Camera source", CV_WINDOW_AUTOSIZE);

	Mat frame, redFrame;
	ColorDetection detection;

	while (true)
	{
		bool bSuccess = cap.read(frame);

		if (!bSuccess)
		{
			std::cout << "Cannot read a frame from video stream" << std::endl;
			break;
		}

		flip(frame, frame, 3);

		imshow("Camera source", frame);

		redFrame = detection.redColorDetection(frame);

		detection.trackObject(frame, redFrame);
		
		namedWindow("Red objects", WINDOW_AUTOSIZE);
		imshow("Red objects", frame);

		if (cv::waitKey(1) == 27)
		{
			std::cout << "Esc key is pressed by user quitting application" << std::endl;
			destroyAllWindows();
			break;
		}
	}

	return 0;
}

