//
//  utility.h
//  Configuration
//
//  Created by Kabir Sohel on 14/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef Configuration_Utility_h
#define Configuration_Utility_h
#include "CommonHeader.h"


class Utility
{
public:
    static void removeLeadingAndTrailingSpaces(std::string &data);
    static bool isComment(string &line);
    
    static void removeComment(string &line);
    static bool getInteger(string &str, int &intValue);
    static int findSubStringFromPosition(int pos,string &str, string &pat);
};

class Timer
{
private:
    timeval startTime;
public:
    void start()
    {
        gettimeofday(&startTime, NULL);
    }
    long long getTime()
    {
        timeval endTime;
        gettimeofday(&endTime, NULL);
        double seconds = endTime.tv_sec - startTime.tv_sec;
        double useconds = endTime.tv_usec - startTime.tv_usec;
        return 1000 * (long long) (seconds + useconds/1000000.);
    }
    void reset()
    {
        start();
    }
};

#endif
