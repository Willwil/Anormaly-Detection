///////////////////////////////////////////////
#ifndef MY_HASH_SET_FUNCTION
#define MY_HASH_SET_FUNCTION

#include "globalInclude.h"
#include "HashParam.h"

void HashAllItems(const Mat& feature, const Mat& proj, HashParam& p);

int hashFunction(HashParam hp, vector<float> shiftVector, const Mat& subProj);

void ProjectAll(HashParam& p, const Mat& feature, const Mat& proj, Mat& projectValue);
float getProjectValue(HashParam p, vector<float> p3);

vector<int> calcHist(float* v, int n, int bins);
vector<int> smooth(vector<int> hists, int bins);

#endif
////////////////////////////////////////////////
