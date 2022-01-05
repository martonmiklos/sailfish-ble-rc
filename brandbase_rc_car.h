#pragma once

#include "shell_rc_car.h"

#include <QTimer>
#include <QLowEnergyController>

class QAESEncryption;

class BrandbaseRcCar : public ShellRcCar
{
    Q_OBJECT
public:
    BrandbaseRcCar(const QBluetoothDeviceInfo &address, QObject *parent = nullptr);

    static bool isDevice(const QBluetoothDeviceInfo &info);
    static QString imagePath();
    QString name() const override;

protected slots:
    void send() override;
    void controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState) override;
    void batteryCharacteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value) override;

private:
    QAESEncryption *m_aesDecryptor;

    static QBluetoothUuid CONTROL_SERVICE_UUID;
    static QBluetoothUuid CONTROL_CHARACTERISTICS_UUID;
    static QBluetoothUuid BATTERY_CHARACTERISTICS_UUID;

    static QByteArray AES_KEY;

    static QByteArray IDLE_COMMAND;
    static QByteArray FORWARD_COMMAND;
    static QByteArray FORWARD_TURBO_COMMAND;
    static QByteArray BACKWARD_COMMAND;
    static QByteArray BACKWARD_TURBO_COMMAND;
    static QByteArray FORWARD_LEFT_COMMAND;
    static QByteArray LEFT_COMMAND;
    static QByteArray FORWARD_RIGHT_COMMAND;
    static QByteArray BACKWARD_LEFT_COMMAND;
    static QByteArray RIGHT_COMMAND;
    static QByteArray BACKWARD_RIGHT_COMMAND;
    static QByteArray FORWARD_TURBO_LEFT_COMMAND;
    static QByteArray FORWARD_TURBO_RIGHT_COMMAND;
    static QByteArray BACKWARD_TURBO_LEFT_COMMAND;
    static QByteArray BACKWARD_TURBO_RIGHT_COMMAND;

    QBluetoothUuid controlServiceUuid() const override;
    QBluetoothUuid controlCharacteristicsUuid() const override;
};

