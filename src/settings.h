#pragma once

#include <QSettings>

class Settings
{
public:
    Settings() = default;
    static Settings *instance();

    bool autoConnect(const QString &mac);
    void setAutoConnect(const QString &mac, bool autoConnect);

    QString alias(const QString &mac);
    void setAlias(const QString &mac, const QString &alias);

private:
    QSettings m_settings;
};

