#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include "Recorde.h"
#include "Menu.h"
#include <ctime>
#include <cstdlib> 


using namespace std;
using namespace cv;

// Configurações
string cascadeName = "haarcascade_frontalface_default.xml";
string wName = "Flappy Face";
int vidas = 1;
int score = 0;
time_t startTime;
float velocidade = 10.0;


// Cano
struct Pipe {
    int x;
    int gapY;
    int width;
    int gapHeight;
    bool contado = false;
};

// Lista de canos
vector<Pipe> pipes;

// Função para desenhar uma imagem com transparência
void drawImage(Mat &frame, Mat &img, int xPos, int yPos) {
    if (yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols || xPos < 0 || yPos < 0)
        return;

    Mat mask;
    vector<Mat> layers;
    split(img, layers);
    if (layers.size() == 4) { // PNG com alpha
        Mat rgb[3] = { layers[0], layers[1], layers[2] };
        mask = layers[3];
        Mat bgr;
        merge(rgb, 3, bgr);
        bgr.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}

// Inicializa um cano
Pipe createPipe(int screenWidth, int screenHeight) {
    Pipe p;
    p.x = screenWidth;
    p.width = 80;
    p.gapHeight = 150;
    p.gapY = rand() % (screenHeight - 200) + 100;
    return p;
}

int main() {
    Menu menu;
    int opcao;

    while (true) {
        opcao = menu.exibir();

        if (opcao == 1) {
            
            break; // sai do menu e começa o jogo normal
        }
        else if (opcao == 2) {
            menu.mostrarRecorde();
        }
        else if (opcao == 3) {
            menu.mostrarCreditos();
        }
    }
    Recorde rec;
    vector<Pipe> canos;
    srand(time(0));

    VideoCapture capture;
    if (!capture.open(0)) {
        cout << "ERRO: Não consegui abrir a câmera." << endl;
        return 1;
    }

    CascadeClassifier cascade;
    if (!cascade.load(cascadeName)) {
        cout << "ERRO: Não consegui carregar o classificador." << endl;
        return -1;
    }

    // Carregar sprite da laranja
    Mat orange = imread("bird.png", IMREAD_UNCHANGED);
    if (orange.empty()) {
        cout << "ERRO: Não consegui carregar bird.png" << endl;
        return -1;
    }
    // Redimensiona para um tamanho fixo
    resize(orange, orange, Size(40, 40));

    namedWindow(wName, WINDOW_NORMAL);
    setWindowProperty(wName, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

    Mat frame, smallFrame, gray;
    double scale = 1;
    char key = 0;

    // Primeiros canos
    capture >> frame;
    int screenWidth = frame.cols / scale;
    int screenHeight = frame.rows / scale;
    pipes.push_back(createPipe(screenWidth, screenHeight));

    cout << "COMECA O JOGO, DESVIE DOS OBSTACULOS, É FLAPPY FACE!" << endl;

    while (vidas > 0) {
        capture >> frame;
        if (frame.empty()) break;

        resize(frame, smallFrame, Size(screenWidth, screenHeight));
        flip(smallFrame, smallFrame, 1);
        cvtColor(smallFrame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        vector<Rect> faces;
        cascade.detectMultiScale(gray, faces, 1.3, 2, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));

        Point birdPos(screenWidth / 2, screenHeight / 2); // fallback
        if (!faces.empty()) {
            Rect f = faces[0];
            birdPos.x = f.x + f.width / 2;
            birdPos.y = f.y + f.height / 2;
        }

        // Atualiza canos
        for (auto &p : pipes) {
            p.x -= velocidade; // velocidade dos canos
        }
        if (pipes.back().x < screenWidth - 200) {
            pipes.push_back(createPipe(screenWidth, screenHeight));
        }
        if (pipes.front().x + pipes.front().width < 0) {
            pipes.erase(pipes.begin());
            
        }

        // Desenha canos
        for (auto &p : pipes) {
            rectangle(smallFrame, Rect(p.x, 0, p.width, p.gapY - p.gapHeight / 2), Scalar(0, 255, 0), FILLED);
            rectangle(smallFrame, Rect(p.x, p.gapY + p.gapHeight / 2, p.width, screenHeight), Scalar(0, 255, 0), FILLED);

            // Colisão
            if (birdPos.x > p.x && birdPos.x < p.x + p.width) {
                if (birdPos.y < p.gapY - p.gapHeight / 2 || birdPos.y > p.gapY + p.gapHeight / 2) {
                    vidas = 0;
                    system("aplay sons/fail.wav &");
                }
            }
        }
        // Score
    for (auto &p : pipes) {
    if (!p.contado && birdPos.x > p.x + p.width) {
        score++;
        p.contado = true;

        
        system("aplay sons/ping.wav &");

        //  Aumenta velocidade a cada 3 pontos
        if (score % 3 == 0) {
            velocidade += 0.8;
        }
    }
}


        imshow(wName, frame);


        // Desenha o pássaro como a laranja
        drawImage(smallFrame, orange, birdPos.x - orange.cols / 2, birdPos.y - orange.rows / 2);

        // HUD
        putText(smallFrame, "Pontos: " + to_string(score), Point(20, 50), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 2);

        imshow(wName, smallFrame);

        key = (char)waitKey(20);
        if (key == 27 || key == 'q') break;
    }
    

    // Tela final
    Mat fim(screenHeight, screenWidth, CV_8UC3, Scalar(0, 0, 0));
    putText(fim, "Perdeste Friend", Point(100, screenHeight / 2), FONT_HERSHEY_DUPLEX, 2, Scalar(0, 0, 255), 3);
    putText(fim, "Pontos: " + to_string(score), Point(100, screenHeight / 2 + 60), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 2);
    putText(fim, "Pressione alguma tecla para continuar", Point(100, screenHeight / 2 + 120), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(200, 200, 200), 2);
    imshow(wName, fim);
    waitKey(5000);
    
    rec.mostrar();             
    
    destroyAllWindows();  
    waitKey(1);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    
    rec.verificarNovoRecorde(score);
    return 0;
}
