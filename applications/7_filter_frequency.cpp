#include <config_loader.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

using namespace cv;
using namespace std;

void on_trackbar_frequency(int, void*) {}

void on_trackbar_noise_gain(int, void*) {}

void menu()
{
    cout << "e : habilita/desabilita interferencia\n"
            "m : habilita/desabilita o filtro mediano\n"
            "g : habilita/desabilita o filtro gaussiano\n"
            "p : realiza uma amostra das imagens\n"
            "s : habilita/desabilita subtração de fundo\n"
            "b : realiza uma amostra do fundo da cena\n"
            "n : processa o negativo\n";
}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image)
{
    Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cópias de tamanho desigual
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

int main(int argc, char* argv[])
{
    Mat imaginaryInput, complexImage, multsp;
    Mat padded, filter, mag;
    Mat image, imagegray, tmp, magI;
    Mat_<float> realInput, zeros, ones;
    Mat backgroundImage;
    vector<Mat> planos;

    // habilita/desabilita ruido
    bool noise = true;
    // frequencia do ruido
    int freq = 10;
    int freq_max;
    // ganho do ruido
    int gain_int = 0;
    int gain_max = 100;
    float gain = 0;

    // habilita filtro da mediana
    bool median = false;
    // habilita o filtro gaussiano
    bool gaussian = false;
    // habilita o negativo da imagem
    bool negative = false;

    // realiza amostragem da imagem
    bool sample = false;

    // captura background
    bool background = false;

    // subtrai fundo da imagem
    bool subtract = false;

    // valor do ruido
    float mean;

    // guarda tecla capturada
    char key;

    // valores ideais dos tamanhos da imagem
    // para calculo da DFT
    int dft_M, dft_N;

    //  char TrackbarRadiusName[50];
    //  strcpy(TrackbarFrequencyName, "Raio");

    string video_name;

    if (argc != 2) {
        printf("Couldn't load all parameter, use example of use: ./6_tilt config_file\n");
        return 0;
    }

    ConfigLoader param_loader(argv[1]);
    param_loader.checkAndGetString("q7_exercise", video_name);

    VideoCapture cap(video_name);

    // apresenta as opcoes de interacao
    menu();

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    // captura uma imagem para recuperar as
    // informacoes de gravação
    cap >> image;

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    freq_max = dft_M / 2 - 1;
    namedWindow("original", 1);
    createTrackbar("frequencia", "original", &freq, freq_max, on_trackbar_frequency);

    on_trackbar_frequency(freq, 0);

    createTrackbar("amp. ruido", "original", &gain_int, gain_max, on_trackbar_frequency);

    on_trackbar_noise_gain(gain_int, 0);

    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());
    ones = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a função de transferencia (filtro de frequencia) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    // prepara o filtro passa-baixas ideal
    for (int i = 0; i < dft_M; i++) {
        for (int j = 0; j < dft_N; j++) {
            if ((i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2) < RADIUS * RADIUS) {
                tmp.at<float>(i, j) = 1.0;
            }
        }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[] = {tmp, tmp};
    merge(comps, 2, filter);

    for (;;) {
        cap >> image;
        cvtColor(image, imagegray, COLOR_BGR2GRAY);
        if (background == true) {
            imagegray.copyTo(backgroundImage);
            background = false;
        }

        if (subtract) { max(imagegray, backgroundImage, imagegray); }

        if (negative) { bitwise_not(imagegray, imagegray); }
        if (median) {
            medianBlur(imagegray, image, 3);
            image.copyTo(imagegray);
        }
        if (gaussian) {
            GaussianBlur(imagegray, image, Size(3, 3), 0);
            image.copyTo(imagegray);
        }
        imshow("original", imagegray);

        // realiza o padding da imagem
        copyMakeBorder(
            imagegray, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));

        // limpa o array de matrizes que vao compor a
        // imagem complexa
        planos.clear();
        // cria a compoente real
        realInput = Mat_<float>(padded);
        // insere as duas componentes no array de matrizes
        planos.push_back(realInput);
        planos.push_back(zeros);

        // combina o array de matrizes em uma unica
        // componente complexa
        merge(planos, complexImage);

        // calcula o dft
        dft(complexImage, complexImage);
        // realiza a troca de quadrantes
        deslocaDFT(complexImage);

        // exibe o espectro e angulo de fase
        // armazena amostra das imagens
        {
            planos.clear();
            split(complexImage, planos);

            Mat magn, angl, anglInt, magnInt;
            cartToPolar(planos[0], planos[1], magn, angl, false);
            normalize(angl, angl, 0, 255, NORM_MINMAX);
            angl.convertTo(anglInt, CV_8U);
            imshow("Angulo de Fase", anglInt);

            magnitude(planos[0], planos[1], planos[0]);
            magI = planos[0];
            magI += Scalar::all(1);
            log(magI, magI);
            normalize(magI, magI, 0, 255, NORM_MINMAX);
            magI.convertTo(magnInt, CV_8U);
            imshow("Espectro", magnInt);

            if (sample == true) {
                imwrite("dft-imagem.png", padded);
                imwrite("dft-espectro.png", magnInt);
                imwrite("dft-angl.png", anglInt);
                cout << "#### sample ok ###\n";
                menu();
                sample = false;
            }
        }

        // aplica o filtro de frequencia
        mulSpectrums(complexImage, filter, complexImage, 0);

        // limpa o array de planos
        planos.clear();

        // separa as partes real e imaginaria para modifica-las
        split(complexImage, planos);

        // usa o valor medio do espectro para dosar o ruido
        mean = abs(planos[0].at<float>(dft_M / 2, dft_N / 2));

        // insere ruido coerente, se habilitado
        if (noise) {
            gain = 1.0 * gain_int / gain_max;
            // F(u,v) recebe ganho proporcional a F(0,0)
            planos[0].at<float>(dft_M / 2 + freq, dft_N / 2 + freq) += gain * mean;

            planos[1].at<float>(dft_M / 2 + freq, dft_N / 2 + freq) += gain * mean;

            // F*(-u,-v) = F(u,v)
            planos[0].at<float>(dft_M / 2 - freq, dft_N / 2 - freq) =
                planos[0].at<float>(dft_M / 2 + freq, dft_N / 2 + freq);

            planos[1].at<float>(dft_M / 2 - freq, dft_N / 2 - freq) =
                -planos[1].at<float>(dft_M / 2 + freq, dft_N / 2 + freq);
        }

        // recompoe os planos em uma unica matriz complexa
        merge(planos, complexImage);

        // troca novamente os quadrantes
        deslocaDFT(complexImage);

        // calcula a DFT inversa
        idft(complexImage, complexImage);

        // limpa o array de planos
        planos.clear();

        // separa as partes real e imaginaria da
        // imagem filtrada
        split(complexImage, planos);

        // normaliza a parte real para exibicao
        normalize(planos[0], planos[0], 0, 1, NORM_MINMAX);
        imshow("filtrada", planos[0]);

        key = (char)waitKey(10);
        if (key == 27) break; // esc pressed!
        switch (key) {
            case 'e': noise = !noise; break;
            case 'm':
                median = !median;
                menu();
                break;
            case 'g':
                gaussian = !gaussian;
                menu();
                break;
            case 'p': sample = true; break;
            case 'b': background = true; break;
            case 's': subtract = !subtract; break;
            case 'n': negative = !negative; break;
        }
    }
    return 0;
}