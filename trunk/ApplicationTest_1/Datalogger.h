// 
// File:   Datalogger.h
// Author: nick
//
// Created on 17 February 2008, 13:19
//

#ifndef _DATALOGGER_H
#define	_DATALOGGER_H


#include "dbxml/DbXml.hpp"
#include "DataloggerRecord.h"

using namespace DbXml;

class Datalogger
{
public:
    
    Datalogger();
    ~Datalogger();
    
    void addRecord(DataloggerRecord rec);
    void createNewLog(string logName);
    void eraseLogfile();
    void openLog();
    void closeLog();
    
    
    
    string retrieveLog();
    
    static void setEnvHome(string homeDir);
    
private:
    
    bool m_isOpen;
    
    //maybe make this static so all objects use the same environment
    XmlManager* m_manager;
    XmlContainer m_container;
    
    void openLogfile(string filename);
    
    
    //static const string envHome;
    
    static string envHome;
    static string databaseName;
};

#endif	/* _DATALOGGER_H */

