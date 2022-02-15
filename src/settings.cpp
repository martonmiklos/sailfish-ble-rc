#include "settings.h"

#include "qt_goodies/durationformatter.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_autoDiscoverBlDevices = m_settings.value("BlAutoDiscover", false).toBool();
    m_leftHandedOperation = m_settings.value("leftHandedOperation", false).toBool();
}

Settings *Settings::instance()
{
    static Settings *instance = nullptr;
    if (!instance)
        instance = new Settings();
    return instance;
}

bool Settings::autoConnect(const QString &mac)
{
    bool ret = false;
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    ret = m_settings.value("autoConnect").toBool();
    m_settings.endGroup();
    m_settings.endGroup();
    return ret;
}

void Settings::setAutoConnect(const QString &mac, bool autoConnect)
{
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    m_settings.setValue("autoConnect", autoConnect);
    m_settings.endGroup();
    m_settings.endGroup();
}

QString Settings::alias(const QString &mac)
{
    QString ret;
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    ret = m_settings.value("alias").toString();
    m_settings.endGroup();
    m_settings.endGroup();
    return ret;
}

void Settings::setAlias(const QString &mac, const QString &alias)
{
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    m_settings.setValue("alias", alias);
    m_settings.endGroup();
    m_settings.endGroup();
}

QObject *Settings::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return Settings::instance();
}

bool Settings::autoDiscoverBlDevices() const
{
    return m_autoDiscoverBlDevices;
}

void Settings::setAutoDiscoverBlDevices(bool on)
{
    if (on != m_autoDiscoverBlDevices) {
        m_settings.setValue("BlAutoDiscover", on);
        m_autoDiscoverBlDevices = on;
        emit autoDiscoverBlDevicesChanged();
    }
}

bool Settings::leftHandedOperation() const
{
    return m_leftHandedOperation;
}

void Settings::setLeftHandedOperation(bool newLeftHandedOperation)
{
    if (m_leftHandedOperation == newLeftHandedOperation)
        return;
    m_leftHandedOperation = newLeftHandedOperation;
    m_settings.setValue("leftHandedOperation", newLeftHandedOperation);
    emit leftHandedOperationChanged();
}

quint32 Settings::runTime(const QString &mac)
{
    return getTime(mac, QStringLiteral("runTime"));
}

QString Settings::runTimeString(const QString &mac)
{
    return DurationFormatter::formatDuration(runTime(mac), DurationFormatter::Sec, DurationFormatter::Sec);
}

void Settings::addRunTime(const QString &mac, quint32 runtimeInSec)
{
    addTime(mac, QStringLiteral("runTime"), runtimeInSec);
}

quint32 Settings::connectTime(const QString &mac)
{
    return getTime(mac, QStringLiteral("connectTime"));
}

QString Settings::connectTimeString(const QString &mac)
{
    return DurationFormatter::formatDuration(connectTime(mac), DurationFormatter::Sec, DurationFormatter::Sec);
}

void Settings::addConnectTime(const QString &mac, quint32 connectTimeInSec)
{
    addTime(mac, QStringLiteral("connectTime"), connectTimeInSec);
}

int Settings::getTime(const QString &mac, const QString &key)
{
    quint32 ret = 0;
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    ret = m_settings.value(key).toUInt();
    m_settings.endGroup();
    m_settings.endGroup();
    return ret;
}

void Settings::addTime(const QString &mac, const QString &key, quint32 time)
{
    m_settings.beginGroup("cars");
    m_settings.beginGroup(mac);
    m_settings.setValue(key, m_settings.value(key).toUInt() + time);
    m_settings.endGroup();
    m_settings.endGroup();
}
