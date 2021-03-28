#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void motionDetector(Mat hist, Mat& previouslyHist);

int main(int argc, char* argv[])
{
    Mat image;
    int width, height;
    Mat hist;
    Mat previouslyHist;
    int nbins = 64;
    float range[] = {0, 255};
    const float* histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;
    string video_name;
    int channels[] = {0};
    int i = 0;

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
    Mat histImg(histh, histw, CV_8UC3, Scalar(0, 0, 0));

    while (cap.isOpened()) {

        cap >> image;
        // If the frame is empty, break immediately
        if (image.empty()) break;

        // Converting image to gray
        cvtColor(image, image, COLOR_BGR2GRAY);

        // Calculate Hist
        calcHist(&image, 1, channels, Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
        normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

        if (i == 0)
            previouslyHist = hist.clone();
        else
            motionDetector(hist, previouslyHist);

        histImg.setTo(Scalar(0));

        // Show image
        for (int i = 0; i < nbins; i++) {
            line(histImg, Point(i, histh), Point(i, histh - cvRound(hist.at<float>(i))), Scalar(0, 0, 255), 1, 8, 0);
        }
        Mat equalized;

        imshow("image", image);
        imshow("image_hist", histImg);

        // Press  ESC on keyboard to exit
        key = waitKey(30);
        if (key == 27) break;
        i++;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}

void motionDetector(Mat hist, Mat& previouslyHist)
{
    float difference;
    int differenteBeans;
    float trono;
    for (int i = 0; i < 64; i++) {
        difference = abs(hist.at<float>(i) - previouslyHist.at<float>(i));
        if (i == 0)
            trono = difference;
        else if (difference > trono)
            trono = difference;
        if (difference >= 1) differenteBeans++; // Arbitrário
    }
    if (differenteBeans >= 16) { cout << "motion Detector\n"; } // 25% difference
    // cout << differenteBeans << endl;
    previouslyHist = hist.clone();
}