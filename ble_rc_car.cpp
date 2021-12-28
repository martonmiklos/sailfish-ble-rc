#include "ble_rc_car.h"

BleRcCar::BleRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    AbstractRcCar(parent)
{
    m_controller = new QLowEnergyController(carInfo);
    m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    connect(m_controller, &QLowEnergyController::connected, this, &BleRcCar::deviceConnected);
    connect(m_controller, &QLowEnergyController::disconnected, this, &BleRcCar::deviceDisconnected);

    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(errorReceived(QLowEnergyController::Error)));
    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(m_controller, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
}

bool BleRcCar::connectToDevice()
{
    if (m_controller->state() == QLowEnergyController::ConnectedState) {
        m_controller->disconnectFromDevice();
        m_reconnecting = true;
    } else {
        m_controller->connectToDevice();
        setConnectionState(Connecting);
        setConnectionStateString(tr("Connecting to device"));
    }
    return true;
}

void BleRcCar::disconnectFromDevice()
{
    m_controller->disconnectFromDevice();
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

void BleRcCar::deviceConnected()
{
    m_connectTimer.restart();
    m_controller->discoverServices();
    setConnectionStateString(tr("Discovering services"));
}

void BleRcCar::serviceScanDone()
{
    setConnectionState(Connected);
    setConnectionStateString(tr("Discovering services done, connected"));
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



