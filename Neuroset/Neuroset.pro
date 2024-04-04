QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    battery.cpp \
    batterythread.cpp \
    controller.cpp \
    defs.cpp \
    electrode.cpp \
    filemanager.cpp \
    frequencydata.cpp \
    main.cpp \
    mainwindow.cpp \
    pc.cpp \
    qcustomplot.cpp \
    sessionlog.cpp \
    source.cpp

HEADERS += \
    battery.h \
    batterythread.h \
    controller.h \
    defs.h \
    electrode.h \
    filemanager.h \
    frequencydata.h \
    mainwindow.h \
    pc.h \
    qcustomplot.h \
    sessionlog.h \
    source.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
