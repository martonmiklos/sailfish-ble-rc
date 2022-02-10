#include "ble_rc_car.h"

#include <QLowEnergyController>

BleRcCar::BleRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    AbstractRcCar(parent)
{
    m_carInfo = carInfo;
}

bool BleRcCar::connectToDevice()
{
    if (m_controller && m_controller->state() == QLowEnergyController::ConnectedState)
        return false;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    m_controller = QLowEnergyController::createCentral(m_carInfo, this);
#else
    m_controller = new QLowEnergyController(m_carInfo.address(), this);
#endif
    m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    connect(m_controller, &QLowEnergyController::connected, this, &BleRcCar::deviceConnected);
    connect(m_controller, &QLowEnergyController::disconnected, this, &BleRcCar::deviceDisconnected);

    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(errorReceived(QLowEnergyController::Error)));
    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(serviceDiscovered(QBluetoothUuid)));
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    m_controller->connectToDevice();
    setConnectionState(Connecting);
    setConnectionStateString(tr("Connecting to device"));
    return true;
}

void BleRcCar::deviceConnected()
{
    m_connectTimer.restart();
    m_controller->discoverServices();
    setConnectionStateString(tr("Discovering BLE services"));
}

void BleRcCar::disconnectFromDevice()
{
    m_controller->disconnectFromDevice();
    disconnect(m_controller, nullptr, this, nullptr);
    m_controller->deleteLater();
    m_controller = nullptr;
}

void BleRcCar::deviceDisconnected()
{
    if (m_reconnecting) {
        m_controller->connectToDevice();
        setConnectionState(Connecting);
    } else {
        setConnectionState(Disconnected);
    }
}

void BleRcCar::errorReceived(QLowEnergyController::Error error)
{
    qWarning() << error << m_connectTimer.elapsed();
    if (m_connectionState != Connected) {
        setConnectionStateString(tr("Discovery error: %1").arg(error));
    }
}

void BleRcCar::setConnectionStateString(const QString &newConnectionStateString)
{
    if (m_connectionStateString != newConnectionStateString) {
        m_connectionStateString = newConnectionStateString;
        emit connectionStateStringChanged();
    }
}

QString BleRcCar::connectionStateString() const
{
    return m_connectionStateString;
}

QString BleRcCar::errorString() const
{
    return m_errorString;
}



