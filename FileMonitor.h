//
//  FileMonitor.h
//  Configuration
//
//  Created by Kabir Sohel on 20/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef __Configuration__FileMonitor__
#define __Configuration__FileMonitor__

#include "CommonHeader.h"
#include "Utility.h"
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
class FileMonitor;
class FileMonitorEventListener;
typedef enum{
    FILE_CHANGED,
    FILE_ADDED,
    FILE_DELETED,
    FIEL_PERMISSION_RESTRICTED,
}FileChangeEventType;


class FileMonitorEvent
{
public:
    FileChangeEventType eventType;
    std::string fileName;
    FileMonitorEventListener* eventListener;
    FileMonitorEvent(){}
    FileMonitorEvent(std::string fileName, FileChangeEventType eventType, FileMonitorEventListener *listener){
        this->eventType = eventType;
        this->fileName = fileName;
        this->eventListener = listener;
    }
};

class FileMonitorEventListener
{
public:
    virtual void fileChanged(FileMonitorEvent) = 0;
    FileMonitorEventListener(){}
    virtual ~FileMonitorEventListener(){}
};


class FileMonitor
{
private:
    std::list <FileMonitorEventListener*> listeners;
    static FileMonitor *instance;
    
    struct File
    {
        File(std::string _file, long long _intervalInMSecond, bool _monitoring)
        {
            fileName = _file;
            monitoring = _monitoring;
            intervalInMSecond = _intervalInMSecond;
        }
        std::string fileName;
        bool monitoring;
        long long intervalInMSecond;
        long long lastTimeStamp;
        struct stat st;
        Timer timer;
        pthread_t pthread;
        int threadId;
        FileMonitorEvent event;
    };
    
    vector <File> files;
    
    
   
    
private:
    
    static void *notifyListeners(void *param);
    int getFileLocation(std::string fileName);
public:
    
    FileMonitor();
    virtual ~FileMonitor();
    //virtual void startFileMonitor() = 0;
    
    static void *fileMonitorLoop(void *param);
    static FileMonitor* getInstance();
    
    bool isFileChanged(int fileLocation);
    
    bool addFile(std::string fileName, long long mSec = 100, bool dontStartNow = false);
    string addFile(int numberOfParam, ...);
    bool isFileAdded(std::string fileName);
    void removeFile(std::string fileName);
    
    void addListener(FileMonitorEventListener* listener);
    void removeListener(FileMonitorEventListener* listener);
    bool isListenerAdded(FileMonitorEventListener* listener);
    void removeAllListeners();
    
    void stopMonitoring(std::string fileName);
    bool startMonitoring(std::string fileName);
    bool isBeingMonitored(std::string fileName);
    bool canMonitor(std::string fileName);
    
    void resetFileMonitorInterval(std::string fileName, long long mSec);
    long long getFileMonitorInterval(std::string fileName);
};







#endif /* defined(__Configuration__fileMonitor__) */
