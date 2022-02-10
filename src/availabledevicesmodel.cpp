#include "availabledevicesmodel.h"

#include "brandbase_rc_car.h"
#include "bburago_rc_car.h"
#include "settings.h"

#include <QBluetoothLocalDevice>

AvailableDevicesModel::AvailableDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_statusString = tr("Start discovery from the pulley menu");
    auto adapters = QBluetoothLocalDevice::allDevices();
    if (adapters.count() == 0)
        return;

    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(adapters.last().address());
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
    connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(deviceScanCancelled()));

    if (Settings::instance()->autoDiscoverBlDevices())
        discoverDevices();
}

int AvailableDevicesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_devices.count();
}

int AvailableDevicesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant AvailableDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (static_cast<Role>(role)) {
    case AvailableDevicesModel::Name:
        return m_devices.at(index.row()).name();
    case AvailableDevicesModel::TypeName:
        return m_devices.at(index.row()).typeName;
    case AvailableDevicesModel::ImagePath:
        return m_devices.at(index.row()).imagePath;
    case AvailableDevicesModel::Index:
        return index.row();
    case AvailableDevicesModel::Alias:
        return m_devices.at(index.row()).alias();
    case AvailableDevicesModel::AutoConnect:
        return m_devices.at(index.row()).autoConnect();
    case AvailableDevicesModel::MacAddress:
        return m_devices.at(index.row()).btInfo().address().toString();
    }

    // for desktop
    if (role == Qt::DisplayRole)
        return m_devices.at(index.row()).name();
    return QVariant();
}

void AvailableDevicesModel::discoverDevices()
{
    if (!m_discoveryAgent)
        return;

    if (m_currentDevice) {
        m_currentDevice->disconnectFromDevice();
        m_currentDevice = nullptr;
    }
    beginRemoveRows(QModelIndex(), 0, m_devices.count());
    endRemoveRows();
    m_devices.clear();
    setStatusString(tr("Discovering devices..."));
    setScanInProgress(true);
    m_discoveryAgent->start();
}

void AvailableDevicesModel::deviceDiscovered(const QBluetoothDeviceInfo & info)
{
    qWarning() << info.address().toString() << info.name();
    if (BrandbaseRcCar::isDevice(info)) {
        DetectedDevice d(info, Brandbase);
        d.typeName = tr("Brandbase Shell Bluetooth RC car");
        d.imagePath = BrandbaseRcCar::imagePath();
        d.type = Brandbase;
        beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
        m_devices.append(d);
        endInsertRows();
    } else if (BburagoRcCar::isDevice(info)) {
        DetectedDevice d(info, Bburago);
        d.typeName = tr("Bburago Ferrari Bluetooth RC car");
        d.imagePath = BburagoRcCar::imagePath(info);
        d.type = Bburago;
        beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
        m_devices.append(d);
        endInsertRows();
    }

    if (Settings::instance()->autoConnect(info.address().toString())) {
        m_deviceToBeOpenedOnDiscoverCancelled = info.address().toString();
        cancelDiscovery();
    }
}

void AvailableDevicesModel::deviceScanFinished()
{
    setScanInProgress(false);
    if (m_devices.count() == 0)
        setStatusString(tr("No devices found"));
    else
        setStatusString(tr("Discovered devices"));
}

void AvailableDevicesModel::deviceScanCancelled()
{
    if (m_deviceToBeOpenedOnDiscoverCancelled.length()) {
        connectToDevice(m_deviceToBeOpenedOnDiscoverCancelled);
        m_deviceToBeOpenedOnDiscoverCancelled.clear();
    }
}

void AvailableDevicesModel::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (m_scanInProgress) {
        switch (error) {
        case QBluetoothDeviceDiscoveryAgent::NoError:
            break;
        case QBluetoothDeviceDiscoveryAgent::InputOutputError:
            setStatusString(tr("IO error during discovery"));
            break;
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
            setStatusString(tr("Please turn on Bluetooth"));
            break;
        case QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError:
            setStatusString(tr("Invalid BT adapter"));
            break;
        case QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError:
            setStatusString(tr("Platform not supported"));
            break;
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
            setStatusString(tr("Unknown error during discovery"));
            break;
        }
    }
    setScanInProgress(false);
}

void AvailableDevicesModel::currentDeviceConnectionStateChangedSlot(AbstractRcCar::ConnectionState oldState, AbstractRcCar::ConnectionState newState)
{
    Q_UNUSED(oldState)
    if (m_currentDevice) {
        if (newState == AbstractRcCar::Disconnected) {
            m_currentDevice->deleteLater();
            m_currentDevice = nullptr;
        }
        qWarning() << oldState << newState;
        emit currentDeviceConnectionStateChanged(oldState, newState);
    }
}

void AvailableDevicesModel::currentDeviceConnectionStateStringChangedSlot()
{
    emit currentDeviceConnectionStateStringChanged(m_currentDevice->connectionStateString());
}

void AvailableDevicesModel::setScanInProgress(bool scanInProgress)
{
    if (m_scanInProgress != scanInProgress) {
        m_scanInProgress = scanInProgress;
        emit scanStateChanged();
    }
}

AbstractRcCar *AvailableDevicesModel::currentDevice() const
{
    return m_currentDevice;
}

QString AvailableDevicesModel::statusString() const
{
    return m_statusString;
}

void AvailableDevicesModel::setStatusString(const QString &statusString)
{
    if (m_statusString != statusString) {
        m_statusString = statusString;
        emit statusStringChanged();
    }
}

QObject *AvailableDevicesModel::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static AvailableDevicesModel instance;
    return &instance;
}

void AvailableDevicesModel::disconnectFromCurrentDevice()
{
    if (m_currentDevice)
        m_currentDevice->disconnectFromDevice();
}

void AvailableDevicesModel::setAutoConnect(int row, bool autoConnect)
{
    m_devices[row].setAutoConnect(autoConnect);
    emit dataChanged(index(row, 0), index(row, 0), QVector<int>({AvailableDevicesModel::AutoConnect}));
}

bool AvailableDevicesModel::scanInProgress() const
{
    return m_scanInProgress;
}

QHash<int, QByteArray> AvailableDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeName] = QByteArrayLiteral("TypeName");
    roles[Alias] = QByteArrayLiteral("Alias");
    roles[ImagePath] = QByteArrayLiteral("ImagePath");
    roles[Name] = QByteArrayLiteral("Name");
    roles[Index] = QByteArrayLiteral("Index");
    roles[MacAddress] = QByteArrayLiteral("MacAddress");
    roles[AutoConnect] = QByteArrayLiteral("AutoConnect");
    return roles;
}

void AvailableDevicesModel::cancelDiscovery()
{
    setScanInProgress(false);
    setStatusString(tr("Discovered devices"));
    m_discoveryAgent->stop();
}

void AvailableDevicesModel::connectToDevice(const QString &mac)
{
    if (m_discoveryAgent->isActive()) {
        // deviceScanCancelled will call this function back
        m_deviceToBeOpenedOnDiscoverCancelled = mac;
        cancelDiscovery();
    } else {
        int deviceIndex = -1, i = 0;
        for (const auto &device : m_devices) {
            if (device.btInfo().address().toString() == mac) {
                deviceIndex = i;
                break;
            }
            i++;
        }

        if (deviceIndex == -1)
            return;
        if (m_currentDevice) {
            delete m_currentDevice;
            m_currentDevice = nullptr;
        }
        connectToDeviceAt(deviceIndex);
    }
}

void AvailableDevicesModel::connectToDeviceAt(int deviceIndex)
{
    switch (m_devices.at(deviceIndex).type) {
    case Brandbase:
    case Bburago:
        BleRcCar *car = nullptr;
        if (m_devices.at(deviceIndex).type == Brandbase)
            car = new BrandbaseRcCar(m_devices.at(deviceIndex).btInfo(), this);
        else
            car = new BburagoRcCar(m_devices.at(deviceIndex).btInfo(), this);
        m_currentDevice = car;
        connect(m_currentDevice, &AbstractRcCar::connectionStateChanged,
                this, &AvailableDevicesModel::currentDeviceConnectionStateChangedSlot);
        connect(m_currentDevice, &AbstractRcCar::connectionStateStringChanged,
                this, &AvailableDevicesModel::currentDeviceConnectionStateStringChangedSlot);
        car->connectToDevice();
        break;
    }
}

AvailableDevicesModel::DetectedDevice::DetectedDevice(const QBluetoothDeviceInfo &btInfo, DeviceType dtype) :
    type(dtype)
{
    m_btInfo = btInfo;
    m_alias = Settings::instance()->alias(m_btInfo.address().toString());
    if (m_alias.isEmpty())
        m_alias = m_btInfo.address().toString();
    m_autoConnect = Settings::instance()->autoConnect(m_btInfo.address().toString());
}

QString AvailableDevicesModel::DetectedDevice::name() const
{
    return m_btInfo.name();
}

QString AvailableDevicesModel::DetectedDevice::alias() const
{
    return m_alias;
}

void AvailableDevicesModel::DetectedDevice::setAlias(const QString &newAlias)
{
    if (m_alias != newAlias) {
        Settings::instance()->setAlias(m_btInfo.address().toString(), newAlias);
    }
}

const QBluetoothDeviceInfo &AvailableDevicesModel::DetectedDevice::btInfo() const
{
    return m_btInfo;
}

bool AvailableDevicesModel::DetectedDevice::autoConnect() const
{
    return m_autoConnect;
}

void AvailableDevicesModel::DetectedDevice::setAutoConnect(bool newAutoConnect)
{
    if (newAutoConnect != m_autoConnect) {
        Settings::instance()->setAutoConnect(m_btInfo.address().toString(), newAutoConnect);
        m_autoConnect = newAutoConnect;
    }
}

