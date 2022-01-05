#include "brandbase_rc_car.h"

#include "Qt-AES/qaesencryption.h"
#include <QLowEnergyCharacteristic>

QBluetoothUuid BrandbaseRcCar::CONTROL_SERVICE_UUID         = QBluetoothUuid(QStringLiteral("0000fff0-0000-1000-8000-00805f9b34fb"));
QBluetoothUuid BrandbaseRcCar::CONTROL_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("d44bc439-abfd-45a2-b575-925416129600"));
QBluetoothUuid BrandbaseRcCar::BATTERY_CHARACTERISTICS_UUID = QBluetoothUuid(QStringLiteral("d44bc439-abfd-45a2-b575-925416129601"));

QByteArray BrandbaseRcCar::AES_KEY = QByteArrayLiteral("\x34\x52\x2a\x5b\x7a\x6e\x49\x2c\x08\x09\x0a\x9d\x8d\x2a\x23\xf8");
QByteArray BrandbaseRcCar::IDLE_COMMAND = QByteArrayLiteral("\x02\x5e\x69\x5a\x48\xff\x2a\x43\x8c\xa6\x80\xf8\x3e\x04\xe4\x5d"); //Alapállapotba állít
QByteArray BrandbaseRcCar::FORWARD_COMMAND = QByteArrayLiteral("\x29\x60\x9c\x66\x48\x52\xcf\xf1\xb0\xf0\xcb\xb9\x80\x14\xbd\x2c"); //Előre
QByteArray BrandbaseRcCar::FORWARD_TURBO_COMMAND = QByteArrayLiteral("\xe6\x55\x67\xda\x8e\x6c\x56\x0d\x09\xd3\x73\x3a\x7f\x47\xff\x06"); //Turbó előre
QByteArray BrandbaseRcCar::BACKWARD_COMMAND = QByteArrayLiteral("\x03\x20\x99\x09\xba\x9d\xa1\xc8\xb9\x86\x16\x3c\x6d\x48\x46\x55"); //Hátra
QByteArray BrandbaseRcCar::BACKWARD_TURBO_COMMAND = QByteArrayLiteral("\xce\xc2\xff\x1d\x7a\xcc\x16\x3c\xd1\x3b\x7e\x61\x53\xad\x5c\x45"); //Turbó hátra
QByteArray BrandbaseRcCar::FORWARD_LEFT_COMMAND = QByteArrayLiteral("\x99\x28\xe5\x90\xdf\xe8\x21\x48\x5f\x41\x4f\xbb\x63\x3d\x5c\x4e"); //Előre és balra
QByteArray BrandbaseRcCar::LEFT_COMMAND = QByteArrayLiteral("\x51\x38\x21\x12\x13\x5c\xcc\xdb\x46\xcf\x89\x21\xb7\x05\x49\x9a"); //Balra
QByteArray BrandbaseRcCar::FORWARD_RIGHT_COMMAND = QByteArrayLiteral("\x0f\x2c\xe5\x66\x62\xd4\xfd\x9d\x32\xa4\x4f\x10\x2b\xf2\x0a\xa7"); //Előre és jobbra
QByteArray BrandbaseRcCar::BACKWARD_LEFT_COMMAND = QByteArrayLiteral("\x98\xce\x98\x1d\x58\xd1\x15\xaf\xe1\x19\x60\xbf\x46\x13\x92\x5c"); //Hátra és balra
QByteArray BrandbaseRcCar::RIGHT_COMMAND = QByteArrayLiteral("\x1b\x57\x69\xcd\xf1\x3e\x8a\xb6\x27\x08\x0f\xf3\xce\xfc\x3b\xc0"); //Jobbra
QByteArray BrandbaseRcCar::BACKWARD_RIGHT_COMMAND = QByteArrayLiteral("\xf2\x52\x0f\xba\x31\x44\xfb\x11\x46\x8f\xe0\x80\xc6\xc2\xc2\x3c"); //Hátra és jobbra
QByteArray BrandbaseRcCar::FORWARD_TURBO_LEFT_COMMAND = QByteArrayLiteral("\x59\x23\x81\xc9\x43\xa4\x17\xca\x1b\xc3\xb5\x94\x00\xe0\xfc\x12"); //Turbó előre és balra
QByteArray BrandbaseRcCar::FORWARD_TURBO_RIGHT_COMMAND = QByteArrayLiteral("\xfb\x97\x6f\xba\x04\xaf\x87\x02\x22\x26\xec\x50\xae\x82\xf8\xc4"); //Turbó előre és jobbra
QByteArray BrandbaseRcCar::BACKWARD_TURBO_LEFT_COMMAND = QByteArrayLiteral("\xd5\x4a\xd5\x58\x57\xd3\x27\x74\x5f\x14\x1d\xd0\x0d\x67\x15\x95"); //Turbó hátra és balra
QByteArray BrandbaseRcCar::BACKWARD_TURBO_RIGHT_COMMAND = QByteArrayLiteral("\x80\xdf\xb2\x16\x5f\x32\x60\xf1\xd9\x83\x77\x50\xf4\x3a\x43\xda"); //Turbó hátra és jobbra

BrandbaseRcCar::BrandbaseRcCar(const QBluetoothDeviceInfo &carInfo, QObject *parent) :
    ShellRcCar(carInfo, parent)
{
    m_aesDecryptor = new QAESEncryption(QAESEncryption::AES_128, QAESEncryption::ECB);
}

QBluetoothUuid BrandbaseRcCar::controlServiceUuid() const
{
    return CONTROL_SERVICE_UUID;
}

QBluetoothUuid BrandbaseRcCar::controlCharacteristicsUuid() const
{
    return CONTROL_CHARACTERISTICS_UUID;
}

void BrandbaseRcCar::batteryCharacteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (info.uuid() == BATTERY_CHARACTERISTICS_UUID) {
        QByteArray decoded = m_aesDecryptor->decode(value, AES_KEY);
        if (decoded.length() == 16) {
            if (m_batteryPercentage != decoded.at(4)) {
                m_batteryPercentage = decoded.at(4);
                emit batteryPercentageUpdated();
            }
        }
    }
}

bool BrandbaseRcCar::isDevice(const QBluetoothDeviceInfo &info)
{
    return info.name().startsWith(QStringLiteral("QCAR"));
}

QString BrandbaseRcCar::imagePath()
{
    return QStringLiteral("qrc:/res/deviceIcons/brandbase_nissan_formula_e.png");
}

QString BrandbaseRcCar::name() const
{
    return tr("Brandbase Shell BLE RC car");
}

void BrandbaseRcCar::send()
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

void BrandbaseRcCar::controlServiceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    ShellRcCar::controlServiceDetailsDiscovered(newState);
    if (newState == QLowEnergyService::ServiceDiscovered) {
        /*connect(m_controlService, &QLowEnergyService::characteristicChanged,
                this,  &BrandbaseRcCar::batteryCharacteristicChanged);*/
    }
}

