#pragma once

#include <QObject>
#include <QVariant>

class AbstractRcCar : public QObject
{
    Q_OBJECT
public:
    enum ConnectionState {
        Disconnected,
        Connecting,
        Connected
    };
    Q_ENUM(ConnectionState)

    enum Feature {
        BatteryVoltageQuery,
        TurboMode,
        Lamp
    };
    Q_ENUM(Feature)

    explicit AbstractRcCar(QObject *parent = nullptr);
    /**
     * @brief setThrottle
     * @param throttle -1 to 1 range
     */
    Q_INVOKABLE virtual void setThrottle(float throttle) = 0;

    /**
     * @brief setSteer
     * @param steer
     */
    Q_INVOKABLE virtual void setSteer(float steer) = 0;

    /**
     * @brief batteryVoltage
     * @return If battery voltage query supported it returns
     * the battery voltage in Volts unit. If not supported it returns 0.
     */
    Q_INVOKABLE virtual qreal batteryPercentage();

    Q_INVOKABLE virtual bool isFeatureSupported(Feature feature) const = 0;
    Q_INVOKABLE virtual bool setFeature(Feature feature, const QVariant &value) = 0;
    Q_INVOKABLE virtual QVariant featureValue(Feature feature) const = 0;

    Q_INVOKABLE virtual QString name() const = 0;
    virtual QString connectionStateString() const = 0;

    virtual bool connectToDevice() = 0;
    virtual void disconnectFromDevice() = 0;
signals:
    void batteryPercentageUpdated();
    void connectionStateChanged(ConnectionState oldState, ConnectionState newState);
    void connectionStateStringChanged();

protected:
    ConnectionState m_connectionState = Disconnected;
    void setConnectionState(ConnectionState state);
};

