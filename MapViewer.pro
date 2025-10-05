QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/coordinateconverter.cpp \
    src/main.cpp \
    src/mapwidget.cpp \
    src/mapwindow.cpp \
    src/osmloader.cpp

HEADERS += \
    src/coordinateconverter.h \
    src/mapwidget.h \
    src/mapwindow.h \
    src/osmloader.h

FORMS += \
    src/mapwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
