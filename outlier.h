#ifndef LXL_OUTLIER
#define LXL_OUTLIER

#include "globalInclude.h"

//clusterSizeStat[i]:the point number 
//so the size is same with the cluster number
extern vector<int> clusterSizeStat;
//abnormalRate[i]: the anormal rate of a specified cluster
//so the size is same with the cluster number
extern vector<float> abnormalRate;


bool* throwBlockOutlier(const Mat& indices, float theta);
bool* throwBlockOutlier_Two(const Mat& indices);
bool* decideAnormal(double* score, double theta, int n);

#endif
