//
//  fileMonitor.cpp
//  Configuration
//
//  Created by Kabir Sohel on 20/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#include "FileMonitor.h"

FileMonitor* FileMonitor::instance = NULL;

FileMonitor::FileMonitor()
{
    
}
FileMonitor::~FileMonitor()
{
    delete instance;
}



void* FileMonitor:: notifyListeners(void *param)
{
    FileMonitorEvent *event = reinterpret_cast<FileMonitorEvent*>(param);
    event->eventListener->fileChanged(*event);
    
    pthread_exit(NULL);
}

void* FileMonitor:: fileMonitorLoop(void *param)
{
    instance = getInstance();
    while(NULL != instance)
    {
        for(int i = 0; i < instance->files.size(); ++i)
        {
            if(false == instance->files[i].monitoring)continue;
            //cout << "time = " << instance->files[i].timer.getTime() << " interval should be  = " << instance->files[i].intervalInMSecond << endl;
            if(instance->files[i].timer.getTime() >= instance->files[i].intervalInMSecond)
            {
                if(true == instance->isFileChanged(i)){
                    std::list<FileMonitorEventListener*>::iterator it = instance->listeners.begin();
                    for(; it != instance->listeners.end(); ++it)
                    {
                        //FileMonitorEvent event(instance->files[i].fileName, FILE_CHANGED, *it);
                        instance->files[i].event.eventType = FILE_CHANGED;
                        instance->files[i].event.eventListener = *it;
                        pthread_create(&instance->files[i].pthread, NULL, FileMonitor::notifyListeners,(void*) &instance->files[i].event);
                        instance->files[i].timer.reset();
                    }
                }
            }
            usleep(50000);
        }
    }
    pthread_exit(NULL);
}
bool FileMonitor::isFileChanged(int fileLocation)
{
    File &file = files[fileLocation];
    
    int errNo = 0;
    errNo = stat(file.fileName.c_str(), &file.st);
    if(errNo != 0)return false;
    
    long long currentTime = file.st.st_mtime;
    
    //cout << "Current Time = " << currentTime << endl << "last Time = " << file.lastTimeStamp << endl;
    
    if(currentTime > file.lastTimeStamp)
    {
        file.lastTimeStamp = currentTime;
        return true;
    }
    return false;
}

FileMonitor* FileMonitor:: getInstance()
{
    if(NULL == instance)
    {
        instance = new FileMonitor();
    }
    return instance;
}

int FileMonitor::getFileLocation(std::string fileName)
{
    for(int i = 0; i < files.size(); ++i)
    {
        if(fileName == files[i].fileName)
        {
            return i;
        }
    }
    return -1;
}

bool FileMonitor:: addFile(std::string fileName, long long mSec, bool dontStartNow)
{
    
    //cout << " time ===== " << mSec << endl;
    int position = getFileLocation(fileName);
    if(0 <= position)
    {
        files[position].monitoring = !dontStartNow;
        files[position].intervalInMSecond = mSec;
        return true;
    }
    
    File file(fileName, mSec, !dontStartNow);
    file.timer.start();
    int error = stat(file.fileName.c_str(), &file.st);
    if(0 != error)return false;
    file.lastTimeStamp = file.st.st_mtime;
    file.event.fileName = fileName;
    files.push_back(file);
    
    //cout << file.fileName << " is being monitored  " << endl;
    return true;
}

string FileMonitor::addFile(int numberOfFiles, ...)
{
    string msg = "";
    va_list fileNames;
    va_start(fileNames, numberOfFiles);
    for(int i = 0; i < numberOfFiles; ++i)
    {
        string fileName = va_arg(fileNames, char*);
        if(false == addFile(fileName))
        {
            msg += fileName + " can't be added.\n";
        }
    }
    return msg;
}

bool FileMonitor:: isFileAdded(std::string fileName)
{
    return getFileLocation(fileName) >= 0;
}

void FileMonitor:: removeFile(std::string fileName)
{
    int pos = getFileLocation(fileName);
    if(pos >= 0)files.erase(files.begin() + pos);
}

void FileMonitor:: addListener(FileMonitorEventListener* listener)
{
    listeners.push_back(listener);
}

void FileMonitor:: removeListener(FileMonitorEventListener* listener)
{
    if(isListenerAdded(listener))listeners.remove(listener);
}

bool FileMonitor:: isListenerAdded(FileMonitorEventListener* listener)
{
    std::list<FileMonitorEventListener*> ::iterator it = listeners.begin();
    for(; it != listeners.end(); ++it)
    {
        if(*it == listener)return true;
    }
    return false;
}

void FileMonitor:: removeAllListeners()
{
    std::list<FileMonitorEventListener*> ::iterator it = listeners.begin();
    while(it != listeners.end())
    {
        delete *it;
        *it = NULL;
        listeners.erase(it);
    }
}

void FileMonitor:: stopMonitoring(std::string fileName)
{
    int pos = getFileLocation(fileName);
    if(pos >= 0) files[pos].monitoring = false;
}

bool FileMonitor:: startMonitoring(std::string fileName)
{
    int pos = getFileLocation(fileName);
    if(pos >= 0 && canMonitor(fileName)){
        files[pos].monitoring = true;
        return true;
    }
    return false;
}

bool FileMonitor::canMonitor(std::string fileName)
{
    int loc = getFileLocation(fileName);
    File &file = files[loc];
    int error = stat(file.fileName.c_str(), &file.st);
    if(0 != error)return false;
    return true;
}

bool FileMonitor:: isBeingMonitored(std::string fileName)
{
    int pos = getFileLocation(fileName);
    return pos >= 0 && files[pos].monitoring;
}

void FileMonitor:: resetFileMonitorInterval(std::string fileName, long long mSec)
{
    int pos = getFileLocation(fileName);
    if(pos >= 0)files[pos].intervalInMSecond = mSec;
}

long long FileMonitor:: getFileMonitorInterval(std::string fileName)
{
    int pos = getFileLocation(fileName);
    if(pos >= 0) return files[pos].intervalInMSecond;
    return 0;
}