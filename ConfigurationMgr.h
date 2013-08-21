//
//  ConfigurationMgr.h
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef __Configuration__ConfigurationMgr__
#define __Configuration__ConfigurationMgr__

#include "Configuration.h"
#include "FileMonitor.h"
#include <fstream>
#include <pthread.h>



/*
 //removed this classed to have className::getInstance()-> approach.
 
class PropertyConfiguration : public Configuration
{
private:
    void loadConfigurationData(const string &fileName);
private:
    void parseRawInput(std::vector<std::string> &data, const string &fileName);
};







class XMLConfiguration : public Configuration
{
public:
    void loadConfigurationData(const string &fileName){}
};



class DBConfiguration : public Configuration
{
public:
    void loadConfigurationData(const string &dbName){}
};

*/






class ConfigurationMgr : public Configuration
{
private:
    static ConfigurationMgr *instance;
    pthread_t fileMonitorThread;
    int fileMonitorThreadId;
    FileMonitor *fileMonitor;
    
private:
    //Constructor
    ConfigurationMgr();
    ~ConfigurationMgr();
    

public:
    
    static ConfigurationMgr* Instance();
    
    void startConfigurationMgr();
    
    void addPropertyFile(int numberOfFiles, ...);
    void addPropertyFile(const string &fileName);
    
    
    void removePropertyFile(const string &fileName);
    
    void parseRawInput(std::vector<std::string> &data, const string &fileName);

    //overriden methods from Configuration
    
    void loadPropertyConfig(const string &fileName);
    void loadXMLConfig(const string &fileName);
    
};
















#endif /* defined(__Configuration__ConfigurationMgr__) */
