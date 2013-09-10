#ifndef MY_MEANSHIFT_PARAM
#define MY_MEANSHIFT_PARAM

typedef struct MyMeanShiftParam
{
	//the neighbor justify window
	//should change as convex???
	double windowRadius; 
	//convex condition 1, reached the iterations
	int maxIterations; 
	//convex condition 2, no change
	//the rate between this change and last change is less than epsilon
	double epsilon; 
	//the ignornabel distance
	double tinyNearestD;
}MeanShiftParam;

#endif
