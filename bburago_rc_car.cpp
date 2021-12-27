#include "bburago_rc_car.h"

QBluetoothUuid BburagoRcCar::CONTROL_SERVICE_UUID         = QBluetoothUuid(QStringLiteral("0000fff0-0000-1000-8000-00805f9b34fb"));
QBluetoothUuid BburagoRcCar::CONTROL_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("0000fff1-0000-1000-8000-00805f9b34fb"));
QBluetoothUuid BburagoRcCar::BATTERY_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("00002a19-0000-1000-8000-00805f9b34fb"));

BburagoRcCar::BburagoRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    ShellRcCar(carInfo, parent)
{

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

void BburagoRcCar::characteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (info.uuid() == BATTERY_CHARACTERISTICS_UUID) {
        if (value.length() == 1) {
            if (m_batteryPercentage != value.at(1)) {
                m_batteryPercentage = value.at(1);
                emit batteryPercentageUpdated();
            }
        }
    }
}

QBluetoothUuid BburagoRcCar::controlServiceUuid() const
{
    return CONTROL_SERVICE_UUID;
}

QBluetoothUuid BburagoRcCar::controlCharacteristicsUuid() const
{
    return CONTROL_CHARACTERISTICS_UUID;
}
