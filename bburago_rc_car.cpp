#include "bburago_rc_car.h"

QBluetoothUuid BburagoRcCar::CONTROL_SERVICE_UUID         = QBluetoothUuid(QStringLiteral("0000fff0-0000-1000-8000-00805f9b34fb"));
QBluetoothUuid BburagoRcCar::BATTERY_SERVICE_UUID         = QBluetoothUuid(QStringLiteral("0000180f-0000-1000-8000-00805f9b34fb"));

QBluetoothUuid BburagoRcCar::CONTROL_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("0000fff1-0000-1000-8000-00805f9b34fb"));
QBluetoothUuid BburagoRcCar::BATTERY_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("00002a19-0000-1000-8000-00805f9b34fb"));

BburagoRcCar::BburagoRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    ShellRcCar(carInfo, parent)
{

}

void BburagoRcCar::serviceScanDone()
{
    qWarning() << "BburagoRcCar service scan done";
}

bool BburagoRcCar::isDevice(const QBluetoothDeviceInfo &info)
{
    return info.name().startsWith(QStringLiteral("SL-FXX-K Evo"))
            || info.name().startsWith(QStringLiteral("SL-SF1000"));
}

QString BburagoRcCar::name() const
{
    return  tr("Bburago Shell BLE RC car");
}

QString BburagoRcCar::imagePath(const QBluetoothDeviceInfo &info)
{
    if (info.name().startsWith(QStringLiteral("SL-FXX-K Evo")))
        return QStringLiteral("qrc:/res/deviceIcons/bburago_sl-fxx-k-evo.png");
    else if (info.name().startsWith(QStringLiteral("SL-SF1000")))
        return QStringLiteral("qrc:/res/deviceIcons/bburago_sl-sf1000.png");
    return QStringLiteral("qrc:/res/deviceIcons/unknown.png");
}

bool BburagoRcCar::connectToDevice()
{
    disconnect(m_batteryService, nullptr, this, nullptr);
    delete m_batteryService;
    m_batteryService = nullptr;

    return ShellRcCar::connectToDevice();
}

void BburagoRcCar::serviceDiscovered(const QBluetoothUuid &uuid)
{
    qWarning() << "service discovered" << uuid;
    ShellRcCar::serviceDiscovered(uuid);
    if (uuid == batteryServiceUuid() && m_batteryService == nullptr) {
        m_batteryService = m_controller->createServiceObject(uuid);
        if (m_batteryService->state() == QLowEnergyService::DiscoveryRequired) {
            connect(m_batteryService, &QLowEnergyService::stateChanged,
                    this, &BburagoRcCar::batteryServiceDetailsDiscovered);
            m_batteryService->discoverDetails();
        }
    }
}

void BburagoRcCar::send()
{
    QByteArray data = QByteArrayLiteral("\x01\x00\x00\x00\x00\x00\x00\x00");
    if (m_turbo)
        data[6] = 1;

    if (m_lamp)
        data[5] = 1;

    if (m_forward)
        data[1] = 1;
    else if (m_backward)
        data[2] = 1;

    if (m_right)
        data[4] = 1;
    else if (m_left)
        data[3] = 1;
    m_controlService->writeCharacteristic(m_controlCharacteristics, data, QLowEnergyService::WriteWithoutResponse);
}

void BburagoRcCar::batteryCharacteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (info.uuid() == BATTERY_CHARACTERISTICS_UUID) {
        if (value.length() == 1) {
            if (m_batteryPercentage != value.at(0)) {
                m_batteryPercentage = value.at(0);
                emit batteryPercentageUpdated();
            }
        }
    }
}

QBluetoothUuid BburagoRcCar::controlServiceUuid() const
{
    return CONTROL_SERVICE_UUID;
}

QBluetoothUuid BburagoRcCar::batteryServiceUuid() const
{
    return BATTERY_SERVICE_UUID;
}

void BburagoRcCar::batteryServiceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    /*if (newState == QLowEnergyService::ServiceDiscovered) {
        auto batteryCharacteristic = m_batteryService->characteristic(BATTERY_CHARACTERISTICS_UUID);
        if (batteryCharacteristic.isValid()) {
            QLowEnergyDescriptor notification = batteryCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            if (notification.isValid()) {
                m_batteryService->writeDescriptor(notification, QByteArray::fromHex("0100"));
                connect(m_batteryService, &QLowEnergyService::characteristicChanged,
                        this,  &BburagoRcCar::batteryCharacteristicChanged);
            } else {
                qWarning() << "Battery notification invalid";
                return;
            }
        } else {
            qWarning() << "No battery characteristics found";
        }
    }*/
}

QBluetoothUuid BburagoRcCar::controlCharacteristicsUuid() const
{
    return CONTROL_CHARACTERISTICS_UUID;
}
