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
        Alias,
        ImagePath,
        Index,
        AutoConnect,
        MacAddress,
        Rssi
    };

    enum DeviceType {
        Brandbase,
        Bburago
    };

    class DetectedDevice {
    public:
        DetectedDevice(const QBluetoothDeviceInfo &btInfo, DeviceType dtype);
        QString imagePath, typeName;
        DeviceType type;
        QString name() const;
        QString alias() const;
        void setAlias(const QString &newAlias);
        const QBluetoothDeviceInfo &btInfo() const;
        bool autoConnect() const;
        void setAutoConnect(bool newAutoConnect);

    private:
        QBluetoothDeviceInfo m_btInfo;
        QString m_alias;
        bool m_autoConnect = false;
    };

    explicit AvailableDevicesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void discoverDevices();

    bool scanInProgress() const;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void connectToDevice(const QString &mac);
    Q_INVOKABLE void connectToDeviceAt(int row);

    Q_INVOKABLE AbstractRcCar *currentDevice() const;

    Q_INVOKABLE QString statusString() const;
    void setStatusString(const QString & statusString);

    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);

    Q_INVOKABLE void disconnectFromCurrentDevice();
    Q_INVOKABLE void setAutoConnect(int row, bool autoConnect);

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void deviceDiscovered(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanCancelled();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void currentDeviceConnectionStateChangedSlot(AbstractRcCar::ConnectionState oldState, AbstractRcCar::ConnectionState newState);
    void currentDeviceConnectionStateStringChangedSlot();

Q_SIGNALS:
    void scanStateChanged();
    void statusStringChanged();
    void currentDeviceConnectionStateChanged(int oldState, int newState);
    void currentDeviceConnectionStateStringChanged(const QString &stateString);

private:
    void setScanInProgress(bool scanInProgress);

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    QList<DetectedDevice> m_devices;
    bool m_scanInProgress = false;
    AbstractRcCar *m_currentDevice = nullptr;
    QString m_statusString;
    QString m_deviceToBeOpenedOnDiscoverCancelled;
    void cancelDiscovery();
};

