// 
// File:   ImpactPredictor.h
// Author: nick
//
// Created on 10 March 2008, 14:12
//

#ifndef _IMPACTPREDICTOR_H
#define	_IMPACTPREDICTOR_H

#include "TelemetryData.h"


class ImpactPredictor
{
public:
    
    ImpactPredictor();
    ~ImpactPredictor();
    
    double getTimeToImpact(TelemetryData data);
    
};

#endif	/* _IMPACTPREDICTOR_H */

