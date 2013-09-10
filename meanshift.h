#ifndef MY_MEAN_SHIFT
#define MY_MEAN_SHIFT


#include "globalInclude.h"
#include "MeanShiftParam.h"
#include "HashParam.h"

#define LSH_NEIGHBOR

extern int shiftTime;
//check up the convex trend of a random selected point
extern vector<float> convexTrend;
extern vector<int> iters;

#ifdef LSH_NEIGHBOR
	//the retained point after LSH in search neighbors in every iteration step
	//these point at least have a same hash value
	extern vector<int> candidatesNumber;
	//the retained point after LSH in search neighbors in every iteration step
	//after all the hash function votes
	extern vector<int> neighborFileterNumber;
#endif

void meanshiftCluster(const Mat& feature, Mat& convexPoint);

void boostSample(int sample_m, int sample_n, Mat& proj, int maxSampleValue);

int setWindow(HashParam hp, const Mat& feature, const Mat& proj);
vector<int> getCandidates(HashParam hp, vector<float> sp, const Mat& proj);
#endif
