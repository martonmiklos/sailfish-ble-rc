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

equals(MY_TARGET, "Sailfish") {
    DEFINES += Q_OS_SAILFISH_OS
}

CONFIG += sailfishapp

QT += bluetooth

isEmpty(VERSION) {
    VERSION = 1.0.0
    message("VERSION is unset, assuming $$VERSION")
}

DEFINES += APP_VERSION=\\\"$$VERSION\\\" \

SOURCES += \
    Qt-AES/qaesencryption.cpp \
    src/abstract_rc_car.cpp \
    src/availabledevicesmodel.cpp \
    src/bburago_rc_car.cpp \
    src/ble_rc_car.cpp \
    src/brandbase_rc_car.cpp \
    src/main.cpp \
    src/settings.cpp \
    src/shell_rc_car.cpp

DISTFILES += qml/sailfish-ble-rc.qml \
    qml/QMLVirtualJoystick/VirtualJoystick.qml \
    qml/QMLVirtualJoystick/background.png \
    qml/QMLVirtualJoystick/finger.png \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/ConnectingPage.qml \
    qml/pages/DrivePage.qml \
    qml/pages/GadgetEditPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/SettingsPage.qml \
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
TRANSLATIONS += translations/sailfish-ble-rc-hu.ts

HEADERS += \
    Qt-AES/qaesencryption.h \
    src/abstract_rc_car.h \
    src/availabledevicesmodel.h \
    src/bburago_rc_car.h \
    src/ble_rc_car.h \
    src/brandbase_rc_car.h \
    src/settings.h \
    src/shell_rc_car.h

RESOURCES += \
    resources.qrc

