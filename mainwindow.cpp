#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    if (cap.open(0)) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    }else{
        exit(1);
    }
    Mat rawImg;
    QImage displayImage;
    while (true) {
        cap >> rawImg;
        cvtColor( rawImg, rawImg, CV_BGR2RGB );
        displayImage = QImage( static_cast<const unsigned char*>(rawImg.data), rawImg.cols, rawImg.rows, QImage::Format_RGB888 );
        ui->label->setPixmap( QPixmap::fromImage(displayImage) );
        if (waitKey(30) >= 0);          // I add this line because the VideoCapture seems not work with out it.
    }
}

void MainWindow::on_exitButton_clicked()
{
    if (cap.isOpened()) {
        cap.release();
    }
    ui->startButton->setEnabled(true);
}
