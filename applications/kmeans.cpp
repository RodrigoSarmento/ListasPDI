#include <config_loader.h>
#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    int nClusters = 8;
    Mat rotulos;
    int nRodadas = 1;
    Mat centros;
    string image_file;
    string result_file;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./8_canny.cpp config_file p1_x p1_y p2_x p2_y\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("kmeans", image_file);

    Mat img = imread(image_file, CV_LOAD_IMAGE_COLOR);
    Mat samples(img.rows * img.cols, 3, CV_32F);
    for (int i = 0; i < 10; i++) {

        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                for (int z = 0; z < 3; z++) { samples.at<float>(y + x * img.rows, z) = img.at<Vec3b>(y, x)[z]; }
            }
        }

        kmeans(
            samples,
            nClusters,
            rotulos,
            TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001),
            nRodadas,
            KMEANS_RANDOM_CENTERS,
            centros);

        Mat rotulada(img.size(), img.type());
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                int indice = rotulos.at<int>(y + x * img.rows, 0);
                rotulada.at<Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
                rotulada.at<Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
                rotulada.at<Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
            }
        }
        imshow("clustered image", rotulada);
        result_file = "kmeans_result" + to_string(i) + ".png";
        imwrite(result_file, rotulada);
        waitKey(0);
    }
}