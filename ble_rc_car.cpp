#include "ble_rc_car.h"

BLE_RC_Car::BLE_RC_Car(QObject *parent) :
    AbstractRC_Car(parent)
{

}

bool BLE_RC_Car::connectToDevice(const QBluetoothDeviceInfo &info)
{
    if (m_controller) {
        disconnect(m_controller, nullptr, this, nullptr);
        m_controller->disconnectFromDevice();
        m_controller->deleteLater();
    }
    m_controller = new QLowEnergyController(info);
    connect(m_controller, &QLowEnergyController::connected, this, &BLE_RC_Car::deviceConnected);
    connect(m_controller, &QLowEnergyController::disconnected, this, &BLE_RC_Car::deviceDisconnected);


    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(errorReceived(QLowEnergyController::Error)));

    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(addLowEnergyService(QBluetoothUuid)));
    connect(m_controller, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    m_controller->connectToDevice();
    setConnectionState(Connecting);
    return true;
}

void BLE_RC_Car::deviceConnected()
{
    m_controller->discoverServices();
}

void BLE_RC_Car::deviceDisconnected()
{
    setConnectionState(Disconnected);
}

void BLE_RC_Car::serviceScanDone()
{
    setConnectionState(Connected);
}

void BLE_RC_Car::errorReceived(QLowEnergyController::Error error)
{

}

QString BLE_RC_Car::errorString() const
{
    return m_errorString;
}



