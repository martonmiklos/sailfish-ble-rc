#pragma once

#include "abstract_rc_car.h"

#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

class BLE_RC_Car : public AbstractRC_Car
{
    Q_OBJECT
public:
    BLE_RC_Car(QObject *parent = nullptr);

    virtual bool connectToDevice(const QBluetoothDeviceInfo &info);

    QString errorString() const;

protected slots:
    virtual void send() = 0;

    void deviceConnected();
    void deviceDisconnected();

    virtual void addLowEnergyService(const QBluetoothUuid &uuid) = 0;
    virtual void serviceScanDone();

    void errorReceived(QLowEnergyController::Error error);

protected:
    QLowEnergyController *m_controller = nullptr;
    QString m_errorString;
};

