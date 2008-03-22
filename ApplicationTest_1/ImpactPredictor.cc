#include "ImpactPredictor.h"
#include <math.h>
#include <iostream>


ImpactPredictor::ImpactPredictor()
{
    
}

ImpactPredictor::~ImpactPredictor()
{
    
}

//basic calculations based on newtonian physics
double ImpactPredictor::getTimeToImpact(TelemetryData data)
{
    double determinant = 0;
    double posImpactTime = 0;
    double negImpactTime = 0;
    double greaterVal = 0;
    double a = data.getAcceleration();
    double b = data.getVelocity();
    double c = data.getDistance();
    
    //if acceleration is 0 it is no longer a 2nd order polynomial.
    if (a == 0)
    {
        //catch divide by 0 error
        if (b == 0 || c == 0)
        {
            return -1;
        }
        else
        {
            return c / b;
        }
    }
    
    //caluate the determinant to determine if the collision is possible 
    determinant = pow(b, 2) - (4 * a * c);
    
    if (determinant < 0)
    {
        return -10; //temporary until I thinkl of something better
    }
    
    posImpactTime = (-b + sqrt(determinant)) / (2 * a);
    negImpactTime = (-b - sqrt(determinant)) / (2 * a);
    
    //std::cout << "+iveImpactTime:" << posImpactTime << " -tiveImpactTime:" << negImpactTime << std::endl;
    
    if (posImpactTime >= negImpactTime)
    {
       greaterVal = posImpactTime; 
    }
    else
    {
        greaterVal = negImpactTime;
    }
    
    if (greaterVal >= 0)
    {
        return greaterVal;
    }
    else
    {
        return -20;
    }
    
    
}

