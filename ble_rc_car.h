#pragma once

#include "abstract_rc_car.h"

#include <QBluetoothDeviceInfo>

class BLE_RC_Car : public AbstractRC_Car
{
    Q_OBJECT
public:
    BLE_RC_Car(QObject *parent = nullptr);

    virtual bool isDevice(const QBluetoothDeviceInfo & info) = 0;
};

