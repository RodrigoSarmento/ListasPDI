#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{

    Mat image;
    Vec3b val;
    string image_file;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./2_2regioes config_file \n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q2_image", image_file);

    image = imread(image_file, IMREAD_COLOR);
    if (!image.data) cout << "Couldn't open image" << image_file << endl;

    Mat image_quad_1(image, Range(0, image.rows / 2), Range(0, image.cols / 2));
    Mat image_quad_2(image, Range(0, image.rows / 2), Range(image.cols / 2, image.cols));
    Mat image_quad_3(image, Range(image.rows / 2, image.rows), Range(0, image.cols / 2));
    Mat image_quad_4(image, Range(image.rows / 2, image.rows), Range(image.cols / 2, image.cols));

    Mat3b final_image(image.rows, image.cols, Vec3b(0, 0, 0));

    image_quad_4.copyTo(final_image(Range(0, image.rows / 2), Range(0, image.cols / 2)));
    image_quad_3.copyTo(final_image(Range(0, image.rows / 2), Range(image.cols / 2, image.cols)));
    image_quad_2.copyTo(final_image(Range(image.rows / 2, image.rows), Range(0, image.cols / 2)));
    image_quad_1.copyTo(final_image(Range(image.rows / 2, image.rows), Range(image.cols / 2, image.cols)));

    imshow("image", final_image);
    cv::waitKey();

    return 0;
}