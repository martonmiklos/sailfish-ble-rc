#include "ble_rc_car.h"

BLE_RC_Car::BLE_RC_Car(QObject *parent) :
    AbstractRC_Car(parent)
{

}

bool BLE_RC_Car::connectToDevice()
{
    if (m_controller && m_controller->state() == QLowEnergyController::ConnectedState) {
        m_controller->disconnectFromDevice();
        m_reconnecting = true;
    } else {
        if (!m_controller) {
            m_controller = new QLowEnergyController(m_devInfo);
            connect(m_controller, &QLowEnergyController::connected, this, &BLE_RC_Car::deviceConnected);
            connect(m_controller, &QLowEnergyController::disconnected, this, &BLE_RC_Car::deviceDisconnected);

            connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
                    this, SLOT(errorReceived(QLowEnergyController::Error)));
            connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
                    this, SLOT(addLowEnergyService(QBluetoothUuid)));
            connect(m_controller, SIGNAL(discoveryFinished()),
                    this, SLOT(serviceScanDone()));
            m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
        }
        m_controller->connectToDevice();
        setConnectionState(Connecting);
        setConnectionStateString(tr("Connecting to device"));
    }
    return true;
}

void BLE_RC_Car::disconnectFromDevice()
{
    m_controller->disconnectFromDevice();
}

void BLE_RC_Car::deviceDisconnected()
{

    if (m_reconnecting) {
        m_controller->connectToDevice();
        setConnectionState(Connecting);
    } else {
        setConnectionState(Disconnected);
    }
}

void BLE_RC_Car::deviceConnected()
{
    m_controller->discoverServices();
    setConnectionStateString(tr("Discovering services"));
}



void BLE_RC_Car::serviceScanDone()
{
    setConnectionState(Connected);
    setConnectionStateString(tr("Discovering services done, connected"));
}

void BLE_RC_Car::errorReceived(QLowEnergyController::Error error)
{
    qWarning() << error;
    if (m_connectionState != Connected) {
        setConnectionStateString(tr("Discovery error: %1").arg(error));
    }
}

void BLE_RC_Car::setConnectionStateString(const QString &newConnectionStateString)
{
    if (m_connectionStateString != newConnectionStateString) {
        m_connectionStateString = newConnectionStateString;
        emit connectionStateStringChanged();
    }
}

void BLE_RC_Car::setDevInfo(const QBluetoothDeviceInfo &newDevInfo)
{
    m_devInfo = newDevInfo;
}

QString BLE_RC_Car::connectionStateString() const
{
    return m_connectionStateString;
}

QString BLE_RC_Car::errorString() const
{
    return m_errorString;
}



