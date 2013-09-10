#include "ParamSetting.h"
#include "utils.h"

void setHashParam(HashParam& p, int nl, int nc)
{
	//the dimesion of the projection vector
	p.projectionSize = 10;
	int repetionNumber = 2;
	//the number of bucket which should projection
	p.bucketNumber = nc*repetionNumber / p.projectionSize;
	//the capacity of bucket can store
	p.bucketLength = nl / 50; 
	
	//weight for expectation and variance
	p.we = 1.0;
	// define the straight line in hash function\
	
	// we use same hash funtion for every bucket
	p.p1.resize(p.projectionSize, 0);
	p.p2.resize(p.projectionSize, 1);
	for(int i = 0; i < p.projectionSize; i ++)
	{
	}
	
	//only when all bucket frequency more than votes, we compute the distance
	p.votes = p.bucketNumber/2;
	//the condition convex accord the already covex point ahead of time
	p.nearVotes = p.bucketNumber *4/5;
	
	p.bucketInfo.resize(p.bucketNumber, vector< vector<int> >());
	for(int i = 0; i < p.bucketNumber; i ++)
	{
		p.bucketInfo[i].resize(p.bucketLength, vector<int>());
	}
}


void setMeanShiftParams(MeanShiftParam& p)
{
	//set params;
	//the neighbor justify window
	//should change as convex???
	p.windowRadius = 5; 
	//convex condition 1, reached the iterations
	p.maxIterations = 500; 
	//convex condition 2, no change
	//the rate between this change and last change is less than epsilon
	p.epsilon = 0.001; 
	p.tinyNearestD = 0.2;
}



void destroyBucket(HashParam& p)
{
	for(int i = 0; i < p.bucketNumber; i ++)
	{
		for(int j = 0; j < p.bucketLength; j ++)
			p.bucketInfo[i][j].clear();
		p.bucketInfo[i].clear();
	}
	p.bucketInfo.clear();
}

void printHashParamInfo(HashParam p)
{
	printf("bucketNumber: %d, projectionSize: %d, bucketLength: %d.\n", p.bucketNumber, p.projectionSize, p.bucketLength); 
	printf("weight: %f, votes: %d, nearVotes: %d\n", p.we, p.votes, p.nearVotes);
	printf("minRange: %f, maxRange: %f, range: %f\n", p.minRange, p.maxRange, p.range);

	for(int i = 0; i < p.projectionSize; i ++)
		printf("p1[%d]: %f ", i, p.p1[i]);
	printf("\n");
	for(int i = 0; i < p.projectionSize; i ++)
		printf("p2[%d]: %f ", i, p.p2[i]);
	printf("\n");
	
	printf("bucketInfo\n");
	printf("p.bucketInfo.size(): %d \n", p.bucketInfo.size());
	for(int i = 0; i < 1; i ++)
	{
		printf("p.bucketInfo[%d].size(): %d \n", i, p.bucketInfo[i].size());
		for(int j = 0; j < p.bucketLength; j ++)
		{
			int size = p.bucketInfo[i][j].size();
			printf("%d ", size);
		}
		printf("\n");
	}
}


void saveHashInfo(const char* hashFile, const Mat& proj, const vector< vector< vector<int> > >& bucketInfo)
{
	printf("writing bucketInfo to file ...\n");
	ofstream ofs;
	ofs.open(hashFile, ofstream::out);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", hashFile);
        exit(1);
	}
	uint bucketNumber = proj.rows; 
	uint projectDimension = proj.cols; 
	assert(bucketNumber > 0 && projectDimension > 0);
	ofs << "#sample info, bucketLength, projectDimension" << endl;
	ofs << bucketNumber << " " << projectDimension << endl;
	for(int i =0 ; i < bucketNumber; i ++)
	{
		for(int j = 0 ; j < projectDimension; j ++)
			ofs << proj.at<int>(i, j) << " ";
		ofs << endl;
	}
	
	assert(bucketNumber == bucketInfo.size());
	uint bucketLength = bucketInfo[0].size();
	ofs << "#bucketInfo, bucketNumber, bucketLength" << endl;
	ofs << bucketNumber << " " << bucketLength << endl;
	assert(bucketNumber > 0 && bucketLength > 0);
	
	ofs << "#bucketInfo: numbers of  every position " << endl;
	for(int i =0 ; i < bucketNumber; i ++)
	{
		for(int j = 0 ; j < bucketLength; j ++)
		{
			int vlen = bucketInfo[i][j].size();
			ofs << vlen << " ";
		}
		ofs << endl;
	}
	ofs << "#core: projection hash information" << endl;
	for(int i =0 ; i < bucketNumber; i ++)
	{
		for(int j = 0 ; j < bucketLength; j ++)
		{
			int vlen = bucketInfo[i][j].size();
			for(int k = 0 ; k < vlen; k ++)
				ofs << bucketInfo[i][j][k] << " ";
		}
	}
	ofs.close();
	printf("end ...writing bucketInfo to file ...\n");
}

void readHashInfo(const char* hashFile, Mat& proj, vector< vector< vector<int> > >& bucketInfo)
{
	printf("reading bucketInfo from file ...\n");
	if(!proj.empty())
		proj.release();
	if(!bucketInfo.size() != 0)
	{
		int ns = bucketInfo.size();
		for(int i =0 ; i < ns; i ++)
		{
			int nss = bucketInfo[i].size();
			if(nss != 0)
			{
				for(int j =0 ; j < nss; j ++)
				{
					int nsss = bucketInfo[i][j].size();
					if(nsss != 0)
						bucketInfo[i][j].clear();
				}
				bucketInfo[i].clear();
			}
		}
		bucketInfo.clear();
	}
		
	ifstream ifs(hashFile, ifstream::in);
	if(!ifs.is_open())
	{
		printf("error occured: can't open %s for read.\n", hashFile);
        exit(1);
	}
	
	int bucketNumber, bucketLength, projectDimension;
	string buffer;
	
	getline(ifs, buffer);
	ifs >> bucketNumber >> projectDimension;
	proj.create(bucketNumber, projectDimension, CV_32SC1);
	for(int i =0 ; i < bucketNumber; i ++)
	{
		for(int j = 0 ; j < projectDimension; j ++)
		{
			ifs >> proj.at<int>(i, j);
			//printf("%d ", proj.at<int>(i, j));
		}
		//printf("\n");
	}
	
	getline(ifs, buffer);
	getline(ifs, buffer);
	int oldNumber = bucketNumber;
	ifs >> bucketNumber >> bucketLength;
	assert(oldNumber == bucketNumber);
	bucketInfo.resize(bucketNumber, vector< vector<int> >());
	
	getline(ifs, buffer);
	getline(ifs, buffer);
	int vlen;
	for(int i =0 ; i < bucketNumber; i ++)
	{
		bucketInfo[i].resize(bucketLength, vector<int>());
		for(int j = 0 ; j < bucketLength; j ++)
		{
			ifs >> vlen;
			bucketInfo[i][j].resize(vlen, -1);
		}
	}
	
	getline(ifs, buffer);
	getline(ifs, buffer);
	for(int i =0 ; i < bucketNumber; i ++)
	{
		for(int j = 0 ; j < bucketLength; j ++)
		{
			int vlen = bucketInfo[i][j].size();
			for(int k = 0 ; k < vlen; k ++)
				ifs >> bucketInfo[i][j][k];
		}
	}
	
	ifs.close();
}
