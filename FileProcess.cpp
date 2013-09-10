#include "FileProcess.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

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

//get the sequence image name in folder;
void getImageStrings(string foldName, vector<string>& imgStrs)
{
    DIR* dir = 0;
    struct dirent* s_dir = 0;
    struct stat st;
    char path[MaxStringLength] = "";

	printf("folder: %s\n", foldName.c_str());
    dir = opendir(foldName.c_str());
    if(dir == NULL)
    {
    	printf("directory not exist!\n");
        return;
    }

    vector<string> substrs;
    while(s_dir = readdir(dir))
    {
        if(strcmp(s_dir->d_name, ".")==0 || strcmp(s_dir->d_name, "..")==0)
            continue;
        //printf("d_name: %s\n", s_dir->d_name);
        if(s_dir->d_name[0] == '.')
            continue;
        stat(s_dir->d_name, &st);
        //printf("d_name: %s\n", st.st_mode);
        if(!isImageFormat(s_dir->d_name))
            continue;
        substrs.insert(s_dir->d_name);
    }
    if(dir)
        closedir(dir);
    set<string>::iterator it = substrs.begin();
    while(it != substrs.end())
    {
        sprintf(path, "%s/%s", foldName.c_str(), (*it).c_str());
        imgStrs.push_back(path);
        it++;
    }
}


bool fileExist(const char* pathname)
{
	int result = access(pathname, R_OK);
	if(result == 0) 
		return true;
	return false;
}
