QT       += core gui

INCLUDEPATH += ../../
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets bluetooth qml

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Qt-AES/qaesencryption.cpp \
    ../../abstract_rc_car.cpp \
    ../../availabledevicesmodel.cpp \
    ../../ble_devicescanner.cpp \
    ../../ble_rc_car.cpp \
    ../../shell_rc_car.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../Qt-AES/qaesencryption.h \
    ../../abstract_rc_car.h \
    ../../availabledevicesmodel.h \
    ../../ble_devicescanner.h \
    ../../ble_rc_car.h \
    ../../shell_rc_car.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
