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

#define STEP 5
#define JITTER 3
#define RAIO 3

int main(int argc, char* argv[])
{
    vector<int> yrange;
    vector<int> xrange;

    Mat image, frame, points;

    int width, height, gray;
    int x, y;
    string image_file;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./8_canny.cpp config_file p1_x p1_y p2_x p2_y\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q8_image", image_file);

    image = imread(image_file, CV_LOAD_IMAGE_GRAYSCALE);
    if (!image.data) {
        cout << "nao abriu" << image_file << endl;
        exit(0);
    }
    srand(time(0));

    width = image.size().width;
    height = image.size().height;

    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++) { xrange[i] = xrange[i] * STEP + STEP / 2; }

    for (uint i = 0; i < yrange.size(); i++) { yrange[i] = yrange[i] * STEP + STEP / 2; }

    points = Mat(height, width, CV_8U, Scalar(255));

    random_shuffle(xrange.begin(), xrange.end());

    for (auto i : xrange) {
        random_shuffle(yrange.begin(), yrange.end());
        for (auto j : yrange) {
            x = i + rand() % (2 * JITTER) - JITTER + 1;
            y = j + rand() % (2 * JITTER) - JITTER + 1;
            gray = image.at<uchar>(x, y);
            circle(points, cv::Point(y, x), RAIO, CV_RGB(gray, gray, gray), -1, CV_AA);
        }
    }
    imwrite("../assets/result/pontos.jpg", points);
    return 0;
}