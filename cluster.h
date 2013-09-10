#ifndef LXL_CLUSTER_H
#define LXL_CLUSTER_H

#include "globalInclude.h"


extern int convexTime;


void k_means(const Mat& feature, int k, Mat& indices, vector<int> nums, Mat& centers);
void spectralClustering(const Mat& feature, int k, Mat& indices, Mat& centers);
double* twoStepCluster(const Mat& convexPoints);

#endif
