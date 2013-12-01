#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "haar2d.h"
#include "intensity.h"

#define DepthStringify(_dep) #_dep

enum {
    SRC_IMAGE_TAB,
    GR_SCL_IMAGE_TAB,
    HAAR_2D_TAB,
    INTENSITY
};

#define ROWS_OFFSET 1
#define NUMBER_ROWS 65
#define NUMBER_COLS 64
#define NUMBER_COEFFS 6

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(SRC_IMAGE_TAB);

    connect(ui->OpenFirstFileBtn,SIGNAL(clicked()),this,SLOT(OpenFirstFile()));
    connect(ui->OpenScndFileBtn,SIGNAL(clicked()),this,SLOT(OpenScndFile()));
    connect(ui->pushButtonGreyScaleImg1,SIGNAL(clicked()),this,SLOT(GrScaleFirstFile()));
    connect(ui->pushButtonGreyScaleImg2,SIGNAL(clicked()),this,SLOT(GrScaleScndFile()));
    connect(ui->pushButtonHaar2D1,SIGNAL(clicked()),this,SLOT(DoHaar2DFirstFile()));
    connect(ui->pushButtonHaar2D2,SIGNAL(clicked()),this,SLOT(DoHaar2DScndFile()));
    connect(ui->pushDeterminnant1,SIGNAL(clicked()),this,SLOT(DoDeterminantsFirstFile()));
    connect(ui->pushDeterminnant2,SIGNAL(clicked()),this,SLOT(DoDeterminantsScndFile()));
    connect(ui->pushButtonRoundFrstImg,SIGNAL(clicked()),this,SLOT(DoRoundFirstImage()));
    connect(ui->pushButtonRoundScndImg,SIGNAL(clicked()),this,SLOT(DoRoundScndImage()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(SelectActiveActions(int)));
    connect(ui->spinBoxIntFirstFile,SIGNAL(valueChanged(int)),this,SLOT(NewIntensityValueFisrtFile(int)));
    connect(ui->spinBoxIntScndFile,SIGNAL(valueChanged(int)),this,SLOT(NewIntensityValueScndFile(int)));
    connect(ui->pushButtonCalcInt1,SIGNAL(clicked()),this,SLOT(CalcIntensityFirstFile(void)));
    connect(ui->pushButtonCalcInt2,SIGNAL(clicked()),this,SLOT(CalcIntensityScndFile(void)));

    ui->tableHaarCoeffGrScl1->setItem(0,0,new QTableWidgetItem("test"));
}

void DepthToString(int DepthVal, QString &str)
{
    switch (DepthVal)
    {
    case CV_8U:
        str = QString(DepthStringify(CV_8U));
        break;
    case CV_8S:
        str = QString(DepthStringify(CV_8S));
        break;
    case CV_16U:
        str = QString(DepthStringify(CV_16U));
        break;
    case CV_16S:
        str = QString(DepthStringify(CV_16S));
        break;
    case CV_32S:
        str = QString(DepthStringify(CV_32S));
        break;
    case CV_32F:
        str = QString(DepthStringify(CV_32F));
        break;
    case CV_64F:
        str = QString(DepthStringify(CV_64F));
        break;
    case CV_USRTYPE1:
        str = QString(DepthStringify(CV_USRTYPE1));
        break;
    default:
        str = QString("none");
        break;
    }
}

void MainWindow::OpenFirstFile(void)
{
    QString depth;
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    if (!fileName.isEmpty())
    {
        first_mat = cv::imread(fileName.toStdString());
        QImage qImage = Mat2QImage(first_mat);
        QPixmap pixmap = QPixmap::fromImage(qImage);
        if (qImage.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        ui->FirstImgWidthValLabel->setText(QString::number(first_mat.cols));
        ui->FirstImgHeightValLabel->setText(QString::number(first_mat.rows));
        ui->FirstImgChValLabel->setText(QString::number(first_mat.channels()));
        DepthToString(first_mat.depth(),depth);
        ui->FirstImgDepthValLabel->setText(depth);
        ui->FirstSrcImageLabel->clear();
        ui->FirstSrcImageLabel->setPixmap(pixmap.scaled(ui->FirstSrcImageLabel->width(),
                                                        ui->FirstSrcImageLabel->height(),
                                                        Qt::KeepAspectRatio));
        ui->FirstGrSclImgWidthValLabel->clear();
        ui->FirstGrSclImgHeightValLabel->clear();
        ui->FirstGrSclImgChValLabel->clear();
        ui->FirstGrSclImgDepthValLabel->clear();
        ui->FirstGrSclImageLabel->clear();
        ui->FirstGrSclImageLabel->setText("Grey Scale 1");
        ui->tableHaarCoeffGrScl1->clearContents();
    }
}

void MainWindow::OpenScndFile(void)
{
    QString depth;
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    if (!fileName.isEmpty())
    {
        scnd_mat = cv::imread(fileName.toStdString());
        QImage qImage = Mat2QImage(scnd_mat);
        QPixmap pixmap = QPixmap::fromImage(qImage);
        if (qImage.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        ui->ScndImgWidthValLabel->setText(QString::number(scnd_mat.cols));
        ui->ScndImgHeightValLabel->setText(QString::number(scnd_mat.rows));
        ui->ScndImgChValLabel->setText(QString::number(scnd_mat.channels()));
        DepthToString(scnd_mat.depth(),depth);
        ui->ScndImgDepthValLabel->setText(depth);
        ui->ScndSrcImageLabel->clear();
        ui->ScndSrcImageLabel->setPixmap(pixmap.scaled(ui->ScndSrcImageLabel->width(),
                                                       ui->ScndSrcImageLabel->height(),
                                                       Qt::KeepAspectRatio));

        ui->ScndGrSclImgWidthValLabel->clear();
        ui->ScndGrSclImgHeightValLabel->clear();
        ui->ScndGrSclImgChValLabel->clear();
        ui->ScndGrSclImgDepthValLabel->clear();
        ui->ScndGrSclImageLabel->clear();
        ui->ScndGrSclImageLabel->setText("Grey Scale 2");
        ui->tableHaarCoeffGrScl2->clearContents();
    }
}

void MainWindow::GrScaleFirstFile()
{
    QString depth;
    if (!first_mat.empty())
    {
        cv::cvtColor( first_mat, first_transform_image, CV_BGR2GRAY );
        QImage qImageGrayScale = Mat2QImage(first_transform_image);
        QPixmap pixmapGrayScale = QPixmap::fromImage(qImageGrayScale);

        ui->FirstGrSclImgWidthValLabel->setText(QString::number(first_transform_image.cols));
        ui->FirstGrSclImgHeightValLabel->setText(QString::number(first_transform_image.rows));
        ui->FirstGrSclImgChValLabel->setText(QString::number(first_transform_image.channels()));
        DepthToString(first_transform_image.depth(),depth);
        ui->FirstGrSclImgDepthValLabel->setText(depth);
        ui->FirstGrSclImageLabel->clear();
        ui->FirstGrSclImageLabel->setPixmap(pixmapGrayScale.scaled(ui->FirstGrSclImageLabel->width(),
                                                                   ui->FirstGrSclImageLabel->height(),
                                                                   Qt::KeepAspectRatio));
        ui->spinBoxIntFirstFile->setValue(getAverageIntensity(first_transform_image));
    }
}

void MainWindow::GrScaleScndFile()
{
    QString depth;
    if (!scnd_mat.empty())
    {
        cv::cvtColor( scnd_mat, scnd_transform_image, CV_BGR2GRAY );
        QImage qImageGrayScale = Mat2QImage(scnd_transform_image);
        QPixmap pixmapGrayScale = QPixmap::fromImage(qImageGrayScale);
        ui->ScndGrSclImgWidthValLabel->setText(QString::number(scnd_transform_image.cols));
        ui->ScndGrSclImgHeightValLabel->setText(QString::number(scnd_transform_image.rows));
        ui->ScndGrSclImgChValLabel->setText(QString::number(scnd_transform_image.channels()));
        DepthToString(scnd_transform_image.depth(),depth);
        ui->ScndGrSclImgDepthValLabel->setText(depth);
        ui->ScndGrSclImageLabel->clear();
        ui->ScndGrSclImageLabel->setPixmap(pixmapGrayScale.scaled(ui->ScndGrSclImageLabel->width(),
                                                                  ui->ScndGrSclImageLabel->height(),
                                                                  Qt::KeepAspectRatio));
        ui->spinBoxIntScndFile->setValue(getAverageIntensity(scnd_transform_image));
    }
}

void MainWindow::NewIntensityValueFisrtFile(int Intensity)
{
    if (!first_transform_image.empty())
    {
        first_transform_image.convertTo(first_transform_image,CV_8U,1,
                                        Intensity-getAverageIntensity(first_transform_image));
        QImage qImageGrayScale = Mat2QImage(first_transform_image);
        QPixmap pixmapGrayScale = QPixmap::fromImage(qImageGrayScale);
        ui->FirstGrSclImageLabel->clear();
        ui->FirstGrSclImageLabel->setPixmap(pixmapGrayScale.scaled(ui->FirstGrSclImageLabel->width(),
                                                                   ui->FirstGrSclImageLabel->height(),
                                                                   Qt::KeepAspectRatio));
        ui->spinBoxIntFirstFile->setValue(getAverageIntensity(first_transform_image));
    }
}

void MainWindow::NewIntensityValueScndFile(int Intensity)
{
    if (!scnd_transform_image.empty())
    {
        scnd_transform_image.convertTo(scnd_transform_image,CV_8U,1,
                                       Intensity-getAverageIntensity(scnd_transform_image));
        QImage qImageGrayScale = Mat2QImage(scnd_transform_image);
        QPixmap pixmapGrayScale = QPixmap::fromImage(qImageGrayScale);
        ui->ScndGrSclImageLabel->clear();
        ui->ScndGrSclImageLabel->setPixmap(pixmapGrayScale.scaled(ui->ScndGrSclImageLabel->width(),
                                                                  ui->ScndGrSclImageLabel->height(),
                                                                  Qt::KeepAspectRatio));
        ui->spinBoxIntScndFile->setValue(getAverageIntensity(scnd_transform_image));
    }
}

void MainWindow::CalcIntensityFirstFile(void)
{
    if (!first_transform_image.empty())
    {
        if (first_transform_image.cols%ui->spinBoxFirstBreakCols->value() != 0)
        {
            QMessageBox msgBox;
            msgBox.setText("Columns should be evenly divisible by break number");
            msgBox.exec();
            return;
        }
        if (first_transform_image.rows%ui->spinBoxFirstBreakRows->value() != 0)
        {
            QMessageBox msgBox;
            msgBox.setText("Rows should be evenly divisible by break number");
            msgBox.exec();
            return;
        }
        ui->tableWidgetFirstFileInt->setRowCount(ui->spinBoxFirstBreakRows->value());
        ui->tableWidgetFirstFileInt->setColumnCount(ui->spinBoxFirstBreakCols->value());
        for (int i = 0; i < ui->spinBoxFirstBreakRows->value(); i++)
        {
            for (int j = 0; j < ui->spinBoxFirstBreakCols->value(); j++)
            {
                cv::Mat disp_frag(first_transform_image,cv::Rect(j*(first_transform_image.cols/4),
                                                                 i*(first_transform_image.rows/4),
                                                                 first_transform_image.cols/4,
                                                                 first_transform_image.rows/4));
                ui->tableWidgetFirstFileInt->setItem(i,j,new QTableWidgetItem(QString::number(getAverageIntensity(disp_frag))));
            }
        }
    }

}

void MainWindow::CalcIntensityScndFile(void)
{
    if (!scnd_transform_image.empty())
    {
        if (scnd_transform_image.cols%ui->spinBoxScndBreakCols->value() != 0)
        {
            QMessageBox msgBox;
            msgBox.setText("Columns should be evenly divisible by break number");
            msgBox.exec();
            return;
        }
        if (scnd_transform_image.rows%ui->spinBoxScndBreakRows->value() != 0)
        {
            QMessageBox msgBox;
            msgBox.setText("Rows should be evenly divisible by break number");
            msgBox.exec();
            return;
        }
        ui->tableWidgetScndFileInt->setRowCount(ui->spinBoxScndBreakRows->value());
        ui->tableWidgetScndFileInt->setColumnCount(ui->spinBoxScndBreakCols->value());
        for (int i = 0; i < ui->spinBoxScndBreakRows->value(); i++)
        {
            for (int j = 0; j < ui->spinBoxScndBreakCols->value(); j++)
            {
                cv::Mat disp_frag(scnd_transform_image,cv::Rect(j*(scnd_transform_image.cols/4),
                                                                i*(scnd_transform_image.rows/4),
                                                                scnd_transform_image.cols/4,
                                                                scnd_transform_image.rows/4));
                ui->tableWidgetScndFileInt->setItem(i,j,new QTableWidgetItem(QString::number(getAverageIntensity(disp_frag))));
            }
        }
    }
}

QLabel* MainWindow::GetActiveLabel(int row, int col)
{
    QLabel* activeLabel;
    switch (row)
    {
    case 0:
        switch (col)
        {
        case 0:
            activeLabel = ui->ImageLabel_1_1;
            break;
        case 1:
            activeLabel = ui->ImageLabel_1_2;
            break;
        case 2:
            activeLabel = ui->ImageLabel_1_3;
            break;
        case 3:
            activeLabel = ui->ImageLabel_1_4;
            break;
        }
        break;
    case 1:
        switch (col)
        {
        case 0:
            activeLabel = ui->ImageLabel_2_1;
            break;
        case 1:
            activeLabel = ui->ImageLabel_2_2;
            break;
        case 2:
            activeLabel = ui->ImageLabel_2_3;
            break;
        case 3:
            activeLabel = ui->ImageLabel_2_4;
            break;
        }
        break;
    case 2:
        switch (col)
        {
        case 0:
            activeLabel = ui->ImageLabel_3_1;
            break;
        case 1:
            activeLabel = ui->ImageLabel_3_2;
            break;
        case 2:
            activeLabel = ui->ImageLabel_3_3;
            break;
        case 3:
            activeLabel = ui->ImageLabel_3_4;
            break;
        }
        break;
    case 3:
        switch (col)
        {
        case 0:
            activeLabel = ui->ImageLabel_4_1;
            break;
        case 1:
            activeLabel = ui->ImageLabel_4_2;
            break;
        case 2:
            activeLabel = ui->ImageLabel_4_3;
            break;
        case 3:
            activeLabel = ui->ImageLabel_4_4;
            break;
        }
        break;
    }
    return activeLabel;
}

void MainWindow::SelectActiveActions(int TabId)
{
    switch(TabId)
    {
        case SRC_IMAGE_TAB:
            ui->OpenFirstFileBtn->setEnabled(true);
            ui->OpenScndFileBtn->setEnabled(true);
            ui->pushButtonGreyScaleImg1->setEnabled(false);
            ui->pushButtonGreyScaleImg2->setEnabled(false);
            ui->pushButtonHaar2D1->setEnabled(false);
            ui->pushButtonHaar2D2->setEnabled(false);
            ui->pushDeterminnant1->setEnabled(false);
            ui->pushDeterminnant2->setEnabled(false);
            ui->spinBoxPrecisionFrstImg->setEnabled(false);
            ui->spinBoxPrecisionScndImg->setEnabled(false);
            ui->pushButtonRoundFrstImg->setEnabled(false);
            ui->pushButtonRoundScndImg->setEnabled(false);
            ui->pushButtonCalcInt1->setEnabled(false);
            ui->pushButtonCalcInt2->setEnabled(false);
            ui->spinBoxFirstBreakCols->setEnabled(false);
            ui->spinBoxFirstBreakRows->setEnabled(false);
            ui->spinBoxScndBreakCols->setEnabled(false);
            ui->spinBoxScndBreakRows->setEnabled(false);
            ui->spinBoxIntFirstFile->setEnabled(false);
            ui->spinBoxIntScndFile->setEnabled(false);
            break;
        case GR_SCL_IMAGE_TAB:
            ui->OpenFirstFileBtn->setEnabled(false);
            ui->OpenScndFileBtn->setEnabled(false);
            ui->pushButtonGreyScaleImg1->setEnabled(true);
            ui->pushButtonGreyScaleImg2->setEnabled(true);
            ui->pushButtonHaar2D1->setEnabled(false);
            ui->pushButtonHaar2D2->setEnabled(false);
            ui->pushDeterminnant1->setEnabled(false);
            ui->pushDeterminnant2->setEnabled(false);
            ui->spinBoxPrecisionFrstImg->setEnabled(false);
            ui->spinBoxPrecisionScndImg->setEnabled(false);
            ui->pushButtonRoundFrstImg->setEnabled(false);
            ui->pushButtonRoundScndImg->setEnabled(false);
            ui->pushButtonCalcInt1->setEnabled(false);
            ui->pushButtonCalcInt2->setEnabled(false);
            ui->spinBoxFirstBreakCols->setEnabled(false);
            ui->spinBoxFirstBreakRows->setEnabled(false);
            ui->spinBoxScndBreakCols->setEnabled(false);
            ui->spinBoxScndBreakRows->setEnabled(false);
            ui->spinBoxIntFirstFile->setEnabled(true);
            ui->spinBoxIntScndFile->setEnabled(true);
            break;
        case HAAR_2D_TAB:
            ui->OpenFirstFileBtn->setEnabled(false);
            ui->OpenScndFileBtn->setEnabled(false);
            ui->pushButtonGreyScaleImg1->setEnabled(false);
            ui->pushButtonGreyScaleImg2->setEnabled(false);
            ui->pushButtonHaar2D1->setEnabled(true);
            ui->pushButtonHaar2D2->setEnabled(true);
            ui->pushDeterminnant1->setEnabled(false);
            ui->pushDeterminnant2->setEnabled(false);
            ui->spinBoxPrecisionFrstImg->setEnabled(false);
            ui->spinBoxPrecisionScndImg->setEnabled(false);
            ui->pushButtonRoundFrstImg->setEnabled(false);
            ui->pushButtonRoundScndImg->setEnabled(false);
            ui->pushButtonCalcInt1->setEnabled(false);
            ui->pushButtonCalcInt2->setEnabled(false);
            ui->spinBoxFirstBreakCols->setEnabled(false);
            ui->spinBoxFirstBreakRows->setEnabled(false);
            ui->spinBoxScndBreakCols->setEnabled(false);
            ui->spinBoxScndBreakRows->setEnabled(false);
            ui->spinBoxIntFirstFile->setEnabled(false);
            ui->spinBoxIntScndFile->setEnabled(false);
            break;
        case INTENSITY:
            ui->OpenFirstFileBtn->setEnabled(false);
            ui->OpenScndFileBtn->setEnabled(false);
            ui->pushButtonGreyScaleImg1->setEnabled(false);
            ui->pushButtonGreyScaleImg2->setEnabled(false);
            ui->pushButtonHaar2D1->setEnabled(false);
            ui->pushButtonHaar2D2->setEnabled(false);
            ui->pushDeterminnant1->setEnabled(false);
            ui->pushDeterminnant2->setEnabled(false);
            ui->spinBoxPrecisionFrstImg->setEnabled(false);
            ui->spinBoxPrecisionScndImg->setEnabled(false);
            ui->pushButtonRoundFrstImg->setEnabled(false);
            ui->pushButtonRoundScndImg->setEnabled(false);
            ui->pushButtonCalcInt1->setEnabled(true);
            ui->pushButtonCalcInt2->setEnabled(true);
            ui->spinBoxFirstBreakCols->setEnabled(true);
            ui->spinBoxFirstBreakRows->setEnabled(true);
            ui->spinBoxScndBreakCols->setEnabled(true);
            ui->spinBoxScndBreakRows->setEnabled(true);
            ui->spinBoxIntFirstFile->setEnabled(false);
            ui->spinBoxIntScndFile->setEnabled(false);

            if (!first_transform_image.empty())
            {
                QLabel* activeLabel;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        cv::Mat disp_frag(first_transform_image,cv::Rect(j*(first_transform_image.cols/4),
                                                                         i*(first_transform_image.rows/4),
                                                                         first_transform_image.cols/4,
                                                                         first_transform_image.rows/4));
                        QImage qImageGrayScale = Mat2QImage(disp_frag);
                        QPixmap pixmapGrayScale = QPixmap::fromImage(qImageGrayScale);
                        activeLabel = GetActiveLabel(i,j);
                        activeLabel->clear();
                        activeLabel->setPixmap(pixmapGrayScale.scaled(activeLabel->width(),
                                                                      activeLabel->height(),
                                                                      Qt::KeepAspectRatio));
                    }
                }

            }
            break;
        default:
            ui->OpenFirstFileBtn->setEnabled(false);
            ui->OpenScndFileBtn->setEnabled(false);
            ui->pushButtonGreyScaleImg1->setEnabled(false);
            ui->pushButtonGreyScaleImg2->setEnabled(false);
            ui->pushButtonHaar2D1->setEnabled(false);
            ui->pushButtonHaar2D2->setEnabled(false);
            ui->pushDeterminnant1->setEnabled(false);
            ui->pushDeterminnant2->setEnabled(false);
            ui->pushButtonCalcInt1->setEnabled(false);
            ui->pushButtonCalcInt2->setEnabled(false);
            ui->spinBoxFirstBreakCols->setEnabled(false);
            ui->spinBoxFirstBreakRows->setEnabled(false);
            ui->spinBoxScndBreakCols->setEnabled(false);
            ui->spinBoxScndBreakRows->setEnabled(false);
            ui->spinBoxIntFirstFile->setEnabled(false);
            ui->spinBoxIntScndFile->setEnabled(false);
            break;
    }
}

void MainWindow::SaveTransformToFile(void)
{
/*
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QDir::currentPath(),
                                                    tr("Images (*.jpg)"));
    if (!fileName.isEmpty())
    {
        cv::imwrite(fileName.toStdString(),transform_image);
    }
*/
}

void MainWindow::DoHaar2DFirstFile(void)
{
    if (!first_transform_image.empty())
    {
        first_transform_image.convertTo(first_transform_image,CV_64FC1);
        haar2d(first_transform_image,first_transform_image.rows, first_transform_image.cols);
        //first_transform_image.convertTo(first_transform_image,CV_8UC1);

        for (int i = ROWS_OFFSET; i < NUMBER_ROWS; i++)
        {
            for (int j = 0; j < NUMBER_COLS; j++)
            {
                ui->tableHaarCoeffGrScl1->setItem(i,j,new QTableWidgetItem(QString::number(first_transform_image.at<double>(i-1,j))));
            }
        }
        ui->pushDeterminnant1->setEnabled(true);
        ui->spinBoxPrecisionFrstImg->setEnabled(true);
        ui->pushButtonRoundFrstImg->setEnabled(true);
    }
}

void MainWindow::DoDeterminantsFirstFile(void)
{
    for (int coeff = 0; coeff < NUMBER_COEFFS; coeff++)
    {
        cv::Mat disp_frag(first_transform_image,cv::Rect(0,0,2<<coeff,2<<coeff));
        ui->tableHaarCoeffGrScl1->setItem(0,coeff,new QTableWidgetItem(QString::number(determinant(disp_frag))));
    }
}

void MainWindow::DoHaar2DScndFile(void)
{
    if (!scnd_transform_image.empty())
    {
        scnd_transform_image.convertTo(scnd_transform_image,CV_64FC1);
        haar2d(scnd_transform_image,scnd_transform_image.rows, scnd_transform_image.cols);
        //scnd_transform_image.convertTo(scnd_transform_image,CV_8UC1);

        for (int i = ROWS_OFFSET; i < NUMBER_ROWS; i++)
        {
            for (int j = 0; j < NUMBER_COLS; j++)
            {
                ui->tableHaarCoeffGrScl2->setItem(i,j,new QTableWidgetItem(QString::number(scnd_transform_image.at<double>(i-1,j))));
            }
        }
        ui->pushDeterminnant2->setEnabled(true);
        ui->spinBoxPrecisionScndImg->setEnabled(true);
        ui->pushButtonRoundScndImg->setEnabled(true);
    }

}

void MainWindow::DoDeterminantsScndFile(void)
{
    for (int coeff = 0; coeff < NUMBER_COEFFS; coeff++)
    {
        cv::Mat disp_frag(scnd_transform_image,cv::Rect(0,0,2<<coeff,2<<coeff));
        ui->tableHaarCoeffGrScl2->setItem(0,coeff,new QTableWidgetItem(QString::number(determinant(disp_frag))));
    }
}

void MainWindow::DoRoundFirstImage(void)
{
    int round_val = ui->spinBoxPrecisionFrstImg->value();
    for (int i = ROWS_OFFSET; i < NUMBER_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_COLS; j++)
        {
            if (ui->tableHaarCoeffGrScl1->item(i,j))
            {
                QString old_val_str = ui->tableHaarCoeffGrScl1->item(i,j)->text();
                double old_val = old_val_str.toDouble();
                ui->tableHaarCoeffGrScl1->setItem(i,j,new QTableWidgetItem(QString::number(round_to_precision(old_val,round_val))));

            }
        }
    }
}

void MainWindow::DoRoundScndImage(void)
{
    int round_val = ui->spinBoxPrecisionScndImg->value();
    for (int i = ROWS_OFFSET; i < NUMBER_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_COLS; j++)
        {
            if (ui->tableHaarCoeffGrScl2->item(i,j))
            {
                QString old_val_str = ui->tableHaarCoeffGrScl2->item(i,j)->text();
                double old_val = old_val_str.toDouble();
                ui->tableHaarCoeffGrScl2->setItem(i,j,new QTableWidgetItem(QString::number(round_to_precision(old_val,round_val))));
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
