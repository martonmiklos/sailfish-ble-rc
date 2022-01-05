#pragma once

#include "shell_rc_car.h"

#include <QTimer>



class BburagoRcCar : public ShellRcCar
{
public:
    explicit BburagoRcCar(const QBluetoothDeviceInfo &address, QObject *parent = nullptr);

    static bool isDevice(const QBluetoothDeviceInfo &info);
    QString name() const override;
    static QString imagePath(const QBluetoothDeviceInfo &info);
    bool connectToDevice() override;

    bool isFeatureSupported(Feature feature) const override;

protected slots:
    void serviceDiscovered(const QBluetoothUuid &uuid) override;
    void send() override;
    void batteryCharacteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value) override;
protected:
    QBluetoothUuid controlServiceUuid() const override;
    QBluetoothUuid controlCharacteristicsUuid() const override;

private:
    QLowEnergyService *m_batteryService = nullptr;
    static QBluetoothUuid CONTROL_SERVICE_UUID;
    static QBluetoothUuid BATTERY_SERVICE_UUID;
    static QBluetoothUuid CONTROL_CHARACTERISTICS_UUID;
    static QBluetoothUuid BATTERY_CHARACTERISTICS_UUID;

    QBluetoothUuid batteryServiceUuid() const;
    QString m_name;

private slots:
    void batteryServiceDetailsDiscovered(QLowEnergyService::ServiceState newState);
};
