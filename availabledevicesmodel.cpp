#include "availabledevicesmodel.h"

#include "shell_rc_car.h"

AvailableDevicesModel::AvailableDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_statusString = tr("Start discovery from the pulley menu");
    //! [les-devicediscovery-1]
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    //discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(deviceDiscovered(const QBluetoothDeviceInfo&)));
    connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
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
    }
    return QVariant();
}

void AvailableDevicesModel::detectDevices()
{
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
    if (Shell_RC_Car::isDevice(info)) {
        DetectedDevice d;
        d.info = info;
        d.typeName = tr("Shell Bluetooth RC car");
        d.imagePath = Shell_RC_Car::imagePath();
        d.type = Shell;
        beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
        m_devices.append(d);
        endInsertRows();
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

void AvailableDevicesModel::currentDeviceConnectionStateChangedSlot(AbstractRC_Car::ConnectionState oldState, AbstractRC_Car::ConnectionState newState)
{
    Q_UNUSED(oldState)
    if (m_currentDevice) {
        if (newState == AbstractRC_Car::Disconnected) {
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

AbstractRC_Car *AvailableDevicesModel::currentDevice() const
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

bool AvailableDevicesModel::scanInProgress() const
{
    return m_scanInProgress;
}

QHash<int, QByteArray> AvailableDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeName] = QByteArrayLiteral("TypeName");
    roles[ImagePath] = QByteArrayLiteral("ImagePath");
    roles[Name] = QByteArrayLiteral("Name");
    roles[Index] = QByteArrayLiteral("Index");
    return roles;
}

void AvailableDevicesModel::connectToDevice(int deviceIndex)
{
    if (deviceIndex < m_devices.count()) {
        if (m_currentDevice) {
            delete m_currentDevice;
            m_currentDevice = nullptr;
        }
        switch (m_devices.at(deviceIndex).type) {
        case Shell:
            auto shell = new Shell_RC_Car(this);
            shell->setDevInfo(m_devices.at(deviceIndex).info);
            shell->connectToDevice();
            m_currentDevice = shell;
            connect(m_currentDevice, &AbstractRC_Car::connectionStateChanged,
                    this, &AvailableDevicesModel::currentDeviceConnectionStateChangedSlot);
            connect(m_currentDevice, &AbstractRC_Car::connectionStateStringChanged,
                    this, &AvailableDevicesModel::currentDeviceConnectionStateStringChangedSlot);
            break;
        }
    }
}
