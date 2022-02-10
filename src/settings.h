#pragma once

#include <QSettings>
#include <QObject>

class QQmlEngine;
class QJSEngine;

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool autoDiscoverBlDevices READ autoDiscoverBlDevices NOTIFY autoDiscoverBlDevicesChanged)

public:
    Settings(QObject *parent = nullptr);
    static Settings *instance();

    Q_INVOKABLE bool autoConnect(const QString &mac);
    Q_INVOKABLE void setAutoConnect(const QString &mac, bool autoConnect);

    Q_INVOKABLE QString alias(const QString &mac);
    Q_INVOKABLE void setAlias(const QString &mac, const QString &alias);
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);

    Q_INVOKABLE bool autoDiscoverBlDevices() const;
    Q_INVOKABLE void setAutoDiscoverBlDevices(bool on);

private:
    QSettings m_settings;
    bool m_autoDiscoverBlDevices = false;
signals:
    void autoDiscoverBlDevicesChanged();
};

