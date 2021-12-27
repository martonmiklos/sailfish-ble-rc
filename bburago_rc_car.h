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

protected slots:
    void send() override;
    void characteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value) override;
protected:
    QBluetoothUuid controlServiceUuid() const override;
    QBluetoothUuid controlCharacteristicsUuid() const override;

private:
    static QBluetoothUuid CONTROL_SERVICE_UUID;
    static QBluetoothUuid CONTROL_CHARACTERISTICS_UUID;
    static QBluetoothUuid BATTERY_CHARACTERISTICS_UUID;
};
