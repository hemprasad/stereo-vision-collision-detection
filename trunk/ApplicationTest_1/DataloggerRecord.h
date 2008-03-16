// 
// File:   DataloggerRecord.h
// Author: nick
//
// Created on 16 February 2008, 17:33
//

#ifndef _DATALOGGERRECORD_H
#define	_DATALOGGERRECORD_H

#include <string>

using std::string;

class DataloggerRecord
{
public:
    
    DataloggerRecord();
    
    string serialiseToXML();
    string getHeader();
    string getBody();
    
    void setValues(int time, double range, double vel, double acc, double estImpact);
    
private:
    
    int m_time;
    double m_range;
    double m_vel;
    double m_acc;
    double m_estImpact;
};


#endif	/* _DATALOGGERRECORD_H */

