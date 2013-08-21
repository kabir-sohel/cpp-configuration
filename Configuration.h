//
//  Configuration.h
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef __Configuration__Configuration__
#define __Configuration__Configuration__

#include <iostream>
#include <stdarg.h>
#include "ConfigurationEvent.h"
#include "ConfigurationException.h"
#include "FileMonitor.h"
#include "Utility.h"






/* Class
 *
 * Basic Configuration class that...
 * Holds Config related data structure
 * Expose some public API
 * Allows modificatoin of Config data
 * Reload when file is changed.
 * Fire events when file is Changed.
 */



class Configuration: public FileMonitorEventListener{
  
private:
    
    vector < string > fileNames;
    // Data holds the key value pair
    // each file maps with its own set of data.
    struct Data
    {
        std::string key;
        std::string value;
        Data(std::string _key, std::string _value)
        {
            key = _key;
            value = _value;
        }
        bool operator<(const Data &ob)const
        {
            return  this->key < ob.key;
        }
    };

    std::map < std::string , set < Data > > files;
    
            
    std::list<ConfigurationEventListener *> listeners;
    
    //local functions

private:
    void update(ConfigurationEvent event);
    void fileChanged(const string &fileName);
            
public:
    Configuration();
    virtual ~Configuration();
    
     
protected:
    void addSource(const string &fileName);
    //void addSource(int numberOfParam, ...);
    void removeSource(const string &fileName);
            
            
public:
            
//methods to be implemented by child class
    virtual void loadPropertyConfig(const string &fileName) = 0;
    virtual void loadXMLConfig(const string &fileName) = 0;

//Method implemented for File Monitor
    void fileChanged(FileMonitorEvent event);
            
            
// public APIs
    void addProperty(const string &key, const string &value);
    void addProperty(const string &fileName, const string &key, const string &value);
    void deleteProperty(const string &key);
    void deleteProperty(const string &key, const string &value);
    void deletePropertyFromFile(const string &fileName, const string &key);
    void deletePropertyFromFile(const string &fileName, const string &key, const string &value);
    void deleteAll();
    void deleteAllFromFile();
    void deleteAllFromFile(const string &fileName);
    
    
    bool isEmpty();
    
    void addListener(ConfigurationEventListener* listener);
    void removeListener(ConfigurationEventListener* listener);
    void removeAllListener();
    
    
    
    bool hasData(const string &fileName, const Data &data);
    bool hasKey(const string &key);
    bool hasKeyInFile(const string &fileName, const string &key);
    bool hasValue(const string &key);
    bool hasValueInFile(const string &fileName, const string &key);
    
    bool hasValue(const string &key, const string &value);
    int countKey();
    int countValue();
    int countValue(const string &key);

    
    
    
    
    //get all the keys Configuration has.
    set<string> getKeyAsSet();
    list<string> getKeyAsList();
    vector<string> getKeyAsVector();
    
    
    //get values associated to one key or a set of key
    
    string get(const string &key);
    string getValueFromFile(const string &fileName, const string &key);
    string getValueAsString(const string &key); //returns the first value if multiple
    set <string> getValueAsSetOfString(const string &key);
    list <string> getValueAsListOfString(const string &key);
    vector<string> getValueAsVectorOfString(const string &key);
    
    
    bool getValueAsInt(const string &key,int &intValue);
    vector<int> getValueAsVectorOfInt(const string &key);
    string replaceVariable(const string &value);
};











#endif /* defined(__Configuration__Configuration__) */
