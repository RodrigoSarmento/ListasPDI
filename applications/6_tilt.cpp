#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat mask(3, 3, CV_32F), mask1;
Mat imageFiltered, result;
Mat image1, image2;

double alfa;
double l1, l2;

int y_max;
int height = 0, center = 0, decayValue = 1;
int barRegion = 0, decaySlider = 0, barCenter = 0;

/**
 * Blur Image
 */
void blurImage();

/**
 * Set decayValue
 */
void setDecayFocus(int, void*);
/**
 * Set center height of region focus
 */
void setCenterHeight(int, void*);
/**
 * Set Center position of center focus region
 */
void setCenterPosition(int, void*);

/**
 * Calculate and show focus
 */
void calculateFocus();

int main(int argc, char* argv[])
{
    char TrackbarName[50];
    string image_name;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./6_tilt config_file\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q6_exercise", image_name);

    image1 = imread(image_name);  // Open Image
    y_max = image1.size().height; // Get height

    image2 = image1.clone();

    for (int i = 1; i < 10; i = i + 2) { GaussianBlur(image2, imageFiltered, Size(i, i), 0, 0); }

    // A variável 'result' guarda a imagem borrada
    imageFiltered.convertTo(result, CV_8U);

    namedWindow("Focus", 1);

    sprintf(TrackbarName, "Decay: ");
    createTrackbar(TrackbarName, "Focus", &decaySlider, 100, setDecayFocus);
    setDecayFocus(decaySlider, 0);

    sprintf(TrackbarName, "Height");
    createTrackbar(TrackbarName, "Focus", &barRegion, 100, setCenterHeight);
    setCenterHeight(barRegion, 0);

    sprintf(TrackbarName, "Center Position");
    createTrackbar(TrackbarName, "Focus", &barCenter, 100, setCenterPosition);
    setCenterPosition(barCenter, 0);

    waitKey();
    return 0;
}

// Ajuste para regular a força de decayValue da região borrada
void setDecayFocus(int, void*)
{
    // O decayValue mínimo será 1 devido o cálculo do alfa
    decayValue = (double)decaySlider + 1;
    calculateFocus();
}

// Ajuste para regular a height da região central que entrará em foco
void setCenterHeight(int, void*)
{
    // Calculo do center da imagem desejado
    height = (double)(barRegion * y_max) / 100;
    calculateFocus();
}

// Ajuste para regular a posição vertical do center da região que entrará em foco
void setCenterPosition(int, void*)
{
    center = (double)(barCenter * y_max) / 100;
    calculateFocus();
}

void calculateFocus()
{
    // Calculo de l1 e l2 (linhas cujo valor de αlfa assume valor em torno de 0.5)
    l1 = center - height / 2;
    l2 = center + height / 2;

    for (int i = 0; i < result.size().height; i++) {
        alfa = 0.5 * (tanh((i - l1) / decayValue) - tanh((i - l2) / decayValue));
        addWeighted(image1.row(i), alfa, result.row(i), 1.0 - alfa, 0.0, image2.row(i));
    }

    imshow("Focus", image2);
    imwrite("focusResult.png", image2);
}

void blurImage() {}