#pragma once

#include "ble_rc_car.h"

#include <QTimer>

class ShellRcCar : public BleRcCar
{
public:
    explicit ShellRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent = nullptr);

    void setThrottle(float throttle) override;
    void setSteer(float steer) override;
    bool isFeatureSupported(Feature feature) const override;
    bool setFeature(Feature feature, const QVariant &value) override;
    QVariant featureValue(Feature feature) const override;

    bool connectToDevice() override;
    qreal batteryPercentage() override;

protected slots:
    void serviceDiscovered(const QBluetoothUuid &uuid) override;
    virtual void controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    virtual void batteryCharacteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value) = 0;

protected:
    virtual QBluetoothUuid controlServiceUuid() const = 0;
    virtual QBluetoothUuid controlCharacteristicsUuid() const = 0;

    QTimer m_sendTimer;
    qreal m_deadband = 0.1;
    bool m_left = false;
    bool m_right = false;
    bool m_forward = false;
    bool m_backward = false;
    bool m_turbo = false;
    bool m_lamp = false;
    QString m_connectionStateString;
    quint8 m_batteryPercentage = 0.0;

    QLowEnergyService *m_controlService = nullptr;
    QLowEnergyCharacteristic m_controlCharacteristics;
};

