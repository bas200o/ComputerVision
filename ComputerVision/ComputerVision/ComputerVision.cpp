// Laden, omzetten naar grijswaarde beeld en opslaan van een beeld 
// Uitleg: http://docs.opencv.org/doc/tutorials/introduction/load_save_image/load_save_image.html
// Jan Oostindie, dd 22-1-2015

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "blobdetectionavans.h"



using namespace cv;
using namespace std;


/// Globale Variabelen voor de slider 
const int threshold_slider_max = 255;
int threshold_slider = 100;
int thresholdvalue = 100;

Mat gray_image;
Mat binaryx;


// Callback functie voor afhandeling trackbar events
void on_trackbar(int, void*)
{
	// waarde ophalen van de slider
	thresholdvalue = threshold_slider;

	// met verkregen waarde een operatie uitvoeren
	threshold(gray_image, binaryx, thresholdvalue, 1, CV_THRESH_BINARY);

	// nieuwe resultaat tonen
	imshow("binair beeld", binaryx * 255);
}


int main()
{


	// Mat is een class voor objecten waarin een afbeelding kan worden opgeslagen.
	Mat image = imread("C:/Users/baswi/Documents/git/ComputerVision/ComputerVision/Images/ballen.jpg");

	// Controleer of alles goed is gegaan
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}







	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: met succes geconverteerd naar grijswaarde beeld." << endl;

	//////////////////////////////////////////////////////////////////////////

	// Grijswaarde afbeelding thresholden
	Mat binaryImage;
	int thresholdvalue = 200;
	threshold(gray_image, binaryImage, thresholdvalue, 1, CV_THRESH_BINARY_INV);
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", gray_image);
	waitKey(0);

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo(binary16S, CV_16S);

	// functie labelBLOBs doet hetzelfde als Label Blobs in VisionLab; input is een
	// binair beeld. Output is een image waarin alle pixels van elke blob voorzien zijn van 
	// volgnummer.
	Mat labeledImage;
	cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;

	// functie show16SImageStretch beeld elke image af op 0 - 255 zodat je vrijwel altijd
	// wel een redelijke view krijgt op de data.
	// (show16SImageClip laat alle pixels met een waarde tussen 0 en 255 zien. Waardes onder 0
	// worden op 0 afgebeeld, waardes boven 255 worden op 255 afgebeeld.)
	show16SImageStretch(labeledImage, "Labeled Image");

	imwrite("c:\\dump\\labeledImage.jpg", labeledImage);

	// labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van 
	// alle BLOBs de volgende informatie terug:
	// - coordinaten van het eerste pixel van de BLOB
	// - coordinaten van het zwaartepunt van de BLOB.
	// - area van de BLOB
	Mat labeledImage2;
	vector<Point2d*> firstpixelVec2;
	vector<Point2d*> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);
	show16SImageStretch(labeledImage2, "Labeled Image 2");

	cout << endl << "*******************************************" << endl << endl;

	// Toon alle informatie in de console 
	cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << endl;
	for (int i = 0; i < firstpixelVec2.size(); i++) {
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec2[i]->x << "," << firstpixelVec2[i]->y << ")" << endl;
		cout << "centre = (" << posVec2[i]->x << "," << posVec2[i]->y << ")" << endl;
		cout << "area = " << areaVec2[i] << endl;
	}

	cout << endl << "*******************************************" << endl << endl;

	// Met de functie labelBLOBsInfo kun je ook een threshold instellen voor de oppervlakte
	// van de BLOBs.

	Mat labeledImage3;
	vector<Point2d*> firstpixelVec3;
	vector<Point2d*> posVec3;
	vector<int> areaVec3;
	int minArea = 15000;
	int maxArea = 16000;
	labelBLOBsInfo(binary16S, labeledImage3,
		firstpixelVec3, posVec3, areaVec3, minArea, maxArea);
	show16SImageStretch(labeledImage3, "Labeled Image 3");

	cout << endl << "*******************************************" << endl << endl;

	// Toon alle informatie in de console 
	cout << "Aantal gevonden BLOBs = " << firstpixelVec3.size() << endl;
	for (int i = 0; i < firstpixelVec3.size(); i++) {
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec3[i]->x << "," << firstpixelVec3[i]->y << ")" << endl;
		cout << "centre = (" << posVec3[i]->x << "," << posVec3[i]->y << ")" << endl;
		cout << "area = " << areaVec3[i] << endl;
	}

	cout << endl << "*******************************************" << endl << endl;

	// Nog een manier om het aantal BLOBs te verkrijgen...
	cout << "Total number of BLOBs = " << maxPixelImage(labeledImage3) << endl;

	string pipo;
	cin >> pipo;

	return 0;

}