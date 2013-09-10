#ifndef _LIAN_FILE_PROCESS
#define _LIAN_FILE_PROCESS

#include "globalInclude.h"

bool isVideoFormat(string s);                                             
bool isImageFormat(string s);
bool directoryAutoSet(string s);
void getImageStrings(string foldName, vector<string>& imgStrs);
bool fileExist(const char* pathname);

#endif
