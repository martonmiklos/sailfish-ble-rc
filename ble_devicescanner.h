#ifndef BLE_DEVICESCANNER_H
#define BLE_DEVICESCANNER_H

#include <QObject>

class BLE_DeviceScanner : public QObject
{
    Q_OBJECT
public:
    explicit BLE_DeviceScanner(QObject *parent = nullptr);
    void detectDevices();
private slots:

signals:

};

#endif // BLE_DEVICESCANNER_H
