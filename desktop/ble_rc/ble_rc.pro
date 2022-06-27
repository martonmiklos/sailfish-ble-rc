QT       += core gui widgets bluetooth qml

INCLUDEPATH += ../../
INCLUDEPATH += ../../src

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Qt-AES/qaesencryption.cpp \
    ../../qt_goodies/durationformatter.cpp \
    ../../src/abstract_rc_car.cpp \
    ../../src/availabledevicesmodel.cpp \
    ../../src/bburago_rc_car.cpp \
    ../../src/ble_rc_car.cpp \
    ../../src/brandbase_rc_car.cpp \
    ../../src/settings.cpp \
    ../../src/shell_rc_car.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../Qt-AES/qaesencryption.h \
    ../../qt_goodies/durationformatter.h \
    ../../src/abstract_rc_car.h \
    ../../src/availabledevicesmodel.h \
    ../../src/bburago_rc_car.h \
    ../../src/ble_rc_car.h \
    ../../src/brandbase_rc_car.h \
    ../../src/settings.h \
    ../../src/shell_rc_car.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
