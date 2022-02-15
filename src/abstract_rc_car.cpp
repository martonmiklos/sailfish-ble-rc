#include "abstract_rc_car.h"

#include "settings.h"

#include <QDebug>

AbstractRcCar::AbstractRcCar(QObject *parent) : QObject(parent)
{

}

void AbstractRcCar::setThrottle(float throttle)
{
    static float lastThrottle = 0.0;

    if (qFuzzyIsNull(lastThrottle) && !qFuzzyIsNull(throttle)) {
        m_runTimer.restart();
    } else if (!qFuzzyIsNull(lastThrottle) && qFuzzyIsNull(throttle)) {
        m_runTimeIn_ms += m_runTimer.elapsed();
    }
}

qreal AbstractRcCar::batteryPercentage()
{
    return 0.0;
}

void AbstractRcCar::setConnectionState(AbstractRcCar::ConnectionState newState)
{
    if (newState != m_connectionState) {
        auto oldState = m_connectionState;
        m_connectionState = newState;
        qWarning() << newState;
        if (oldState != AbstractRcCar::Connected && newState == AbstractRcCar::Connected) {
            // got connected
            m_connectionTimeTimer.restart();
        } else if (oldState == AbstractRcCar::Connected && newState != AbstractRcCar::Connected) {
            // got disconnected
            Settings::instance()->addConnectTime(identifier(), m_connectionTimeTimer.elapsed() / 1000.0);
            Settings::instance()->addRunTime(identifier(), m_runTimeIn_ms);
        }
        emit connectionStateChanged(oldState, m_connectionState);
    }
}
