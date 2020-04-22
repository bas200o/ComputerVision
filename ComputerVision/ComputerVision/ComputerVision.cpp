
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat image = imread("X:/settings/laptop background/boemboemboem.jpg");;

	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Laat de afbeelding zien in een apart window
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image);

	// Wacht op een muiskklik in het window van de afbeelding
	waitKey(0);

	// Ruim alle aangemaakte windows weer op.
	destroyAllWindows();

	return 0;
}
