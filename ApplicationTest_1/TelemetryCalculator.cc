#include "TelemetryCalculator.h"


TelemetryCalculator::TelemetryCalculator()
{
    reset();
}

TelemetryCalculator::~TelemetryCalculator()
{
    
}

double TelemetryCalculator::getTimeToImpact()
{
    return 0;
}

    
void TelemetryCalculator::setTimeStep(double time)
{
    
}


void TelemetryCalculator::setDistance(double dist)
{
    
}


void TelemetryCalculator::calculate(double timeDelta, double dist)
{
    m_time = timeDelta;
    
    //previous final calcs become the new initials
    
    m_initDist = m_fnlDist;
    m_initVel = m_fnlVel;
    
    m_fnlDist = dist;
    
    switch (m_processState)
    {
        case START:
            
            m_initDist = dist;
            m_processState = VEL_CALC;
            break;
            
        case VEL_CALC:
            
            //m_fnlDist = dist;
            calculateVelocity();
            
            m_processState = ACC_CALC;
            break;
            
        case ACC_CALC:
            
            calculateAcceleration();
            break;
            
        default:
            
            break;
                    
    }
    
    
}

void TelemetryCalculator::calculateVelocity()
{   
    m_fnlVel = (m_fnlDist - m_initDist) / m_time; 
    
    //m_initDist = m_fnlDist;
}

void TelemetryCalculator::calculateAcceleration()
{
    calculateVelocity();
    
    m_acc = (m_fnlVel - m_initVel) / m_time;
    
    //m_initVel = m_fnlVel;

}

TelemetryData TelemetryCalculator::getTelemetry()
{
    if (m_processState == ACC_CALC)
    {
        TelemetryData data;
        
        data.setDistance(m_fnlDist);
        data.setVelocity(m_fnlVel);
        data.setAcceleration(m_acc);
        
        return data;
    }
    else
    {
        TelemetryData dt;
        
        return dt;
        //no data yet
        //throw
    }
}

void TelemetryCalculator::reset()
{
    m_time = 0;
    m_initDist = 0;
    m_fnlDist = 0;
    m_initVel = 0;
    m_fnlVel = 0;
    m_acc = 0;
    m_processState = START;
}
