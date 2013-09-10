#ifndef MY_HASH_PARAM
#define MY_HASH_PARAM
typedef struct MyHashParam
{
	//the number of bucket which should projection
	int bucketNumber;
	//the dimesion of the projection vector
	int projectionSize;
	//the capacity of bucket can store
	int bucketLength; 
	
	//weight for expectation and variance
	double we;
	// define the straight line in hash function\
	// we use same hash funtion for every bucket
	vector<float> p1, p2;
	
	//only when all bucket frequency more than votes, we compute the distance
	int votes;
	//the condition convex accord the already covex point ahead of time
	int nearVotes;
	
	//vector<int>** bucketInfo;
	vector< vector< vector<int> > > bucketInfo;
	Mat hashPoint;
	
	//for all the function;
	float minRange, maxRange, range;
	
	#ifdef HASH_FUNC_ONE
		float r;
	#else
		float a, b, c;
	#endif
	
}HashParam;

#endif
