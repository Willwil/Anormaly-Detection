///////////////////////////////////////////////
#ifndef MY_PARAM_SETTING
#define MY_PARAM_SETTING

#include "globalInclude.h"
#include "HashParam.h"
#include "MeanShiftParam.h"


void setHashParam(HashParam& p, int nl, int nc);
void setMeanShiftParams(MeanShiftParam& p);

void destroyBucket(HashParam& p);
void printHashParamInfo(HashParam p);

void saveHashInfo(const char* hashFile, const Mat& proj, const vector< vector< vector<int> > >& bucketInfo);
void readHashInfo(const char* hashFile, Mat& proj, vector< vector< vector<int> > >& bucketInfo);

#endif
////////////////////////////////////////////////

