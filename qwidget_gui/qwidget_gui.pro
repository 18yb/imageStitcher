#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T09:38:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qwidget_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    laplacian.cpp \
    laplaciangaussian.cpp \
    roberts.cpp \
    sobel.cpp \
    stitcher.cpp \
    hog.cpp

HEADERS  += mainwindow.h \
    laplacian.h \
    laplaciangaussian.h \
    roberts.h \
    sobel.h \
    stitcher.h \
    hog.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_flann
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_xfeatures2d
