#include "availabledevicesmodel.h"

#include "shell_rc_car.h"

AvailableDevicesModel::AvailableDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{
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
    beginResetModel();
    endResetModel();
    m_devices.clear();
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
        qWarning() << m_devices.count() << m_devices.count() + 1;
        beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count() + 1);
        m_devices.append(d);
        endInsertRows();
    }
}

void AvailableDevicesModel::deviceScanFinished()
{
    setScanInProgress(false);
}

void AvailableDevicesModel::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    setScanInProgress(false);
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

QObject *AvailableDevicesModel::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static AvailableDevicesModel instance;
    return &instance;
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

void AvailableDevicesModel::useDevice(int deviceIndex)
{
    if (deviceIndex < m_devices.count()) {
        if (m_currentDevice) {
            delete m_currentDevice;
        }
        switch (m_devices.at(deviceIndex).type) {
        case Shell:
            auto shell = new Shell_RC_Car(this);
            shell->connectToDevice(m_devices.at(deviceIndex).info);
            m_currentDevice = shell;
            break;
        }
    }
}
