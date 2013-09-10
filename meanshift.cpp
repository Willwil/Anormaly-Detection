#include "meanshift.h"

#include "utils.h"
#include "cluster.h"
#include "ParamSetting.h"
#include "MyHashFuncSet.h"
#include "MatrixDataIo.h"


#define BOOST_SAMPLE
#define LOAD_HASHINFO_FROM_FILE

int shiftTime;
vector<int> iters;
vector<float> convexTrend;

#ifdef LSH_NEIGHBOR
vector<int> candidatesNumber;
vector<int> neighborFileterNumber;
#endif


//proj:each row represent which dimensions should projection in this hash,different row represent different bucket
void meanshiftCluster(const Mat& feature, Mat& convexPoints)
{
	printf("start to meanshift point...\n");
	
	time_t startTime = time(0);

	printf("start to compute the convex point for every point ...\n");
	int nl  =  feature.rows;
	int nc  =  feature.cols;
	printf("nl: %d, nc: %d.\n", nl, nc);
	convexPoints.create(nl, nc, CV_32FC1);
	
	//set params;
	MeanShiftParam mp;
	setMeanShiftParams(mp);
	
	#ifdef LSH_NEIGHBOR
		HashParam hp;
		setHashParam(hp, nl, nc);
		Mat proj;

		#ifdef LOAD_HASHINFO_FROM_FILE
			const char* hashFile = "./output/hashInfo.dat";
			readHashInfo(hashFile, proj, hp.bucketInfo);
			for(int i = 0; i < hp.bucketNumber; i ++)
			{
				for(int j = 0; j < hp.projectionSize; j ++)
				{
					int t = proj.at<int>(i, j);
					if(t < 0 || t >= nc)
						printf("i: %d, j: %d, t: %d\n", i, j, t);
				}
			}
		#else
			proj.create(hp.bucketNumber, hp.projectionSize, CV_32SC1);
			boostSample(hp.bucketNumber, hp.projectionSize, proj, nc);
			HashAllItems(feature, proj, hp);
			const char* hashFile = "./output/hashInfo.dat";
			saveHashInfo(hashFile, proj, hp.bucketInfo);
		#endif
		
		printf("bucketNumber: %d, projectionSize: %d.\n", hp.bucketNumber, hp.projectionSize);
		printf("bucket length: %d\n", hp.bucketLength);
		printHashParamInfo(hp);
		//mp.windowRadius = setWindow(hp, feature, proj);
		mp.windowRadius = 50.001144;
		mp.tinyNearestD = mp.windowRadius * 0.1;
		hp.votes = 2;
		
	#else
		boostSample(hp.bucketNumber, hp.projectionSize, proj, nc);
	#endif
	//end of set params;
	
	/********** set window **************/

	
	int initialPoint;
	float demoninator;
	vector<float> molecular(nc, 0);
	vector<float> shiftVector(nc, 0);
	int inNeighbor;

	int iterations;
	float oldChange;
	float newChange;
	bool advanceConvexed;
	int advancedConvexedPoint;
	
	vector<int> candidates;
	vector<int> frequency;
	vector<int> hashLocation(hp.bucketNumber, -1);
	vector<int> cursors(hp.bucketNumber, 0);
	vector<int> itemNumber(hp.bucketNumber, 0);
	
	vector<float> closeness;
	
	printf("start to enter circles for every point ...\n");
	for(int counts = 0; counts < nl; counts ++)
	{
		printf("the %d-th iterations ...\n", counts);
		initialPoint = counts;
		iterations = mp.maxIterations;
		advanceConvexed = false;
		for(int j = 0; j < nc; j ++)
		{
			shiftVector[j] =  feature.at<float>(initialPoint, j);
		}
		
		while(iterations > 0)
		{
			iterations --;
			inNeighbor = 0;
			demoninator = 0;
			for(int j = 0; j < nc; j ++)
			{
				molecular[j] =  0;
			}
			
			#ifdef LSH_NEIGHBOR
			//hash the shift vector and get candidates
			for(int bucket = 0; bucket < hp.bucketNumber; bucket ++)
			{
				//////////////////////////////////////////////////////////
				hashLocation[bucket] = hashFunction(hp, shiftVector, proj.row(bucket));
			}	
				
			candidates.clear();
			frequency.clear();
			for(int bucket = 0; bucket < hp.bucketNumber; bucket ++)
			{
				cursors[bucket] = 0;
				itemNumber[bucket] =  hp.bucketInfo[bucket][hashLocation[bucket]].size();
			}
			int candidateSize = 0;
			int lastElement = -1;
			while(1)
			{
				bool changeFlag = false;
				int min;
				int minBucket;
				for(int i = 0; i < hp.bucketNumber; i ++)
				{
					if(cursors[i] < itemNumber[i])
					{
						changeFlag = true;
						min = hp.bucketInfo[i][hashLocation[i]][cursors[i]];
						minBucket = i;
						break;
					}
				}
				if(!changeFlag)
					break;
			
				for(int i = minBucket + 1; i < hp.bucketNumber; i ++)
				{
					if(cursors[i] < itemNumber[i] && hp.bucketInfo[i][hashLocation[i]][cursors[i]] < min)
					{
						min = hp.bucketInfo[i][hashLocation[i]][cursors[i]];
						minBucket = i;
					}
				}
				if(min == lastElement)
				{
					frequency[candidateSize-1] ++;
				}
				else
				{
					candidates.push_back(min);
					frequency.push_back(1);
					candidateSize ++;
					lastElement = min;
				}
				cursors[minBucket] ++;
			}
			
			candidatesNumber.push_back(candidateSize);
			//printf("tag1\n");
			
			//filter the candidates;
			int statisticalFilted = 0;
			int size = candidateSize;
			
			/*closeness.clear();
			closeness.resize(size, 0.0);
			for(int cc = 0; cc < size; cc ++)
			{
				int near = candidates[cc];
				for(int bucket = 0; bucket < hp.bucketNumber; bucket ++)
					closeness[cc] += weight[bucket] * (hashLocation[bucket] - hp.hashInfo[near][bucket]) ** 2;
				if(closeness[cc] > mp.windowRadius)
					continue;
				
				if(counts > cc && closeness[cc] < mp.tinyNearestD)
				{
					advancedConvexedPoint = cc;
					advanceConvexed = true;	
					float* curent_data = convexPoints.ptr<float>(counts);
					const float* convex_data = convexPoints.ptr<float>(cc);
					for(int j = 0; j < nc; j ++)
						curent_data[j] = convex_data[j];
					break;
				}
				double temp = exp(0 - closeness[cc]);
				demoninator +=  temp;
				for(int j = 0; j < nc; j ++)
				{
					molecular[j] +=  temp * feature.at<float>(candidates[cc], j);
				}
				inNeighbor ++;
			}*/
			
			for(int cc = 0; cc < size; cc ++)
			{
				if(frequency[cc] >= hp.votes)  
				{
				 	statisticalFilted ++;
					float t = l2norm(shiftVector, feature.row(candidates[cc]));
					//indicate the cc point have convexed and the hash value vevy same
					//and their distance is very near, it's convex
					if(counts > cc && t < mp.tinyNearestD)
					{
						advancedConvexedPoint = cc;
						advanceConvexed = true;	
						float* curent_data = convexPoints.ptr<float>(counts);
						const float* convex_data = convexPoints.ptr<float>(cc);
						for(int j = 0; j < nc; j ++)
							curent_data[j] = convex_data[j];
						break;
					}
					if(t < mp.windowRadius)
					{
						double temp = exp(0 - t);
						demoninator +=  temp;
						for(int j = 0; j < nc; j ++)
						{
							molecular[j] +=  temp * feature.at<float>(candidates[cc], j);
						}
						inNeighbor ++;
					}
				}
			}
			printf("candidateSize: %d, statisticalFilted: %d, inNeighbor: %d\n", candidateSize, statisticalFilted, inNeighbor);
			neighborFileterNumber.push_back(statisticalFilted);
			//printf("tag2\n");
			
			#else
			for(int i = 0; i < nl; i ++)
			{
				float t = l2norm(shiftVector, feature.row(i));
				if(t < mp.tinyNearestD && counts > i)
				{
					advancedConvexedPoint = i;
					advanceConvexed = true;	
					float* curent_data = convexPoints.ptr<float>(counts);
					const float* convex_data = convexPoints.ptr<float>(i);
					for(int j = 0; j < nc; j ++)
						curent_data[j] = convex_data[j];
					break;
				}
				if(t < windowRadius)
				{
					double temp = exp(0 - t);
					demoninator +=  temp;
					for(int j = 0; j < nc; j ++)
					{
						molecular[j] +=  temp * feature.at<float>(i, j);
					}
					inNeighbor ++;
				}
			}
			#endif

			if(inNeighbor == 0)
			{
				printf("neighbor: 0\n");
				advanceConvexed = true;
				advancedConvexedPoint = counts;
				float* curent_data = convexPoints.ptr<float>(counts);
				const float* convex_data = feature.ptr<float>(advancedConvexedPoint);
				for(int j = 0; j < nc; j ++)
					curent_data[j] = convex_data[j];
				break;
			}
			if(advanceConvexed)
				break;
				
			demoninator = 1.0/demoninator;
			for(int j = 0; j < nc; j ++)
			{
				molecular[j] = 	demoninator * molecular[j];
			}
			//printf("tag3\n ");
			
			
			newChange = l2norm(shiftVector, molecular);
			convexTrend.push_back(newChange);
			if(newChange / oldChange < mp.epsilon)
				break;
			oldChange = newChange;
			shiftVector = molecular;
			//printf("tag4\n ");
		}
		
		convexTrend.push_back(-1);//seprate by -1
		iters.push_back(mp.maxIterations - iterations);
		
		if(!advanceConvexed)
		{
			float* data = convexPoints.ptr<float>(counts);
			for(int j = 0; j < nc; j ++)
				data[j] = shiftVector[j];
		}   
	}

	string writer = "./output/convex.dat";
	MatrixDataIo mdi(writer.c_str(), true, convexPoints);
	
	#ifdef LSH_NEIGHBOR
	printf("destroy the bucket info ...\n");
	destroyBucket(hp);
	#endif
	
	//compute used time
	{
		time_t endTime = time(0);
		int totalUsed = endTime - startTime;
		//comparedTime[1] = totalUsed;
		shiftTime = totalUsed;
		printf("meanshift LSH: %d\n", totalUsed);
	}	
	
	programPause();
	printf("exit meanshift clustering...\n");
}


void boostSample(int sample_m, int sample_n, Mat& proj, int maxSampleValue)
{
	#ifdef BOOST_SAMPLE
		printf("start to sample feature dimension ...\n");
		srand(time(0));
		for(int i = 0; i < sample_m; i ++)
		{
			for(int j = 0; j < sample_n; j ++)
			{
				proj.at<int>(i, j) = rand() % maxSampleValue;	
				//printf("%d ", proj.at<int>(i, j));
			}
			//printf("\n");
		}
	#else
		for(int i = 0; i < sample_m; i ++)
		{
			for(int j = 0; j < sample_n; j ++)
				proj.at<int>(i, j) = i * sample_n + j;	
		}
	#endif
}


int setWindow(HashParam hp, const Mat& feature, const Mat& proj)
{
	printf("start to auto-set window size ...\n");
	//sample
	int n = feature.rows;
	int nc = feature.cols;
	
	float trySize;//尝试窗口大小
	int omit;//ignorge the head and tail sample element
	int sum_neighbors = 0;
	float average_neighbors;
	int retained_neighbors = 20;
	trySize = 50;
	
	vector<float> sp(nc, 0);
	int candidateSize = 0;
	int neighbors;
	srand(time(0));
	
	omit = 1;
	int throw_stone = 10;
	for(int i =0; i < throw_stone; i ++)
	{
		int t = rand() % n;
		for(int j = 0; j < nc; j ++)
		{
			sp[j] =  feature.at<float>(t, j);
		}
		neighbors = 0;
		vector<int> candidates = getCandidates(hp, sp, proj);
		candidateSize = candidates.size();
		for(int i = 0; i < candidateSize; i ++)
		{
			float distance = l2norm(sp, feature.row(candidates[i]));
			if(distance < trySize)
				neighbors ++;
		}
		sum_neighbors += neighbors;
	}
	average_neighbors = sum_neighbors * 1.0/throw_stone;
	printf("average_neighbors: %f\n", average_neighbors);
	trySize = trySize * pow(retained_neighbors/average_neighbors, 1.0/nc);
	printf("trySize: %f\n", trySize);
	
	
	int samplePoints = 50;
	sum_neighbors = 0;
	omit = 5;
	printf("samplePoints: %d\n", samplePoints);
	for(int i =0; i < samplePoints; i ++)
	{
		int t = rand() % n;
		for(int j = 0; j < nc; j ++)
		{
			sp[j] =  feature.at<float>(t, j);
		}
		neighbors = 0;
		vector<int> candidates = getCandidates(hp, sp, proj);
		candidateSize = candidates.size();
		for(int i = 0; i < candidateSize; i ++)
		{
			float distance = l2norm(sp, feature.row(candidates[i]));
			if(distance < trySize)
				neighbors ++;
		}
		sum_neighbors += neighbors;
		printf("%d\n", neighbors);
	}
	
	printf("\nsum_neighbors: %d\n", sum_neighbors);
	average_neighbors = sum_neighbors * 1.0/samplePoints;
	printf("average_neighbors: %f\n", average_neighbors);
	
	float r = trySize * pow(average_neighbors / retained_neighbors, 1.0/nc);
	printf("window size: %f\n", r);
	return r;
}

vector<int> getCandidates(HashParam hp, vector<float> sp, const Mat& proj)
{
	vector<int> candidates;
	vector<int> frequency;
	vector<int> hashLocation(hp.bucketNumber, -1);
	vector<int> cursors(hp.bucketNumber, 0);
	vector<int> itemNumber(hp.bucketNumber, 0);
	

	for(int bucket = 0; bucket < hp.bucketNumber; bucket ++)
	{
		hashLocation[bucket] = hashFunction(hp, sp, proj.row(bucket));
	}	
		
	candidates.clear();
	frequency.clear();
	for(int bucket = 0; bucket < hp.bucketNumber; bucket ++)
	{
		cursors[bucket] = 0;
		itemNumber[bucket] =  hp.bucketInfo[bucket][hashLocation[bucket]].size();
	}
	int candidateSize = 0;
	int lastElement = -1;
	//printf("start...get candidtate ...\n");
	while(1)
	{
		bool changeFlag = false;
		int min;
		int minBucket;
		for(int i = 0; i < hp.bucketNumber; i ++)
		{
			if(cursors[i] < itemNumber[i])
			{
				changeFlag = true;
				min = hp.bucketInfo[i][hashLocation[i]][cursors[i]];
				minBucket = i;
				break;
			}
		}
		if(!changeFlag)
			break;
		
		for(int i = minBucket + 1; i < hp.bucketNumber; i ++)
		{
			if(cursors[i] < itemNumber[i] && hp.bucketInfo[i][hashLocation[i]][cursors[i]] < min)
			{
				min = hp.bucketInfo[i][hashLocation[i]][cursors[i]];
				minBucket = i;
			}
		}
		if(min == lastElement)
		{
			frequency[candidateSize-1] ++;
		}
		else
		{
			candidates.push_back(min);
			frequency.push_back(1);
			candidateSize ++;
			lastElement = min;
		}
		cursors[minBucket] ++;
	}
	//printf("end...get candidtate ...\n");
	
	return candidates;
}
