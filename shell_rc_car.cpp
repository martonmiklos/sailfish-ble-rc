#include "shell_rc_car.h"

Shell_RC_Car::Shell_RC_Car(QObject *parent) :
    BLE_RC_Car(parent)
{
    m_sendTimer.setSingleShot(false);
    m_sendTimer.setInterval(100);
    connect(&m_sendTimer, &QTimer::timeout, this, &Shell_RC_Car::send);
}

void Shell_RC_Car::setThrottle(float throttle)
{
    if (throttle > m_deadband) {
        m_forward = true;
        m_backward = false;
    } else if (throttle < -m_deadband) {
        m_forward = false;
        m_backward = true;
    } else {
        m_forward = false;
        m_backward = false;
    }
}

void Shell_RC_Car::setSteer(float steer)
{
    if (steer > m_deadband) {
        m_right = true;
        m_left = false;
    } else if (steer < -m_deadband) {
        m_right = false;
        m_left = true;
    } else {
        m_right = false;
        m_left = false;
    }
}

bool Shell_RC_Car::isFeatureSupported(AbstractRC_Car::Feature feature) const
{
    switch (feature) {
    case AbstractRC_Car::BatteryVoltageQuery:
        return false;
    case AbstractRC_Car::TurboMode:
    case AbstractRC_Car::Lamp:
        return true;
    }
    return false;
}

bool Shell_RC_Car::setFeature(AbstractRC_Car::Feature feature, const QVariant &value)
{
    if (!isFeatureSupported(feature))
        return false;

    if (feature == Feature::TurboMode)
        m_turbo = value.toBool();
    return true;
}

bool Shell_RC_Car::isDevice(const QBluetoothDeviceInfo &info)
{
    return info.name().startsWith(QStringLiteral("QCAR"));
}

bool Shell_RC_Car::connectToDevice(const QBluetoothDeviceInfo &info)
{
    if (m_controller) {
        disconnect(m_controller, nullptr, this, nullptr);
        m_controller->disconnectFromDevice();
        m_controller->deleteLater();
    }
    m_controller = new QLowEnergyController(info);
    connect(m_controller, SIGNAL(connected()),
            this, SLOT(deviceConnected()));
    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(errorReceived(QLowEnergyController::Error)));
    connect(m_controller, SIGNAL(disconnected()),
            this, SLOT(deviceDisconnected()));
    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(addLowEnergyService(QBluetoothUuid)));
    connect(m_controller, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    m_controller->connectToDevice();
    setConnectionState(Connecting);
    return true;
}

void Shell_RC_Car::send()
{

}

void Shell_RC_Car::addLowEnergyService(const QBluetoothUuid &uuid)
{

}

void Shell_RC_Car::deviceConnected()
{

}

void Shell_RC_Car::errorReceived(QLowEnergyController::Error)
{

}

void Shell_RC_Car::serviceScanDone()
{

}

void Shell_RC_Car::deviceDisconnected()
{

}

QString Shell_RC_Car::imagePath() const
{
    return QStringLiteral("://res/deviceIcons/shell.png");
}
