#include <algorithm>
#include <config_loader.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border, points;
int step = 5, jitter = 3, raio = 2;
void on_trackbar_canny(int, void*)
{
    Canny(image, border, top_slider, 3 * top_slider);
    imshow("canny", border);
}

int main(int argc, char* argv[])
{
    int width, height, gray, x, y;
    vector<int> yrange;
    vector<int> xrange;
    string image_file;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./8_canny.cpp config_file p1_x p1_y p2_x p2_y\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q8_image", image_file);

    image = imread(image_file, CV_LOAD_IMAGE_GRAYSCALE);

    sprintf(TrackbarName, "Threshold inferior", top_slider_max);

    namedWindow("canny", 1);
    createTrackbar(TrackbarName, "canny", &top_slider, top_slider_max, on_trackbar_canny);

    on_trackbar_canny(top_slider, 0);

    width = image.size().width;
    height = image.size().height;

    xrange.resize(height / step);
    yrange.resize(width / step);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++) { xrange[i] = xrange[i] * step + step / 2; }
    for (uint i = 0; i < yrange.size(); i++) { yrange[i] = yrange[i] * step + step / 2; }

    points = Mat(height, width, CV_8U, Scalar(255));

    random_shuffle(xrange.begin(), xrange.end());
    random_shuffle(yrange.begin(), yrange.end());

    for (auto i : xrange) {
        for (auto j : yrange) {
            x = i + rand() % (2 * jitter) - jitter + 1;
            y = j + rand() % (2 * jitter) - jitter + 1;
            gray = image.at<uchar>(x, y);
            circle(points, cv::Point(y, x), 4, CV_RGB(gray, gray, gray), -1, CV_AA);
        }
    }
    imshow("points", points);
    waitKey();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            gray = image.at<uchar>(i, j);
            if (border.at<uchar>(i, j) == 255 and points.at<uchar>(i, j) != 255) {
                circle(points, cv::Point(j, i), 2, CV_RGB(gray, gray, gray), -1, CV_AA);
            }
        }
    }

    imshow("points", points);
    waitKey();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            gray = image.at<uchar>(i, j);
            if (border.at<uchar>(i, j) == 255 and points.at<uchar>(i, j) != 255) {
                circle(points, cv::Point(j, i), 1, CV_RGB(gray, gray, gray), -1, CV_AA);
            }
        }
    }

    imshow("points", points);
    waitKey();
    imwrite("cannyborders.png", points);
    return 0;
}