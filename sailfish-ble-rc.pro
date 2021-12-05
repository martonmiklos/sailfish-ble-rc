# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = sailfish-ble-rc

CONFIG += sailfishapp

QT += bluetooth

SOURCES += src/sailfish-ble-rc.cpp \
    Qt-AES/qaesencryption.cpp \
    abstract_rc_car.cpp \
    availabledevicesmodel.cpp \
    ble_devicescanner.cpp \
    ble_rc_car.cpp \
    shell_rc_car.cpp

DISTFILES += qml/sailfish-ble-rc.qml \
    qml/QMLVirtualJoystick/VirtualJoystick.qml \
    qml/QMLVirtualJoystick/background.png \
    qml/QMLVirtualJoystick/finger.png \
    qml/cover/CoverPage.qml \
    qml/pages/ConnectingPage.qml \
    qml/pages/DrivePage.qml \
    qml/pages/Settings.qml \
    qml/pages/SearchPage.qml \
    rpm/sailfish-ble-rc.changes.in \
    rpm/sailfish-ble-rc.changes.run.in \
    rpm/sailfish-ble-rc.spec \
    rpm/sailfish-ble-rc.yaml \
    translations/*.ts \
    sailfish-ble-rc.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

CONFIG += qml_debug

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
#TRANSLATIONS += translations/sailfish-ble-rc-de.ts

HEADERS += \
    Qt-AES/qaesencryption.h \
    abstract_rc_car.h \
    availabledevicesmodel.h \
    ble_devicescanner.h \
    ble_rc_car.h \
    shell_rc_car.h

RESOURCES += \
    resources.qrc

