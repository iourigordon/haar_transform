#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T23:56:08
#
#-------------------------------------------------

QT       += core gui

TARGET = qt_opencv
TEMPLATE = app

LIBS += -lopencv_core -lopencv_ml -lopencv_imgproc -lopencv_video \
        -lopencv_objdetect -lopencv_gpu -lopencv_highgui -lopencv_calib3d \
        -lopencv_flann -lopencv_features2d -lopencv_legacy -lopencv_contrib

SOURCES += main.cpp\
        mainwindow.cpp \
    mat2qimage.cpp \
    haar2d.cpp \
    intensity.cpp

HEADERS  += mainwindow.h \
    mat2qimage.h \
    haar2d.h \
    intensity.h

FORMS    += mainwindow.ui
