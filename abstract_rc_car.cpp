#include "abstract_rc_car.h"

AbstractRC_Car::AbstractRC_Car(QObject *parent) : QObject(parent)
{

}

qreal AbstractRC_Car::batteryPercentage()
{
    return 0.0;
}

void AbstractRC_Car::setConnectionState(AbstractRC_Car::ConnectionState state)
{
    if (state != m_connectionState) {
        auto oldState = m_connectionState;
        m_connectionState = state;
        emit connectionStateChanged(oldState, m_connectionState);
    }
}
