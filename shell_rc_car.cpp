#include "shell_rc_car.h"

ShellRcCar::ShellRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    BleRcCar(carInfo, parent)
{
    m_sendTimer.setSingleShot(false);
    m_sendTimer.setInterval(50);
    connect(&m_sendTimer, &QTimer::timeout, this, &ShellRcCar::send);
}

bool ShellRcCar::isFeatureSupported(AbstractRcCar::Feature feature) const
{
    switch (feature) {
    case AbstractRcCar::BatteryVoltageQuery:
    case AbstractRcCar::TurboMode:
    case AbstractRcCar::Lamp:
        return true;
    }
    return false;
}

void ShellRcCar::setThrottle(float throttle)
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

void ShellRcCar::setSteer(float steer)
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

bool ShellRcCar::setFeature(AbstractRcCar::Feature feature, const QVariant &value)
{
    if (!isFeatureSupported(feature))
        return false;

    if (feature == Feature::TurboMode)
        m_turbo = value.toBool();
    else if (feature == Feature::Lamp)
        m_lamp = value.toBool();
    return true;
}

QVariant ShellRcCar::featureValue(AbstractRcCar::Feature feature) const
{
    switch (feature) {
    case AbstractRcCar::BatteryVoltageQuery:
        return m_batteryPercentage;
    case AbstractRcCar::TurboMode:
        return m_turbo;
    case AbstractRcCar::Lamp:
        return m_lamp;
    }
    return QVariant();
}


bool ShellRcCar::connectToDevice()
{
    if (m_controlService) {
        disconnect(m_controlService, nullptr, this, nullptr);
        delete m_controlService;
        m_controlService = nullptr;
    }
    return BleRcCar::connectToDevice();
}

qreal ShellRcCar::batteryPercentage()
{
    return m_batteryPercentage;
}

void ShellRcCar::serviceDiscovered(const QBluetoothUuid &uuid)
{
    if (uuid == controlServiceUuid() && m_controlService == nullptr) {
        m_controlService = m_controller->createServiceObject(uuid);
        if (m_controlService->state() == QLowEnergyService::DiscoveryRequired) {
            connect(m_controlService, &QLowEnergyService::stateChanged,
                    this, &ShellRcCar::controlServiceDetailsDiscovered);
            m_controlService->discoverDetails();
        }
    }
}

void ShellRcCar::controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered)
        return;

    qWarning() << "Control service discovered" << newState;
    m_controlCharacteristics = m_controlService->characteristic(controlCharacteristicsUuid());
    if (m_controlCharacteristics.isValid()) {
        qWarning() << "Connected!";
        setConnectionState(Connected);
        m_sendTimer.start();
    } else {
        qWarning() << "No characteristics " << controlCharacteristicsUuid() << " have been found in the control service";
    }
}

