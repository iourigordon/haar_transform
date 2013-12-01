#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "mat2qimage.h"

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
    QLabel* GetActiveLabel(int row, int col);

public slots:
    void OpenFirstFile(void);
    void OpenScndFile(void);
    void GrScaleFirstFile(void);
    void GrScaleScndFile(void);
    void SaveTransformToFile(void);
    void SelectActiveActions(int TabId);
    void DoHaar2DFirstFile(void);
    void DoHaar2DScndFile(void);
    void DoDeterminantsFirstFile(void);
    void DoDeterminantsScndFile(void);
    void DoRoundFirstImage(void);
    void DoRoundScndImage(void);
    void NewIntensityValueFisrtFile(int Intensity);
    void NewIntensityValueScndFile(int Intensity);
    void CalcIntensityFirstFile(void);
    void CalcIntensityScndFile(void);
    
private:
    Ui::MainWindow *ui;
    cv::Mat first_mat;
    cv::Mat scnd_mat;
    cv::Mat first_transform_image;
    cv::Mat scnd_transform_image;
};

#endif // MAINWINDOW_H
