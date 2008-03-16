#include "Datalogger.h"

/*
 *  Need to update this so I the logfile name is not hard coded.
 *  Also add validations to check that the logfile names that will
 *  be passed in. Check that a file already exists.
 *
 *
 *
 */

#include <sstream>

using std::endl;

string Datalogger::envHome = "/home/nick/NetBeansProjects/ApplicationTest_1/data";
string Datalogger::databaseName = "test2log.bdbxml";

Datalogger::Datalogger()
{
    m_manager = 0;
    m_isOpen = false;
}

Datalogger::~Datalogger()
{
    if (m_manager != 0) delete m_manager;
}

/*void Datalogger::setEnvHome(string homeDir)
{
    Datalogger::s_envHome = homeDir;
}*/


/*
 *NOT USED
 */
void Datalogger::openLogfile(string filename)
{
    XmlContainer container;
    
    if (m_manager->existsContainer(filename))
    {
        container = m_manager->openContainer(filename);
    }
    else
    {
        container = m_manager->createContainer(filename);
    }
    
}

void Datalogger::addRecord(DataloggerRecord rec)
{
    XmlDocument doco;
    XmlQueryContext qContext;
    XmlUpdateContext uContext;
    XmlModify mod;
    XmlQueryExpression select;
    //XmlContainer container;
    /*string filename = "test2log.bdbxml";
    
    if (m_manager->existsContainer(filename))
    {
        container = m_manager->openContainer(filename);
    }
    else
    {
        container = m_manager->createContainer(filename);
    }*/
    
    if (!m_isOpen)
    {
        //@TODO throw exception
    }
    
    //might need to be careful here if the log is new. It might not add
    string updateQuery = "/datalog/logHeader";
    
    try
    {
                        
        uContext = m_manager->createUpdateContext();
        qContext = m_manager->createQueryContext();

        mod = m_manager->createModify(); 
        select = m_manager->prepare(updateQuery, qContext);

        mod.addInsertAfterStep(select, XmlModify::Element, rec.getHeader(), rec.getBody());  

        //@TODO add error checking on the getDoc
        doco = m_container.getDocument("test2log.xml");

        XmlValue modValue(doco);

        mod.execute(modValue, qContext, uContext);
    }
    catch (XmlException e)
    {
        
    }
     
}

void Datalogger::createNewLog(string logName)
{
    std::stringstream doc; // = "<datalog>\n</datalog>";
    
    XmlUpdateContext uContext;
    
    logName = "test2log.xml";
    
    doc << "<datalog>" << endl;
    doc << "<logHeader>" << endl;
    
    doc << "</logHeader>" << endl << "</datalog>";
    
    uContext = m_manager->createUpdateContext();
    
    try
    {
        m_container.putDocument(logName, doc.str(), uContext, 0);
    }
    catch (XmlException e)
    {
        
    }
}

void Datalogger::eraseLogfile()
{
    XmlUpdateContext uContext;
    
    uContext = m_manager->createUpdateContext();
    
    try
    {
        m_container.deleteDocument("test2log.xml", uContext);
    }
    catch (XmlException e)
    {
        
    }
}

void Datalogger::openLog()
{
    //string filename = "test2log.bdbxml";
    u_int32_t envFlags = DB_CREATE | DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN;
    
    DbEnv* env = new DbEnv(0);
    
    try
    {
        env->open(envHome.c_str(), envFlags, 0);
        m_manager = new XmlManager(env, DBXML_ADOPT_DBENV);
        
    if (m_manager->existsContainer(databaseName))
    {
        m_container = m_manager->openContainer(databaseName);
    }
    else
    {
        m_container = m_manager->createContainer(databaseName);
    }
        
        
    }
    catch (XmlException e)
    {
        
    }
}


void Datalogger::closeLog()
{
    try
    {
        if (m_manager != 0) 
        {
            delete m_manager;
            m_manager = 0;
        } 
        
        m_isOpen = false;
    }
    catch (XmlException e)
    {
        
    }
    
}

string Datalogger::retrieveLog()
{
    string docContent;
    //string filename = "test2log.bdbxml";
    XmlDocument doco;
    /*XmlContainer container;
    
    if (m_manager->existsContainer(filename))
    {
        container = m_manager->openContainer(filename);
    }
    else
    {
        container = m_manager->createContainer(filename);
    }*/
    
    doco = m_container.getDocument("test2log.xml");
    
    doco.getContent(docContent);
    
    return docContent;
}

//void dataLoggerTest()
//{
//    string envHome("/home/nick/NetBeansProjects/ApplicationTest_1/data");
//    u_int32_t envFlags = DB_CREATE | DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN;
//    
//    std::stringstream doc;
//    
//    //string filename = "/home/nick/NetBeansProjects/ApplicationTest_1/data/testlog.bdbxml";
//    string filename = "test2log.bdbxml";
//    
//    DbEnv loggerEnv(0);
//    XmlManager* manager = 0;
//    
//    
//    try
//    {
//        loggerEnv.open(envHome.c_str(), envFlags, 0);
//        manager = new XmlManager(&loggerEnv);
//    }
//    catch (DbException e)
//    {
//        
//    }
//    
//    XmlContainer container;
//    
//    if (manager->existsContainer(filename))
//    {
//        container = manager->openContainer(filename);
//    }
//    else
//    {
//        container = manager->createContainer(filename);
//    }
//    
//    
//    //create XML file here
//    
//    XmlUpdateContext uContext;
//    
//    
//    XmlDocument doco;
//    
//    //doco = container.getDocument("test2log.xml");
//    
//    DataloggerRecord rec;
//    
//    /*doc << "<datalog>" << endl;
//    doc << rec.serialiseToXML();
//    doc << "</datalog>" << endl;
//    
//    cout << doc.str() << endl;
//    
//    doco.setContent(doc.str());
//    
//    context = manager->createUpdateContext();
//    
//    container.updateDocument(doco, context);*/
//    
//    uContext = manager->createUpdateContext();
//    string query = "collection('test2log.bdbxml')/datalog";
//    
//    //print result to output
//    XmlQueryContext qContext = manager->createQueryContext();
//    
//    try
//    {
//        //container.putDocument("test2log.xml", doc.str(), context, 0);
//        
//        
//        //run query and display results by iterating through result set
//        
//        /*XmlResults result = manager->query(query, qContext);
//        XmlValue value;
//        
//        cout << "Total:" << result.size() << endl;
//
//        while(result.next(value))
//        {
//            cout << value.asString() << endl;
//        }*/
//    }
//    catch (XmlException e)
//    {
//        cout << "something went wrong here" << endl;
//        cout << e.what() << endl;
//    }
//    
//    
//    
//    //add a node here
//   /* XmlQueryExpression select = manager->prepare("/datalog", qContext);
//    XmlModify mod = manager->createModify();
//    
//    rec.setValues(100, 200, 300, 400, 500);
//    
//    //should change this to addInserrtAfterStep
//    mod.addAppendStep(select, XmlModify::Element, rec.getHeader(), rec.getBody());
//    
//    
//    
//    doco = container.getDocument("test2log.xml");
//    XmlValue modValue(doco);
//    
//    mod.execute(modValue, qContext, uContext);*/
//    
//    
//    //print result to output
//    
//    string docContent;
//    doco = container.getDocument("test2log.xml");
//    
//    cout << doco.getName() << endl;
//    cout << doco.getContent(docContent) << endl;
//    
//    
//    try
//    {
//        //loggerEnv.close(0);
//        
//        delete manager;
//    }
//    catch (DbException e)
//    {
//        
//    }

    
//}
