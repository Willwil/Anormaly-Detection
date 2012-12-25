#include "globalInclude.h"

int frameWidth = 0;
int frameHeight = 0;


bool isVideoFormat(string s)
{
    //printf("s: %s\n", s.c_str());
    string subs;
    int findPos = s.find_last_of('.');
    if(findPos == string::npos)
        return false;
    subs = s.substr(findPos+1);
    //printf("subs: %s\n", subs.c_str());
    if(strcmp(subs.c_str(), "mpg") == 0)
        return true;
    if(strcmp(subs.c_str(), "avi") == 0)
        return true;
    return false;
}


bool isImageFormat(string s)
{
    //printf("s: %s\n", s.c_str());
    string subs;
    int findPos = s.find_last_of('.');
    if(findPos == string::npos)
        return false;
    subs = s.substr(findPos);
    //printf("subs: %s\n", subs.c_str());
    if(strcmp(subs.c_str(), ".tif") == 0)
        return true;
    if(strcmp(subs.c_str(), ".TIF") == 0)
        return true;
    return false;
}

bool directoryAutoSet(string s)
{
    struct stat st;
    stat(s.c_str(), &st);
    if(S_ISDIR(st.st_mode))
    {
        return true;
    }
    return false;
}
