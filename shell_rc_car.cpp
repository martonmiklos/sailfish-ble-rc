#include "shell_rc_car.h"

#include <QLowEnergyCharacteristic>

QString Shell_RC_Car::CONTROL_SERVICE_UUID = "0000fff0-0000-1000-8000-00805f9b34fb";
QString Shell_RC_Car::CONTROL_CHARACTERISTICS_UUID = "d44bc439-abfd-45a2-b575-925416129600";

QByteArray Shell_RC_Car::IDLE_COMMAND = QByteArrayLiteral("\x02\x5e\x69\x5a\x48\xff\x2a\x43\x8c\xa6\x80\xf8\x3e\x04\xe4\x5d"); // // Alapállapotba állít
QByteArray Shell_RC_Car::FORWARD_COMMAND = QByteArrayLiteral("\x29\x60\x9c\x66\x48\x52\xcf\xf1\xb0\xf0\xcb\xb9\x80\x14\xbd\x2c"); // // Előre
QByteArray Shell_RC_Car::FORWARD_TURBO_COMMAND = QByteArrayLiteral("\xe6\x55\x67\xda\x8e\x6c\x56\x0d\x09\xd3\x73\x3a\x7f\x47\xff\x06"); // // Turbó előre
QByteArray Shell_RC_Car::BACKWARD_COMMAND = QByteArrayLiteral("\x03\x20\x99\x09\xba\x9d\xa1\xc8\xb9\x86\x16\x3c\x6d\x48\x46\x55"); // // Hátra
QByteArray Shell_RC_Car::BACKWARD_TURBO_COMMAND = QByteArrayLiteral("\xce\xc2\xff\x1d\x7a\xcc\x16\x3c\xd1\x3b\x7e\x61\x53\xad\x5c\x45"); // // Turbó hátra
QByteArray Shell_RC_Car::FORWARD_LEFT_COMMAND = QByteArrayLiteral("\x99\x28\xe5\x90\xdf\xe8\x21\x48\x5f\x41\x4f\xbb\x63\x3d\x5c\x4e"); // // Előre és balra
QByteArray Shell_RC_Car::LEFT_COMMAND = QByteArrayLiteral("\x51\x38\x21\x12\x13\x5c\xcc\xdb\x46\xcf\x89\x21\xb7\x05\x49\x9a"); // // Balra
QByteArray Shell_RC_Car::FORWARD_RIGHT_COMMAND = QByteArrayLiteral("\x0f\x2c\xe5\x66\x62\xd4\xfd\x9d\x32\xa4\x4f\x10\x2b\xf2\x0a\xa7"); // // Előre és jobbra
QByteArray Shell_RC_Car::BACKWARD_LEFT_COMMAND = QByteArrayLiteral("\x98\xce\x98\x1d\x58\xd1\x15\xaf\xe1\x19\x60\xbf\x46\x13\x92\x5c"); // // Hátra és balra
QByteArray Shell_RC_Car::RIGHT_COMMAND = QByteArrayLiteral("\x1b\x57\x69\xcd\xf1\x3e\x8a\xb6\x27\x08\x0f\xf3\xce\xfc\x3b\xc0"); // // Jobbra
QByteArray Shell_RC_Car::BACKWARD_RIGHT_COMMAND = QByteArrayLiteral("\xf2\x52\x0f\xba\x31\x44\xfb\x11\x46\x8f\xe0\x80\xc6\xc2\xc2\x3c"); // // Hátra és jobbra
QByteArray Shell_RC_Car::FORWARD_TURBO_LEFT_COMMAND = QByteArrayLiteral("\x59\x23\x81\xc9\x43\xa4\x17\xca\x1b\xc3\xb5\x94\x00\xe0\xfc\x12"); // // Turbó előre és balra
QByteArray Shell_RC_Car::FORWARD_TURBO_RIGHT_COMMAND = QByteArrayLiteral("\xfb\x97\x6f\xba\x04\xaf\x87\x02\x22\x26\xec\x50\xae\x82\xf8\xc4"); // // Turbó előre és jobbra
QByteArray Shell_RC_Car::BACKWARD_TURBO_LEFT_COMMAND = QByteArrayLiteral("\xd5\x4a\xd5\x58\x57\xd3\x27\x74\x5f\x14\x1d\xd0\x0d\x67\x15\x95"); // // Turbó hátra és balra
QByteArray Shell_RC_Car::BACKWARD_TURBO_RIGHT_COMMAND = QByteArrayLiteral("\x80\xdf\xb2\x16\x5f\x32\x60\xf1\xd9\x83\x77\x50\xf4\x3a\x43\xda"); // // Turbó hátra és jobbra

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

bool Shell_RC_Car::connectToDevice(const QBluetoothDeviceInfo &info)
{
    if (m_controlService) {
        disconnect(m_controlService, nullptr, this, nullptr);
        delete m_controlService;
        m_controlService = nullptr;
    }
    return BLE_RC_Car::connectToDevice(info);
}


void Shell_RC_Car::addLowEnergyService(const QBluetoothUuid &uuid)
{
    if (uuid == QBluetoothUuid(CONTROL_SERVICE_UUID) && m_controlService == nullptr)
        m_controlService = m_controller->createServiceObject(uuid);
}

void Shell_RC_Car::serviceScanDone()
{
    if (m_controlService) {
        if (m_controlService->state() == QLowEnergyService::DiscoveryRequired) {
            connect(m_controlService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                    this, SLOT(controlServiceDetailsDiscovered(QLowEnergyService::ServiceState)));
            m_controlService->discoverDetails();
            return;
        }
        processControlServiceCharacteristics();
    } else {
        // WTF no control service found...
    }
}

void Shell_RC_Car::controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered)
        return;

    processControlServiceCharacteristics();
}

void Shell_RC_Car::processControlServiceCharacteristics()
{
    m_controlCharacteristics = m_controlService->characteristic(QBluetoothUuid(CONTROL_CHARACTERISTICS_UUID));
    if (m_controlCharacteristics.isValid()) {
        setConnectionState(Connected);
        m_sendTimer.start();
    }
}

bool Shell_RC_Car::isDevice(const QBluetoothDeviceInfo &info)
{
    return info.name().startsWith(QStringLiteral("QCAR"));
}

QString Shell_RC_Car::imagePath()
{
    return QStringLiteral("qrc:/res/deviceIcons/shell.png");
}

QString Shell_RC_Car::name() const
{
    return  tr("Shell BLE RC car");
}

void Shell_RC_Car::send()
{
    if (m_connectionState != Connected)
        return;

    QByteArray data;
    if (!m_left && !m_right && !m_forward && !m_backward) {
        data = IDLE_COMMAND;
    }

    if (m_turbo) {
        if (m_forward && !m_left && !m_right) {
            data = FORWARD_TURBO_COMMAND;
        } else if (m_backward && !m_left && !m_right) {
            data = BACKWARD_TURBO_COMMAND;
        } else if (m_forward && m_left && !m_right) {
            data = FORWARD_TURBO_LEFT_COMMAND;
        } else if (m_forward && !m_left && m_right) {
            data = FORWARD_TURBO_RIGHT_COMMAND;
        } else if (m_backward && m_left && !m_right) {
            data = BACKWARD_TURBO_LEFT_COMMAND;
        } else if (m_backward && !m_left && m_right) {
            data = BACKWARD_TURBO_RIGHT_COMMAND;
        }
    } else {
        if (m_left && !m_forward && !m_backward) {
            data = LEFT_COMMAND;
        } else if (m_right && !m_forward && !m_backward) {
            data = RIGHT_COMMAND;
        } else if (m_forward && !m_left && !m_right) {
            data = FORWARD_COMMAND;
        } else if (m_backward && !m_left && !m_right) {
            data = BACKWARD_COMMAND;
        } else if (m_forward && m_left && !m_right) {
            data = FORWARD_LEFT_COMMAND;
        } else if (m_forward && !m_left && m_right) {
            data = FORWARD_RIGHT_COMMAND;
        } else if (m_backward && m_left && !m_right) {
            data = BACKWARD_LEFT_COMMAND;
        } else if (m_backward && !m_left && m_right) {
            data = BACKWARD_RIGHT_COMMAND;
        }
    }
    m_controlService->writeCharacteristic(m_controlCharacteristics, data, QLowEnergyService::WriteWithoutResponse);
}


