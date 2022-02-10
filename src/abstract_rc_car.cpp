#include "abstract_rc_car.h"

#include <QDebug>

AbstractRcCar::AbstractRcCar(QObject *parent) : QObject(parent)
{

}

qreal AbstractRcCar::batteryPercentage()
{
    return 0.0;
}

void AbstractRcCar::setConnectionState(AbstractRcCar::ConnectionState state)
{
    if (state != m_connectionState) {
        auto oldState = m_connectionState;
        m_connectionState = state;
        qWarning() << state;
        emit connectionStateChanged(oldState, m_connectionState);
    }
}
