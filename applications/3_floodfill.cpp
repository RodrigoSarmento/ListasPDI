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

    Point p;
    image = imread(argv[1], IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "imagem nao carregou corretamente\n";
        return (-1);
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

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
                // achou um objeto
                nobjects++;
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, 150);
                cv::imshow("image", image);
                cv::waitKey();
            }
        }
    }

    // Inverting the color, now the black background will be white
    cv::floodFill(image, cv::Point(0, 0), 255);
    cv::imshow("image", image);
    cv::waitKey();

    // What is left as black are the bubles with holes, so lets find those
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                cv::imshow("image", image);
                cv::waitKey();
                // achou um objeto
                nobjects_with_holes++;
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, 255);
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
            cout << "entrou\n";
            // circle(image, Point(j, 0), 5, Scalar(150, 0, 0));

            // achou um objeto
            p.x = j;
            p.y = i;
            // preenche o objeto com o contador
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
            cout << "entrou\n";
            // circle(image, Point(j, 0), 5, Scalar(150, 0, 0));

            // achou um objeto
            p.x = i;
            p.y = j;
            // preenche o objeto com o contador
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