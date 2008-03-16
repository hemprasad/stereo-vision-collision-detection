#include "TelemetryData.h"

TelemetryData::TelemetryData()
{
    m_distance = 0;
    m_velocity = 0;
    m_acceleration = 0;
}

TelemetryData::~TelemetryData()
{
    
    
}

double TelemetryData::getDistance()
{
    return m_distance;
}

void TelemetryData::setDistance(double dist)
{
    m_distance = dist;
}

double TelemetryData::getVelocity()
{
    return m_velocity;
}


void TelemetryData::setVelocity(double vel)
{
    m_velocity = vel;
}

double TelemetryData::getAcceleration()
{
    return m_acceleration;
}

void TelemetryData::setAcceleration(double acc)
{
    m_acceleration = acc;
}





