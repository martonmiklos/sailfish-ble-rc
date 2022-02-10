#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_autoDiscoverBlDevices = m_settings.value("BlAutoDiscover").toBool();
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
