#pragma once

#include <QSettings>
#include <QObject>

class QQmlEngine;
class QJSEngine;

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool autoDiscoverBlDevices READ autoDiscoverBlDevices NOTIFY autoDiscoverBlDevicesChanged)
    Q_PROPERTY(bool leftHandedOperation READ leftHandedOperation NOTIFY leftHandedOperationChanged)

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

    Q_INVOKABLE bool leftHandedOperation() const;
    Q_INVOKABLE void setLeftHandedOperation(bool newLeftHandedOperation);

    Q_INVOKABLE quint32 runTime(const QString &mac);
    Q_INVOKABLE QString runTimeString(const QString &mac);
    Q_INVOKABLE void addRunTime(const QString &mac, quint32 runtimeInSec);

    Q_INVOKABLE quint32 connectTime(const QString &mac);
    Q_INVOKABLE QString connectTimeString(const QString &mac);
    Q_INVOKABLE void addConnectTime(const QString &mac, quint32 connecttimeInSec);
private:
    QSettings m_settings;
    bool m_autoDiscoverBlDevices = false;
    bool m_leftHandedOperation = false;

    int getTime(const QString &mac, const QString & key);
    void addTime(const QString &mac, const QString & key, quint32 time);

signals:
    void autoDiscoverBlDevicesChanged();
    void leftHandedOperationChanged();
};

