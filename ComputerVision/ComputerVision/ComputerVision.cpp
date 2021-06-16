/*
	This program uses a usb camera to read objects from the video feed.

	Bas Winkelhof, 2020-5-12
*/

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>


using namespace cv;
using std::vector;


const String window_capture_name = "Video Capture";
const String window_detection_name = "Finger Detection";

double distance_threshold = 50.0;
int counted_fingers = 0;
std::string text;

double calculateAngle(Point far, Point start, Point end);


double calculateAngle(Point far, Point start, Point end) {
    double a = sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
    double b = sqrt(pow((far.x - start.x), 2) + pow((far.y - start.y), 2));
    double c = sqrt(pow((end.x - far.x), 2) + pow((end.y - far.y), 2));
    double angle = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
    return angle;
}

cv::Mat doEdgeDetect(cv::Mat input) {
    counted_fingers = 0;
    vector<vector<Point>> Contours;
    vector<Vec4i> hier;
    vector<Point> points;
    vector<Point> far_points;

    int min_area = 35;
    int dilation_size = 12;
    cv::Mat output;


    Mat element = getStructuringElement(cv::MORPH_RECT,
        Size(2 * dilation_size + 1, 2 * dilation_size + 1),
        Point(dilation_size, dilation_size));
    blur(input, output, Size(7, 7));
    cvtColor(output, output, COLOR_BGR2HSV);

    cv::inRange(output, cv::Vec3i(0, 0, 0), cv::Vec3i(10, 255, 255), output);
    cv::threshold(output, output, 25, 255, THRESH_BINARY);
    cv::dilate(output, output, element);
    cv::erode(output, output, element);
    findContours(output, Contours, hier, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

    auto it = Contours.begin();
    while (it != Contours.end()) {
        if (contourArea(*it) < min_area) {
            it = Contours.erase(it);
        }
        else {
            it++;
        }
    }

    Mat drawing = Mat::zeros(output.size(), CV_8UC3);
    vector<vector<Point> >hull(Contours.size());
    vector<vector<int> > hullsI(Contours.size());
    vector<vector<Vec4i>> defects(Contours.size());

    for (int i = 0; i < Contours.size(); i++)
    {
        try {
            convexHull(Contours[i], hull[i], false, false);
            convexHull(Contours[i], hullsI[i], true, false);
            if (hullsI[i].size() > 3)
            {
                convexityDefects(Contours[i], hullsI[i], defects[i]);
            }
        }
        catch (cv::Exception e) {

        }
        drawContours(drawing, Contours, i, cv::Scalar(255, 0, 0));
        drawContours(drawing, hull, i, cv::Scalar(255, 0, 0));
    }

    for (int i = 0; i < Contours.size(); ++i)
    {
        for (const Vec4i& v : defects[i])
        {
            float depth = v[3] / 256;
            if (depth > 1)
            {
                int startidx = v[0]; Point ptStart(Contours[i][startidx]);
                int endidx = v[1]; Point ptEnd(Contours[i][endidx]);
                int faridx = v[2]; Point ptFar(Contours[i][faridx]);

                if (calculateAngle(ptFar, ptStart, ptEnd) > 1.5) {
                    continue;
                }

                points.push_back(ptStart);
                points.push_back(ptEnd);
                far_points.push_back(ptFar);
                circle(drawing, ptFar, 4, Scalar(0, 255, 0), 2);
            }
        }
    }


    for (auto it = points.begin(); it != points.end(); it++) {
        for (auto it_2 = points.begin(); it_2 != points.end(); it_2++) {
            if (it != it_2) {
                double distance = sqrt(pow(it->x - it_2->x, 2) + pow(it->y - it_2->y, 2));
                if (distance < distance_threshold) {
                    it = points.erase(it);
                    it_2 = points.begin();
                }
            }
            else {
                continue;
            }
        }
    }
    counted_fingers = points.size();

    return drawing;
}

int main()
{

    VideoCapture cap(0);
    std::vector<std::vector<Point>> contours;
    namedWindow(window_capture_name);
    namedWindow(window_detection_name);

    Mat input, output;


    while (true) {
        cap >> input;
        output = doEdgeDetect(input);
        std::stringstream ss;
        ss << "Detected fingers: " << counted_fingers;
        cv::putText(output, ss.str(), cv::Point(0, 50), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 255, 255));
        imshow(window_capture_name, input);
        imshow(window_detection_name, output);

        char key = (char)waitKey(30);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    return 0;
}