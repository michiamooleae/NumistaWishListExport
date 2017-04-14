TARGET = NumistaWishListExport
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QMAKE_MAC_SDK = macosx10.12

SOURCES += main.cpp \
    httpretriever.cpp

macx: LIBS += -L$$PWD/../../../../usr/local/Cellar/boost/1.59.0/lib/ -lboost_system
macx: LIBS += -L$$PWD/../../../../usr/local/Cellar/opencv3/3.1.0/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
macx: LIBS += -L$$PWD/../../../../usr/local/Cellar/curl/7.46.0/lib/ -lcurl

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/boost/1.59.0/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/boost/1.59.0/include

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/opencv3/3.1.0/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/opencv3/3.1.0/include

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/curl/7.46.0/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/curl/7.46.0/include

DISTFILES += \
    .clang-format

HEADERS += \
    httpretriever.h
