//
//  utility.cpp
//  Configuration
//
//  Created by Kabir Sohel on 14/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#include "Utility.h"


void Utility::removeLeadingAndTrailingSpaces(std::string &data)
{
    int length = (int) data.size();
    int leadingSpaces = 0;
    int trailingSpace = 0;
    for(int i = 0; i < length && ' ' == data[i]; ++i, ++leadingSpaces);
    for(int i = length - 1; 0 <= i && ' ' == data[i]; --i, ++trailingSpace);
    if(leadingSpaces + trailingSpace >= length)
    {
        data = "";
    }
    else
    {
        int actualLength = length - (leadingSpaces + trailingSpace);
        data = data.substr(leadingSpaces,actualLength);
    }
}


bool Utility::isComment(string &line)
{
    for(int i = 0; i < line.size(); ++i)
    {
        if(' ' == line[i] || '\t' == line[i])continue;
        else if(line[i] == '#') return true;
        else return false;
    }
    return false;
}




void Utility::removeComment(string &line)
{
    for(int i = 0; i < line.size(); ++i)
    {
        if('#' == line[i])
        {
            line = line.substr(0,i);
            return;
        }
    }
}


bool Utility::getInteger(string &str, int &intValue)
{
    for(int i = 0; i < str.size(); ++i)
    {
        if(str[i] < '0' || str[i] > '9')return false;
    }
    intValue = atoi(str.c_str());
    return true;
}



int Utility::findSubStringFromPosition(int pos,string &str, string &pat)
{
    for(int i = pos; i <= str.size() - pat.size(); ++i)
    {
        if(str.substr(i,pat.size()) == pat)return i;
    }
    return (int)str.size();
}


