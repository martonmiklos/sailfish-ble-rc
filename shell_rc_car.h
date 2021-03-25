#pragma once

#include "ble_rc_car.h"

#include <QTimer>
#include <QLowEnergyController>

class Shell_RC_Car : public BLE_RC_Car
{
    Q_OBJECT
public:
    Shell_RC_Car(QObject *parent = nullptr);

    void setThrottle(float throttle);
    void setSteer(float steer);
    bool isFeatureSupported(Feature feature) const;
    bool setFeature(Feature feature, const QVariant &value);

    bool isDevice(const QBluetoothDeviceInfo &info);
    bool connectToDevice(const QBluetoothDeviceInfo &info);
    QString imagePath() const;

private slots:
    void send();
    void addLowEnergyService(const QBluetoothUuid &uuid);
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void serviceScanDone();
    void deviceDisconnected();

private:
    QTimer m_sendTimer;
    qreal m_deadband = 0.1;
    bool m_left = false;
    bool m_right = false;
    bool m_forward = false;
    bool m_backward = false;
    bool m_turbo = false;
    bool m_lamp = false;
    QLowEnergyController *m_controller = nullptr;
};

