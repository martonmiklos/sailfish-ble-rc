#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "availabledevicesmodel.h"
#include "settings.h"

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#elif defined(Q_OS_SAILFISH_OS)
#include <sailfishapp.h>
#endif

#include <QLoggingCategory>


int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationDomain("hu");
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("SailRC");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
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

    qmlRegisterSingletonType<AvailableDevicesModel>(uri, 1, 0, "AvailableDevicesModel",  AvailableDevicesModel::qmlInstance);
    qmlRegisterSingletonType<Settings>(uri, 1, 0, "Settings",  Settings::qmlInstance);
    qmlRegisterUncreatableType<AbstractRcCar>(uri, 1, 0, "AbstractRcCar","AbstractRcCar is an abstract type");
#if defined(Q_OS_ANDROID)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
#elif defined(Q_OS_SAILFISH_OS)
    return SailfishApp::main(argc, argv);
#endif
}
