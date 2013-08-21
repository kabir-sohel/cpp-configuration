//
//  Configuration.cpp
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#include "Configuration.h"










//constructor
Configuration::Configuration()
{
    //fileNumber = 0;
    set < Data > st;
    files[""] = st;
    fileNames.push_back("");
}
//virtual desctructor

Configuration::~Configuration()
{
    removeAllListener();
}


bool Configuration::hasData(const string &fileName, const Data &data)
{
    set < Data > &st = files[fileName];
    return st.find(data) != st.end();
}



void Configuration::addProperty(const string &key, const string &value)
{
    Data data(key,value);
    if(true == hasData("", data))
    {
        return;
    }
    files[""].insert(data);
}

void Configuration::addProperty(const string &fileName, const string &key, const string &value)
{
    Data data(key,value);
    if(files.find(fileName) == files.end())
    {
        //cout << "call addsource first " << endl;
        return;
    }
    
    if(true == hasData(fileName, data))
    {
        return;
    }
    files[fileName].insert(data);
}






void Configuration::deleteProperty(const string &key)
{
    for(int i = 0; i < fileNames.size(); ++i)
    {
        deletePropertyFromFile(fileNames[i], key);
    }
}

void Configuration::deletePropertyFromFile(const string &fileName, const string &key)
{
    set < Data > &st = files[fileName];
    
    set < Data > ::iterator it = st.begin();
    for(; it != st.end();)
    {
        if(it->key == key)
        {
            st.erase(it);
        }
        else ++it;
    }
}

void Configuration::deletePropertyFromFile(const string &fileName, const string &key, const string &value)
{
    set < Data > &st = files[fileName];
    
    set < Data > ::iterator it = st.begin();
    for(; it != st.end();)
    {
        if(it->key == key && it->value == value)
        {
            st.erase(it);
        }
        else ++it;
    }
}

void Configuration::deleteProperty(const string &key, const string &value)
{
    for(int i = 0; i < fileNames.size(); ++i)
    {
        deletePropertyFromFile(fileNames[i], key, value);
    }
}

void Configuration::deleteAll()
{
    files.clear();
}

void Configuration::deleteAllFromFile()
{
    set < Data > defaultData = files[""];
    files.clear();
    files[""] = defaultData;
}
void Configuration::deleteAllFromFile(const string &fileName)
{
    files.erase(fileName);
}


bool Configuration::isEmpty()
{
    for(map<string, set < Data> > ::iterator it = files.begin(); it != files.end(); ++it)
    {
        if(it->second.size() > 0) return true;
    }
    return false;
}


//Local functions + public API
void Configuration::addSource(const string &fileName)
{
    
    if(files.find(fileName) != files.end())
    {
        //duplicate call, ignore.
        return;
    }
    //++fileNumber;
    fileNames.push_back(fileName);
    set < Data > st;
    files[fileName] = st;
    
    //cout << "before Loading\n";
    loadPropertyConfig(fileName);
}





void Configuration::removeSource(const string &fileName)
{
    for(int i = 0; i < fileNames.size(); ++i)
    {
        if(fileNames[i] == fileName)
        {
            fileNames.erase(fileNames.begin() + i);
            deleteAllFromFile(fileName);
            return;
        }
    }
}


//methods regarding listener
void Configuration::addListener(ConfigurationEventListener* listener)
{
    this->listeners.push_back(listener);
}

void Configuration::removeListener(ConfigurationEventListener* listener)
{
    list<ConfigurationEventListener*>::iterator it = listeners.begin();
    for(; it != listeners.end(); ++it)
    {
        if(*it == listener)
        {
            delete *it;
            listeners.erase(it);
            break;
        }
    }
}

void Configuration::removeAllListener()
{
    list<ConfigurationEventListener*>::iterator it = listeners.begin();
    while(it != listeners.end())
    {
        delete *it;
        listeners.erase(it);
        it = listeners.begin();
    }
}


//private methods



void Configuration::update(ConfigurationEvent event)
{
    list<ConfigurationEventListener*>::iterator it = listeners.begin();
    
    for(; it != listeners.end(); ++it)
    {
        ConfigurationEventListener* listener = *it;
        listener->configurationChanged(event);
    }
}







//Methods implemented from File Changed Event Listener

void Configuration::fileChanged(FileMonitorEvent event)
{
    fileChanged(event.fileName);
    
}



//methods regarding fileSystem



//this function will be called by any file Monitor if the file is changed. it will load file and invoke the update method when there is data change.
void Configuration::fileChanged(const string &fileName)
{
    if(files.find(fileName) != files.end())
    {
        set < Data > prev = files[fileName];
        files[fileName].clear();
        
        loadPropertyConfig(fileName);
        set < Data > &curr = files[fileName];

        set < Data > :: iterator prevIt = prev.begin();
 
        for(; prevIt != prev.end(); ++ prevIt)
        {
            if(false == hasKeyInFile(fileName, prevIt->key))
            {
                //deletion occurs
                ConfigurationEvent event(CONFIGURATION_DELETED, fileName, prevIt->key, prevIt->value);
                update(event);
                
                //config is deleted from file, so treat is like default configuratoin.
                addProperty("", prevIt->key, prevIt->value);
            }
            else if(prevIt->value != getValueFromFile(fileName, prevIt->key))
            {
                //modification occurs
                ConfigurationEvent event(CONFIGURATION_MODIFIED, fileName, prevIt->key, prevIt->value);
                update(event);
            }
        }
        
        set < Data > :: iterator currIt = curr.begin();
        for(; currIt != curr.end(); ++currIt)
        {
            if(prev.find(*currIt) == prev.end())
            {
                //addition occurs
                ConfigurationEvent event(CONFIGURATION_ADDED, fileName, currIt->key, currIt->value);
                update(event);
            }
        }
        
    }
}



//misc public API
int Configuration::countKey()
{
    set < string > keys = getKeyAsSet();
    return (int) keys.size();
}

int Configuration::countValue()
{
    int numberOfValues = 0;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        numberOfValues += files[fileNames[i]].size();
    }
    return numberOfValues;
}
int Configuration::countValue(const string &key)
{
    int numberOfValues = 0;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        numberOfValues += hasKeyInFile(fileNames[i], key);
    }
    return numberOfValues;
}


bool Configuration::hasKey(const string &key)
{
    for(int i = 0; i < fileNames.size(); ++i)
    {
        if(true == hasKeyInFile(fileNames[i], key))
        {
            return true;
        }
    }
    return false;
}

bool Configuration::hasKeyInFile(const string &fileName, const string &key)
{
    for(set < Data > ::iterator it = files[fileName].begin(); it != files[fileName].end(); ++it)
    {
        if(it->key == key)return true;
    }
    return false;
}
           
bool Configuration::hasValue(const string &key)
{
    for(int i = 0; i < fileNames.size(); ++i)
    {
        if(true == hasValueInFile(fileNames[i],key))
        {
            return true;
        }
    }
    return false;
}
           
bool Configuration::hasValueInFile(const string &fileName, const string &key)
{
    for(set < Data > ::iterator it = files[fileName].begin(); it != files[fileName].end(); ++it)
    {
        if(it->key == key && "" != it->value)
        {
            return  true;
        }
    }
    return false;
}

std::set<std::string> Configuration::getKeyAsSet()
{
    set < std::string > keys;
    for(int i = 0; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            keys.insert(it->key);
        }
    }
    return keys;
}


std::list<std::string> Configuration::getKeyAsList()
{
    set < std::string > keys = getKeyAsSet();
    list < std::string > keysAsList;
    for(set < string > ::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        keysAsList.push_back(*it);
    }
    return keysAsList;
}


std::vector<std::string> Configuration::getKeyAsVector()
{
    set < std::string > keys = getKeyAsSet();
    vector < std::string > keysAsVector;
    for(set < string > ::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        keysAsVector.push_back(*it);
    }
    return keysAsVector;
}


std::string Configuration::replaceVariable(const string &_value)
{
    string pat = "$";
    int currentPosition = 0;
    string value = _value;
    while(currentPosition < value.size())
    {
        int loc = Utility::findSubStringFromPosition(currentPosition,value,pat);
        if(0 > loc || value.size() == loc)break;
        
        if(loc && '\\' == value[loc - 1])
        {
            currentPosition = loc + 1;
            continue;
        }
        string temStr = "";
        for(int i = loc + 1; i < value.size(); ++i)
        {
            if(value[i] == ' ' || value[i] == '\t' || value[i] == '$')
            {
                currentPosition = i + 1;
                break;
            }
            else temStr.push_back(value[i]);
            
            if(true == hasKey(temStr))
            {
                string newValue = getValueAsString(temStr);
                value = value.substr(0,loc) + newValue + value.substr(i + 1);
                
                currentPosition = loc + 1;
                break;
            }
        }
    }
    return value;
}

std::string Configuration::get(const string &key)
{
    std:string value = getValueAsString(key);
    if("" == value)return value;
    return replaceVariable(value);
}


string Configuration::getValueFromFile(const string &fileName, const string &key)
{
    set < Data > &st = files[fileName];
    set < Data > :: iterator it = st.begin();
    for(; it != st.end(); ++ it)
    {
        if(it->key == key) return it->value;
    }
    return "";
}

//return the value of the key, the first one if there is multiple value.
std::string Configuration::getValueAsString(const string &key)
{
    std::string value = "";
    bool done = false;
    for(int i = 1; !done; ++i)
    {
        if(i == fileNames.size())
        {
            done = true;
            i = 0;
        }
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        
        for(; it != st.end(); ++it)
        {
            if(it->key == key)
            {
                return it->value;
            }
        }
    }
    return value;
}


std::set <std::string> Configuration::getValueAsSetOfString(const string &key)
{
    std::set<std::string>valueSet;
    for(int i = 1; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            if(it->key == key)
            {
                valueSet.insert(it->value);
            }
        }
    }
    return valueSet;
}



std::list <std::string> Configuration::getValueAsListOfString(const string &key)
{
    std::list<std::string> valueList;
    for(int i = 1; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            if(it->key == key)
            {
                valueList.push_back(it->value);
            }
        }
    }
    return valueList;
}

std::vector<std::string> Configuration::getValueAsVectorOfString(const string &key)
{
    std::vector<std::string> valueVector;
    for(int i = 1; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            if(it->key == key)
            {
                valueVector.push_back(it->value);
            }
        }
    }
    return valueVector;
}

bool Configuration::getValueAsInt(const string &key, int &intValue)
{
    for(int i = 1; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            std::string str = it->value;
            if(true == Utility::getInteger(str, intValue))
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<int> Configuration::getValueAsVectorOfInt(const string &key)
{
    vector <int> valueVector;
    
    for(int i = 1; i < fileNames.size(); ++i)
    {
        set < Data > &st = files[fileNames[i]];
        set < Data > :: iterator it = st.begin();
        for(; it != st.end(); ++it)
        {
            std::string str = it->value;
            int intValue = 0;
            if(true == Utility::getInteger(str, intValue))
            {
                valueVector.push_back(intValue);
            }
        }
    }
    return valueVector;
}
