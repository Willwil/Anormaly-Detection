#include "outlier.h"

//#define ANALYSIS_ANORMALY


//clusterSizeStat[i]:the point number 
//so the size is same with the cluster number
vector<int> clusterSizeStat;
//abnormalRate[i]: the anormal rate of a specified cluster
//so the size is same with the cluster number
vector<float> abnormalRate;




//indices start from 0
//abandon the cluster whose size is less than theta
bool* throwBlockOutlier(const Mat& indices, float theta)
{
	printf("start to distinguish the anormaly clusters ...\n");
	
	bool* abandoned = new bool[indices.rows];
	int n = indices.rows;
	vector<int> counts;
	int findClusterNumber = 0;
	
	//get every cluster numbers
	for(int i = 0; i < n; i ++)
	{
		abandoned[i] = false;
		int t = indices.at<int>(i, 0);
		//if(t != 0)
		//	printf("%d ", t);
		if(t < findClusterNumber)
			counts[t] ++;
		else
		{
			counts.resize(t+1, 0);
			counts[t] ++;
			findClusterNumber = t;
		}
	}
	printf("\n");
	printf("class number: %d\n", findClusterNumber);
	
	vector<bool> cluster_distinct(findClusterNumber, false);
	for(int i = 0; i < findClusterNumber; i ++)
	{
		if( (counts[i] * 1.0) / n < theta)
			cluster_distinct[i] = true;
	}
	for(int i = 0; i < n; i ++)
	{
		abandoned[i] = cluster_distinct[indices.at<int>(i, 0)];
	}
	
	
	
	#ifdef ANALYSIS_ANORMALY
		vector<int> actualAnormalInCluster(s, 0);
		int* labels = readLable("label.file");
		for(int i = 0; i < n; i ++)
		{
			if(labels[i] == 1)
			{
				int t = indices.at<int>(i, 0);
				actualAnormalInCluster[t] ++;
			}
		}
		int k = 0;
		for(int i = 0; i < s; i ++)
		{	
			if(actualAnormalInCluster[i] > 0)
			{
				abnormalRate.push_back(actualAnormalInCluster[i] / clusterSizeStat[k]);
				k ++;
			}
		}
	#endif	
	
	return abandoned;
}


//indices start from 0
//abandon the cluster whose size is not possible when normal(gaussian probality)
bool* throwBlockOutlier_Two(const Mat& indices)
{
	printf("start to distinguish the anormaly clusters in method two...\n");
	bool* abandoned = new bool[indices.rows];
	int n = indices.rows;
	//sort by class ID
	vector<int> counts;  
	
	//get every cluster numbers
	for(int i = 0; i < n; i ++)
	{
		abandoned[i] = false;
		int t = indices.at<int>(i, 0);
		if(t < counts.size())
			counts[t] ++;
		else
		{
			counts.resize(t+1, 0);
			counts[t] ++;
		}
	}
	
	//get the mean number of cluster size, and regard cluster size obey gaussian distribution.
	float mean, mean_square, delta_square;
	mean = 0.0;
	mean_square = 0.0;
	
	int s = counts.size();
	int clusterSize = 0;
	
	printf("counts: ");
	for(int i = 0; i < s; i ++)
	{
		int t = counts[i];
		if(t > 0)
		{
			mean += t;
			mean_square += t * t;
			clusterSize ++;
			printf("%d ", t);
		}
	}
	printf("\n");
	mean = mean / clusterSize;
	delta_square = mean_square/clusterSize - mean*mean;
	printf("class number: %d\n", clusterSize);
	
	//accord the min probality, compute the impossibel size range.
	double min_prob = 0.01;
	const float pi = 3.14159265;
	int threshod = (int)sqrt(log(min_prob * (sqrt(2*pi*delta_square))) * delta_square) + mean;
	printf("threshod: %d\n", threshod);
	
	for(int i = 0; i < n; i ++)
	{
		if(counts[indices.at<int>(i, 0)] < threshod)
			abandoned[i] = true;
	}
	
	for(int i = 0; i < s; i ++)
	{
		int t = counts[i];
		if(t > 0)
			clusterSizeStat.push_back(t);
	}
	#ifdef ANALYSIS_ANORMALY
		vector<int> actualAnormalInCluster(s, 0);
		int* labels = readLable("label.file");
		for(int i = 0; i < n; i ++)
		{
			if(labels[i] == 1)
			{
				int t = indices.at<int>(i, 0);
				actualAnormalInCluster[t] ++;
			}
		}
		int k = 0;
		for(int i = 0; i < s; i ++)
		{
			int t = actualAnormalInCluster[i];
			if(t > 0)
			{
				abnormalRate.push_back(t / clusterSizeStat[k]);
			}
		}
	#endif
	
	return abandoned;
}

bool* decideAnormal(double* score, double theta, int n)
{
	printf("entering deciding ...\n");
	bool* anormal = new bool[n];
	for(int i = 0; i < n; i ++)
	{
		if(score[i] > theta)
			anormal[i] = false;
		else
			anormal[i] = true;
	}
	int win_r = 1;
	for(int i = 0; i < n; i ++)
	{
		bool result;
		if(anormal[i])
		{
			result = false;
			for (int j = i-win_r; j <= i+win_r; ++j)
			{
				if(j < 0 || j == i)
					continue;
				result = result || anormal[j];
				//printf("result\n");
			}
			if(!result)
				anormal[i] = false;
				
		}
		else
		{
			result = true;
			for (int j = i-win_r; j <= i+win_r; ++j)
			{
				if(j < 0 || j == i)
					continue;
				result = result && anormal[j];
				//printf("result\n");
			}
			if(result)
				anormal[i] = false;
		}
	}
	return anormal;
}