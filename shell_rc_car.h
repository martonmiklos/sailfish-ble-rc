#pragma once

#include "ble_rc_car.h"

#include <QTimer>
#include <QLowEnergyController>

class QAESEncryption;

class Shell_RC_Car : public BLE_RC_Car
{
    Q_OBJECT
public:
    Shell_RC_Car(QObject *parent = nullptr);

    void setThrottle(float throttle) override;
    void setSteer(float steer) override;
    bool isFeatureSupported(Feature feature) const override;
    bool setFeature(Feature feature, const QVariant &value) override;
    QVariant featureValue(Feature feature) const override;

    bool connectToDevice() override;

    static bool isDevice(const QBluetoothDeviceInfo &info);
    static QString imagePath();
    QString name() const override;
    qreal batteryPercentage() override;

protected slots:
    void send() override;
    void serviceScanDone() override;
    void addLowEnergyService(const QBluetoothUuid &uuid) override;
    void controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void characteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value);
private:
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
    QAESEncryption *m_aesDecryptor;

    QLowEnergyService *m_controlService = nullptr;
    QLowEnergyCharacteristic m_controlCharacteristics;
    QLowEnergyCharacteristic m_batteryCharacteristics;

    static QString CONTROL_SERVICE_UUID;
    static QString CONTROL_CHARACTERISTICS_UUID;
    static QString BATTERY_CHARACTERISTICS_UUID;

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

    void processControlServiceCharacteristics();
};

