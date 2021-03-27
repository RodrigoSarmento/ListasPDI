#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
    cv::Mat image, realce;
    int width, height;
    int nobjects, nobjects_with_holes;

    cv::Point p;
    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "imagem nao carregou corretamente\n";
        return (-1);
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    // busca objetos presentes
    nobjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            if (image.at<uchar>(i, j) == 255) {
                // achou um objeto
                nobjects++;
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, 254);
            }
        }
    }
    std::cout << "a figura tem " << nobjects << " bolhas\n";
    cv::equalizeHist(image, realce);
    cv::imshow("image", image);
    cv::imshow("realce", realce);
    cv::imwrite("labeling.png", image);
    cv::waitKey();

    cv::floodFill(image, cv::Point(0, 0), 255);
    cv::imshow("image", image);
    cv::waitKey();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                cv::floodFill(image, cv::Point(0, 0), 255);
                cv::imshow("image", image);
                cv::waitKey();
                // achou um objeto
                nobjects_with_holes++;
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, 125);
            }
        }
    }

    std::cout << "a figura tem " << nobjects_with_holes << " bolhas com buracos\n";

    return 0;
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