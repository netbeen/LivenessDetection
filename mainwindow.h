#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QTimer>
#include <QTime>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
    bool isContinueCaptureVideo;
    int frameCount;

private slots:
    void on_startButton_clicked();
    void on_exitButton_clicked();
    void updateFrameRateLabel();
    void ESRtest();

};

#endif // MAINWINDOW_H
