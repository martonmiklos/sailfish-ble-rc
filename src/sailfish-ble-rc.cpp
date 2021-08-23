#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "availabledevicesmodel.h"

#include <sailfishapp.h>

int main(int argc, char *argv[])
{
    auto uri = "hu.mm.sailfish_ble_rc";
    // SailfishApp::main() will display "qml/sailfish-ble-rc.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    qmlRegisterSingletonType<AvailableDevicesModel>    (uri, 1, 0, "AvailableDevicesModel",  AvailableDevicesModel::qmlInstance);
    qmlRegisterUncreatableType<AbstractRC_Car>(uri, 1, 0, "AbstractRC_Car","AbstractRC_Car is an abstract type");
    return SailfishApp::main(argc, argv);
}
