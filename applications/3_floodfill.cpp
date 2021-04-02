#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void cleanEdges(Mat& image, int width, int height);

int main(int argc, char* argv[])
{
    Mat image, realce;
    int width, height;
    int nobjects, nobjects_with_holes;
    string image_file;
    Point p;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./3_floodfill config_file \n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q3_image", image_file);

    image = imread(image_file, IMREAD_GRAYSCALE);

    if (!image.data) {
        cout << "imagem nao carregou corretamente\n";
        return (-1);
    }

    width = image.cols;
    height = image.rows;
    cout << width << "x" << height << endl;

    p.x = 0;
    p.y = 0;

    // Removing bubles in the edges of image
    cleanEdges(image, width, height);
    imshow("image", image);
    waitKey();

    // Detecting all bubles in image and using floodfill to paint as gray
    nobjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // Found a bubble
                nobjects++;
                p.x = j;
                p.y = i;
                // Using floodFill
                floodFill(image, p, 150);
                imshow("image", image);
                waitKey();
            }
        }
    }

    // Inverting the color, now the black background will be white
    floodFill(image, Point(0, 0), 255);
    imshow("image", image);
    waitKey();

    // What is left as black are the bubles with holes, so lets find those
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                imshow("image", image);
                waitKey();
                // Found a bubble
                nobjects_with_holes++;
                p.x = j;
                p.y = i;
                // Using floodFill
                floodFill(image, p, 255);
            }
        }
    }

    // Now I have the count of all bubles and the count of bubles with holes
    cout << "a figura tem " << nobjects_with_holes << " bolhas com buracos\n";
    cout << "a figura tem " << nobjects - nobjects_with_holes << " bolhas sem buracos\n";

    return 0;
}

void cleanEdges(Mat& image, int width, int height)
{
    int i = 0;
    Point p;

    p.x = 0;
    p.y = 0;

    for (int j = 0; j < width; j++) {
        if (image.at<uchar>(i, j) == 255) {
            p.x = j;
            p.y = i;
            floodFill(image, p, 0);
        }
        if (j == width - 1) {
            if (i == height - 1) break;
            j = 0;
            i = height - 1;
        }
    }

    i = 0;
    for (int j = 0; j < height; j++) {
        if (image.at<uchar>(j, i) == 255) {
            p.x = i;
            p.y = j;
            floodFill(image, p, 0);
        }
        if (j == height - 1) {
            if (i == height - 1) break;
            j = 0;
            i = height - 1;
        }
    }
}

/**
 * Respostas:
 * Uma solução seria na hora de contar os objetos transformar a imagem em colorida criando
 * mais canais de cores e assim mais possibilidades para rotulação.
 *
 * Eu segui a lógica de continuar usando o código do floodfill para contar os objetos
 * porém eu pinto todos eles de 254(qualquer valor que não preto ou branco)
 * mas dps disso eu uso o floodfill para inverter as cores da imagem a partir do preto do pixel
 * 0,0. Dessa maneira os buracos pretos serão mantidos na imagem.
 * Então eu faço um segundo processo de varrer a imagem igual no primeiro passo, porém
 * buscando a cor preta ao invés da branca, e cada vez que eu acho uma imagem eu uso o floodfill
 * e incremento o contador.
 *
 * No final eu vou ter no novo contador a quantidade de bolas brancas com um buraco
 * e o total de bolas brancas, fazendo o total de bolas brancas - total de bolas brancas com
 * o buraco eu tenho o total de bolas brancas sem buraco.
 *
 * *** Ainda tem q ver oq fazer para os casos dos corners
 */