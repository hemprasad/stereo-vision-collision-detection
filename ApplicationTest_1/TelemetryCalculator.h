// 
// File:   TelemetryCalculator.h
// Author: nick
//
// Created on 10 March 2008, 12:23
//

#ifndef _TELEMETRYCALCULATOR_H
#define	_TELEMETRYCALCULATOR_H

#include "TelemetryData.h"

enum ProcessStates
{
    START,
    VEL_CALC,
    ACC_CALC
};

//enum {INITIAL = 0; FINAL = 1};



class TelemetryCalculator
{
public:
    
    TelemetryCalculator();
    ~TelemetryCalculator();
    
    double getTimeToImpact();
    void calculate(double timeDelta, double dist);
    
    void setTimeStep(double time);
    void setDistance(double dist);
    
    TelemetryData getTelemetry();
    
    void reset();
    
    

private:
   
    //should eventually replace by telemetry data types
    
    //in seconds
    double m_time;
    
    //in metres
    double m_initDist;
    double m_fnlDist;
    
    //in metres per second
    double m_initVel;
    double m_fnlVel;
    
    //in metres per second ^ 2
    double m_acc;
    
    ProcessStates m_processState;
    
    void calculateVelocity();
    void calculateAcceleration();
    
};

#endif	/* _TELEMETRYCALCULATOR_H */

