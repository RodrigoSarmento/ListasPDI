#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    Mat image;
    int width, height;
    vector<Mat> planes;
    Mat histR, histG, histB;
    int nbins = 64;
    float range[] = {0, 255};
    const float* histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;
    string video_name;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./4_histogram config_file\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q4_video", video_name);

    VideoCapture cap(video_name);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins / 2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));

    while (1) {

        cap >> image;
        // If the frame is empty, break immediately
        if (image.empty()) break;

        split(image, planes);
        calcHist(&planes[0], 1, 0, Mat(), histR, 1, &nbins, &histrange, uniform, acummulate);
        calcHist(&planes[1], 1, 0, Mat(), histG, 1, &nbins, &histrange, uniform, acummulate);
        calcHist(&planes[2], 1, 0, Mat(), histB, 1, &nbins, &histrange, uniform, acummulate);

        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
        normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
        normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

        histImgR.setTo(Scalar(0));
        histImgG.setTo(Scalar(0));
        histImgB.setTo(Scalar(0));

        for (int i = 0; i < nbins; i++) {
            line(histImgR, Point(i, histh), Point(i, histh - cvRound(histR.at<float>(i))), Scalar(0, 0, 255), 1, 8, 0);
            line(histImgG, Point(i, histh), Point(i, histh - cvRound(histG.at<float>(i))), Scalar(0, 255, 0), 1, 8, 0);
            line(histImgB, Point(i, histh), Point(i, histh - cvRound(histB.at<float>(i))), Scalar(255, 0, 0), 1, 8, 0);
        }
        histImgR.copyTo(image(Rect(0, 0, nbins, histh)));
        histImgG.copyTo(image(Rect(0, histh, nbins, histh)));
        histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));
        imshow("image", image);

        // Press  ESC on keyboard to exit
        key = waitKey(30);
        if (key == 27) break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}