// 
// File:   TelemetryData.h
// Author: nick
//
// Created on 10 March 2008, 13:24
//

#ifndef _TELEMETRYDATA_H
#define	_TELEMETRYDATA_H

class TelemetryData
{
public:
    TelemetryData();
    ~TelemetryData();
    
    double getDistance();
    void setDistance(double dist);
    
    double getVelocity();
    void setVelocity(double vel);
    
    double getAcceleration();
    void setAcceleration(double acc);
    
private:
    
    double m_distance;
    double m_velocity;
    double m_acceleration;
    
};


#endif	/* _TELEMETRYDATA_H */

