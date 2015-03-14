#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ESR.hpp"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frameCount(0)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateFrameRateLabel()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    ESRtest();
}

void MainWindow::on_exitButton_clicked()
{
    isContinueCaptureVideo = false;
    if (cap.isOpened()) {
        cap.release();
    }
    exit(0);
}

void MainWindow::updateFrameRateLabel(){
    ui->FrameRateLabel->setText(QString::number(frameCount, 10));
    frameCount = 0;
}

void MainWindow::ESRtest(){
    if (cap.open(0)) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    }else{
        exit(1);
    }
    CascadeClassifier cascadeFrontalface;
    cascadeFrontalface.load("/home/netbeen/workspace/20141015-ESR-HelenDatabase/data/haarcascade_frontalface_alt2.xml");
    ShapeRegressor regressor;
    regressor.load("/home/netbeen/workspace/20141015-ESR-HelenDatabase/data/model-Helen-HAAR-alt2-10-125.txt");

    const int initial_number = 20;
    const int landmarkNum = 194;
    Mat rawImg,grayImg;
    QImage displayImage;
    BoundingBox boundingBox;
    isContinueCaptureVideo = true;
    while (isContinueCaptureVideo) {
        cap >> rawImg;
        cvtColor(rawImg, grayImg, COLOR_RGB2GRAY);
        if (detectFace(grayImg, cascadeFrontalface,5, boundingBox)) {
            Mat_<double> current_shape = regressor.predict(grayImg, boundingBox, initial_number);   // Predict the shape.
            for (int i = 0; i < landmarkNum; i++) {
                circle(rawImg, Point2d(current_shape(i, 0), current_shape(i, 1)), 3, Scalar(0, 255, 0), -1, 8, 0);
            }
            rectangle(rawImg, boundingBox.returnRect(), Scalar(0, 255, 255), 3, 8, 0);
        }
        cvtColor( rawImg, rawImg, COLOR_BGR2RGB );
        displayImage = QImage( static_cast<const unsigned char*>(rawImg.data), rawImg.cols, rawImg.rows, QImage::Format_RGB888 );
        ui->label->setPixmap( QPixmap::fromImage(displayImage) );
        frameCount++;
        if (waitKey(30) >= 0);          // I add this line because the VideoCapture seems not work with out it.
    }
}
