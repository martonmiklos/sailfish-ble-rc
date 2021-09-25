#pragma once

#include "abstract_rc_car.h"

#include <QAbstractListModel>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QQmlEngine>

class AvailableDevicesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool scanInProgress READ scanInProgress NOTIFY scanStateChanged)
    Q_PROPERTY(QString statusString READ statusString NOTIFY statusStringChanged)
public:
    enum Role {
        Name = Qt::UserRole + 1,
        TypeName,
        ImagePath,
        Index
    };

    enum DeviceType {
        Shell,
    };

    class DetectedDevice {
    public:
        DetectedDevice() = default;
        QString imagePath, typeName;
        QBluetoothDeviceInfo info;
        DeviceType type;
        QString name() const
        {
            return info.name();
        }
    };

    explicit AvailableDevicesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void detectDevices();

    bool scanInProgress() const;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void useDevice(int deviceIndex);

    Q_INVOKABLE AbstractRC_Car *currentDevice() const;

    Q_INVOKABLE QString statusString() const;
    void setStatusString(const QString & statusString);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void deviceDiscovered(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);

Q_SIGNALS:
    void scanStateChanged();
    void statusStringChanged();

private:
    void setScanInProgress(bool scanInProgress);

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    QList<DetectedDevice> m_devices;
    bool m_scanInProgress = false;
    AbstractRC_Car *m_currentDevice = nullptr;
    QString m_statusString;
};

