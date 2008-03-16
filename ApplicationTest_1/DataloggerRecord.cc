#include "DataloggerRecord.h"
#include <sstream>


DataloggerRecord::DataloggerRecord()
{
    m_time = 10;
    m_range = 20;
    m_vel = 30;
    m_acc = 40;
    m_estImpact = 50;
}

string DataloggerRecord::serialiseToXML()
{
    std::stringstream xml;
    
    xml << "<record>" << std::endl;
    xml << "<time>" << m_time << "</time>" << std::endl;
    xml << "<range>" << m_range << "</range>" << std::endl;
    xml << "<vel>" << m_vel << "</vel>" << std::endl;
    xml << "<acc>" << m_acc << "</acc>" << std::endl;
    xml << "<estImpact>" << m_estImpact << "</estImpact>" << std::endl;
    xml << "</record>" << std::endl;
    
    return xml.str(); 
}

void DataloggerRecord::setValues(int time, double range, double vel, double acc, double estImpact)
{
    m_time = time;
    m_range = range;
    m_vel = vel;
    m_acc = acc;
    m_estImpact = estImpact;
}

string DataloggerRecord::getHeader()
{
    std::stringstream xml;
    
    xml << "record";
    
    return xml.str();
}

string DataloggerRecord::getBody()
{
    std::stringstream xml;
    
    xml << "<time>" << m_time << "</time>";// << std::endl;
    xml << "<range>" << m_range << "</range>";// << std::endl;
    xml << "<vel>" << m_vel << "</vel>";// << std::endl;
    xml << "<acc>" << m_acc << "</acc>";// << std::endl;
    xml << "<estImpact>" << m_estImpact << "</estImpact>";// << std::endl;
    
    return xml.str();
}
