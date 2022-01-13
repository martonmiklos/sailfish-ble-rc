#pragma once

#include "abstract_rc_car.h"

#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QElapsedTimer>

class BleRcCar : public AbstractRcCar
{
    Q_OBJECT
public:
    BleRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent = nullptr);

    virtual bool connectToDevice() override;
    virtual void disconnectFromDevice() override;

    QString errorString() const;

    void setDevInfo(const QBluetoothDeviceInfo &newDevInfo);
    QString connectionStateString() const override;

protected slots:
    virtual void send() = 0;

    void deviceConnected();
    void deviceDisconnected();

    virtual void serviceDiscovered(const QBluetoothUuid &uuid) = 0;

    void errorReceived(QLowEnergyController::Error error);

protected:
    QLowEnergyController *m_controller = nullptr;
    QString m_errorString, m_connectionStateString;
    void setConnectionStateString(const QString &newConnectionStateString);

    bool m_reconnecting = false;
    QElapsedTimer m_connectTimer;
    QBluetoothDeviceInfo m_carInfo;
};

