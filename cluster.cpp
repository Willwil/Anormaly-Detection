void cluster(Mat& feature, int k, Mat& indices, Mat& centers)
{
	ClusterMethod method  =  K_Means;
	switch(method)
	{
		case K_Means:
            k_means(feature, k, indices, centers);
			break;
		case Spectral_Cluster:
            spectralClustering(feature, k, indices, centers);
			break;
	}
}

void k_means(const Mat& feature, int k, Mat& indices, Mat& centers)
{
	int try_again  =  5;
	TermCriteria criteria;
	criteria.epsilon  =  0.1;
	kmeans(feature, k, indices, criteria, try_again, KMEANS_PP_CENTERS, centers);
}

void spectralClustering(const Mat& feature, int k, Mat& indices, Mat& centers)
{
	Mat D;
	Mat W;
	Mat L;
	
	int n  =  feature.rows;
	int cols  =  feature.cols;
    W.create(n, n, CV_32FC1);//weight matrix
	//D.createn(n, n, CV_32FC1);
	D  =  Mat::zeros(n, n, CV_32FC1); //a square matrix
    for(int i  =  0 ; i < n; i++)
    {
        float* wdata  =  W.ptr<float>(i);
        const float* data  =  feature.ptr<float>(i);
        for(int j  =  0;  j < n; j++)
        {
        		wdata[j]  =  0;
        		if(i  =  =  j)
        		{
        			wdata[j]  =  1;
        			continue;
        		}
        		for(int k  =  0; k < cols; k ++)
        			wdata[j] + =  (data[k] - feature.at<float>(j, k)) * (data[k] - feature.at<float>(j, k));
        		wdata[j]  =  1/wdata[j];
        }
        
        float sum  =  0.0;
        for(int j  =  0;  j < n; j++)
        		sum + =  wdata[j];
        	D.at<float>(i, i)  =  sum;
    }
    
    L  =  D - W;  //****
    
    //SVD svd(L, )
    //SVD::compute(L, )
    Mat eigenValues;
    Mat eigenVectors;
    eigen(L, eigenValues, eigenVectors);
    Mat pcaResult;
    transpose(eigenVectors.rowRange(eigenVectors.rows - k, eigenVectors.rows), pcaResult);
    k_means(pcaResult, k, indices, centers);
}

void GMM(Mat feature, Mat indices, int K)
{


}


void meanshiftCluster(Mat feature, Mat indices)
{
	int nl  =  feature.rows;
	int nc  =  feature.cols;
	
	Mat W;
	W.create(nl, nl, CV_32FC1);//weight matrix
    for(int i  =  0 ; i < n; i++)
    {
        float* wdata  =  W.ptr<float>(i);
        const float* data  =  feature.ptr<float>(i);
        for(int j  =  0;  j < n; j++)
        {
        		wdata[j]  =  0;
        		if(i  =  =  j)
        		{
        			wdata[j]  =  1;
        			continue;
        		}
        		for(int k  =  0; k < cols; k ++)
        			wdata[j] + =  (data[k] - feature.at<float>(j, k)) * (data[k] - feature.at<float>(j, k));
        		wdata[j]  =  1/wdata[j];
        }
    }
 
	
	vector<float> shiftVector;	
	Mat convexPoints;
	convexPoints.create(nl, nc, CV_32FC1);
	
	//set params;
	float windowRadius = 5; //window ***
	int maxIterations = 100; //convex condition 1
	double epsilon = 11;   //convex condition 2
	
	int counts;
	int initialPoint;
	vector<float> molecular;
	float demoninator;
	
	for(int counts = 0; counts < nl; counts ++)
	{
		initialPoint = counts;
		for(int j = 0; j < nc; j ++)
		{
			shiftVector[j] =  feature.at<float>(initialPoint, j);
		}
		
		while(maxIterations > 0)
		{
			int inNeighbor = 0;
			demoninator = 0;
			for(int j = 0; j < nc; j ++)
			{
				molecular[j] =  0;
			}
					
			for(int i = 0; i < nl; i ++)
			{
				if(l2norm(shiftVector, i) < windowRadius)
				{
					double temp = math.exp(0 - l2norm(shiftVector, i)));
					demoninator +=  temp;
					for(int j = 0; j < nc; j ++)
					{
						molecular[j] +=  temp * feature.at<float>(i, j);
					}
					inNeighbor ++;
				}
			}
			if(inNeighbor == 0)
				exit(1);
			molecular = molecular / demoninator;
			
			if(l2norm(shiftVector, molecular) < epsilon)
				break;
			shiftVector = molecular;
			
			maxIterations --;
		}
		
		convexPoints.push_back(shiftVector);
	}
	
	
	//k_means(convexPoints, );
	
	//union
	double maxD = 11;//
	
	int clusterNumber = 0;
	vector<vector<int>> clusterIndex;
	int* nearestPoint = new int[nl];
	int* pointCluster = new int[nl];
	bool* clusterFlag = new bool[nl];
	for(int i = 0; i< nl;i ++) 
		clusterFlag[i] = false;
	
	for(int i = 0; i < nl; i ++)
	{
		float min = = l2norm(i, 0);
		int minIndex = 0;
		if(i == 0)
		{
			min = l2norm(i, 1);
			minIndex = 1;
		}
		for(int j = 0; j < nl; j ++)
		{
			if(j == i)
				continue;
			if(l2norm(i, j) < min)
			{
				min = l2norm(i, j);
				minIndex = j;
			}
		}
		if(min < maxD)
			nearestPoint[i] = j;
		else
			nearestPoint[i] = -1;
	}
	
	for(int i = 0; i < nl; i ++)
	{
		int m;
		int nearest = nearestPoint[i];
		if(nearest == -1)
		{
			if(!clusterFlag[i])
			{
				//vector<int> onecluster;
				//onecluster.push_back(i);
				//clusterIndex.push_back(onecluster);
				
				pointCluster[i] = clusterNumber;
				clusterNumber ++;
			}
		}
		else
		{
			if(!clusterFlag[i])
			{
			
				if(clusterFlag[nearest])
				{
					//clusterIndex[pointCluster[nearestPoint[i]]].push_back(i);
				
					pointCluster[i] = pointCluster[nearest];
				}
				else
				{
					//vector<int> onecluster;
					//onecluster.push_back(i);
					//clusterIndex.push_back(onecluster);
				
					pointCluster[i] = clusterNumber;
					clusterNumber ++;
				}
			}
			
			m = pointCluster[i];
			if(!clusterFlag[nearest])
			{
				//clusterIndex[m].push_back(nearestPoint[i]);
			
				pointCluster[nearest] = m;
				clusterFlag[nearest] = true;
			}
		}
		
		clusterFlag[i] = true;
		
	}
	
	if(!indices.empty)
		indices.release();
	indices.create(nl, 1, CV_8UC1);
	for(int i = 0; i < nl; i ++)
	{
		indices.at<uint>(i, 0) = pointCluster[i];	
	}
	
	delete[] nearestPoint;
	delete[] pointCluster;
	delete[] clusterFlag;
	nearestPoint = 0;
	pointCluster = 0;
	clusterFlag = 0;
}


/*void meanshift(Mat feature)
{
	int level  =  1;
	double color_radius2 = color_radius*color_radius;
	int minRegion  =  50;

	
	int nl  =  feature.rows;
	int nc  =  feature.cols;

	// Step Two. Cluster
	// Connect
	int regionCount  =  0;
	int *modePointCounts  =  new int[nl*nc];
	memset(modePointCounts, 0, nc*nl*sizeof(int));
	float *mode  =  new float[nl*nc*3];
	{
		int label  =  -1;
		for(int i = 0; i< nl;i ++) 
			for(int j = 0; j < nc; j ++)
				labels[i][j] = -1;
		for(int i = 0;i<nl;i++) 
			for(int j = 0;j<nc;j++)
				if(labels[i][j]<0)
				{
					labels[i][j]  =  ++label;
					float L  =  (float)((uchar *)(result->imageData + i*ncStep))[j*result->nChannels + 0],
						U  =  (float)((uchar *)(result->imageData + i*ncStep))[j*result->nChannels + 1],
						V  =  (float)((uchar *)(result->imageData + i*ncStep))[j*result->nChannels + 2];
					mode[label*3+0]  =  L*100/255;
					mode[label*3+1]  =  354*U/255-134;
					mode[label*3+2]  =  256*V/255-140;
					// Fill
					std::stack<CvPoint> neighStack;
					neighStack.push(cvPoint(i,j));
					const int dxdy[][2]  =  {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
					while(!neighStack.empty())
					{
						CvPoint p  =  neighStack.top();
						neighStack.pop();
						for(int k = 0;k<8;k++)
						{
							int i2  =  p.x+dxdy[k][0], j2  =  p.y+dxdy[k][1];
							if(i2> = 0 && j2> = 0 && i2<nl && j2<nc && labels[i2][j2]<0 && color_distance(result, i,j,i2,j2)<color_radius2)
							{
								labels[i2][j2]  =  label;
								neighStack.push(cvPoint(i2,j2));
								modePointCounts[label]++;
								L  =  (float)((uchar *)(result->imageData + i2*ncStep))[j2*result->nChannels + 0];
								U  =  (float)((uchar *)(result->imageData + i2*ncStep))[j2*result->nChannels + 1];
								V  =  (float)((uchar *)(result->imageData + i2*ncStep))[j2*result->nChannels + 2];
								mode[label*3+0] + =  L*100/255;
								mode[label*3+1] + =  354*U/255-134;
								mode[label*3+2] + =  256*V/255-140;
							}
						}
					}
					mode[label*3+0] / =  modePointCounts[label];
					mode[label*3+1] / =  modePointCounts[label];
					mode[label*3+2] / =  modePointCounts[label];
				}
				//current Region count
				regionCount  =  label+1;
	}			
	std::cout<<"Mean Shift(Connect):"<<regionCount<<std::endl;
	int oldRegionCount  =  regionCount;

	// TransitiveClosure
	for(int counter  =  0, deltaRegionCount  =  1; counter<5 && deltaRegionCount>0; counter++)
	{
		// 1.Build RAM using classifiction structure
		RAList *raList  =  new RAList [regionCount], *raPool  =  new RAList [10*regionCount];	//10 is hard coded!
		for(int i  =  0; i < regionCount; i++)
		{
			raList[i].label  =  i;
			raList[i].next  =  NULL;
		}
		for(int i  =  0; i < regionCount*10-1; i++)
		{
			raPool[i].next  =  &raPool[i+1];
		}
		raPool[10*regionCount-1].next  =  NULL;
		RAList	*raNode1, *raNode2, *oldRAFreeList, *freeRAList  =  raPool;
		for(int i = 0;i<nl;i++) 
			for(int j = 0;j<nc;j++)
			{
				if(i>0 && labels[i][j]! = labels[i-1][j])
				{
					// Get 2 free node
					raNode1			 =  freeRAList;
					raNode2			 =  freeRAList->next;
					oldRAFreeList	 =  freeRAList;
					freeRAList		 =  freeRAList->next->next;
					// connect the two region
					raNode1->label	 =  labels[i][j];
					raNode2->label	 =  labels[i-1][j];
					if(raList[labels[i][j]].Insert(raNode2))	//already exists!
						freeRAList  =  oldRAFreeList;
					else
						raList[labels[i-1][j]].Insert(raNode1);
				}
				if(j>0 && labels[i][j]! = labels[i][j-1])
				{
					// Get 2 free node
					raNode1			 =  freeRAList;
					raNode2			 =  freeRAList->next;
					oldRAFreeList	 =  freeRAList;
					freeRAList		 =  freeRAList->next->next;
					// connect the two region
					raNode1->label	 =  labels[i][j];
					raNode2->label	 =  labels[i][j-1];
					if(raList[labels[i][j]].Insert(raNode2))
						freeRAList  =  oldRAFreeList;
					else
						raList[labels[i][j-1]].Insert(raNode1);
				}
			}

			// 2.Treat each region Ri as a disjoint set
			for(int i  =  0; i < regionCount; i++)
			{
				RAList	*neighbor  =  raList[i].next;
				while(neighbor)
				{
					if(color_distance(&mode[3*i], &mode[3*neighbor->label])<color_radius2)
					{
						int iCanEl  =  i, neighCanEl	 =  neighbor->label;
						while(raList[iCanEl].label ! =  iCanEl) iCanEl  =  raList[iCanEl].label;
						while(raList[neighCanEl].label ! =  neighCanEl) neighCanEl  =  raList[neighCanEl].label;
						if(iCanEl<neighCanEl)
							raList[neighCanEl].label  =  iCanEl;
						else
						{
							//raList[raList[iCanEl].label].label  =  iCanEl;
							raList[iCanEl].label  =  neighCanEl;
						}
					}
					neighbor  =  neighbor->next;
				}
			}
			// 3. Union Find
			for(int i  =  0; i < regionCount; i++)
			{
				int iCanEl	 =  i;
				while(raList[iCanEl].label ! =  iCanEl) iCanEl	 =  raList[iCanEl].label;
				raList[i].label	 =  iCanEl;
			}
			// 4. Traverse joint sets, relabeling image.
			int *modePointCounts_buffer  =  new int[regionCount];
			memset(modePointCounts_buffer, 0, regionCount*sizeof(int));
			float *mode_buffer  =  new float[regionCount*3];
			int	*label_buffer  =  new int[regionCount];

			for(int i = 0;i<regionCount; i++)
			{
				label_buffer[i]	 =  -1;
				mode_buffer[i*3+0]  =  0;
				mode_buffer[i*3+1]  =  0;
				mode_buffer[i*3+2]  =  0;
			}
			for(int i = 0;i<regionCount; i++)
			{
				int iCanEl	 =  raList[i].label;
				modePointCounts_buffer[iCanEl] + =  modePointCounts[i];
				for(int k = 0;k<3;k++)
					mode_buffer[iCanEl*3+k] + =  mode[i*3+k]*modePointCounts[i];
			}
			int	label  =  -1;
			for(int i  =  0; i < regionCount; i++)
			{
				int iCanEl	 =  raList[i].label;
				if(label_buffer[iCanEl] < 0)
				{
					label_buffer[iCanEl]	 =  ++label;

					for(int k  =  0; k < 3; k++)
						mode[label*3+k]	 =  (mode_buffer[iCanEl*3+k])/(modePointCounts_buffer[iCanEl]);

					modePointCounts[label]	 =  modePointCounts_buffer[iCanEl];
				}
			}
			regionCount  =  label+1;
			for(int i  =  0; i < nl; i++)
				for(int j  =  0; j < nc; j++)
					labels[i][j]	 =  label_buffer[raList[labels[i][j]].label];

			delete [] mode_buffer;
			delete [] modePointCounts_buffer;
			delete [] label_buffer;

			//Destroy RAM
			delete[] raList;
			delete[] raPool;

			deltaRegionCount  =  oldRegionCount - regionCount;
			oldRegionCount  =  regionCount;
			std::cout<<"Mean Shift(TransitiveClosure):"<<regionCount<<std::endl;
	}

	// Prune
	{
		int *modePointCounts_buffer  =  new int[regionCount];
		float *mode_buffer  =  new float[regionCount*3];
		int	*label_buffer  =  new int [regionCount];
		int minRegionCount;

		do{
			minRegionCount  =  0;
			// Build RAM again
			RAList *raList  =  new RAList [regionCount], *raPool  =  new RAList [10*regionCount];	//10 is hard coded!
			for(int i  =  0; i < regionCount; i++)
			{
				raList[i].label  =  i;
				raList[i].next  =  NULL;
			}
			for(int i  =  0; i < regionCount*10-1; i++)
			{
				raPool[i].next  =  &raPool[i+1];
			}
			raPool[10*regionCount-1].next  =  NULL;
			RAList	*raNode1, *raNode2, *oldRAFreeList, *freeRAList  =  raPool;
			for(int i = 0;i<nl;i++) 
				for(int j = 0;j<nc;j++)
				{
					if(i>0 && labels[i][j]! = labels[i-1][j])
					{
						// Get 2 free node
						raNode1			 =  freeRAList;
						raNode2			 =  freeRAList->next;
						oldRAFreeList	 =  freeRAList;
						freeRAList		 =  freeRAList->next->next;
						// connect the two region
						raNode1->label	 =  labels[i][j];
						raNode2->label	 =  labels[i-1][j];
						if(raList[labels[i][j]].Insert(raNode2))	//already exists!
							freeRAList  =  oldRAFreeList;
						else
							raList[labels[i-1][j]].Insert(raNode1);
					}
					if(j>0 && labels[i][j]! = labels[i][j-1])
					{
						// Get 2 free node
						raNode1			 =  freeRAList;
						raNode2			 =  freeRAList->next;
						oldRAFreeList	 =  freeRAList;
						freeRAList		 =  freeRAList->next->next;
						// connect the two region
						raNode1->label	 =  labels[i][j];
						raNode2->label	 =  labels[i][j-1];
						if(raList[labels[i][j]].Insert(raNode2))
							freeRAList  =  oldRAFreeList;
						else
							raList[labels[i][j-1]].Insert(raNode1);
					}
				}
				// Find small regions
				for(int i  =  0; i < regionCount; i++)
					if(modePointCounts[i] < minRegion)
					{
						minRegionCount++;
						RAList *neighbor  =  raList[i].next;
						int candidate  =  neighbor->label;
						float minDistance  =  color_distance(&mode[3*i], &mode[3*candidate]);
						neighbor  =  neighbor->next;
						while(neighbor)
						{
							float minDistance2  =  color_distance(&mode[3*i], &mode[3*neighbor->label]);
							if(minDistance2<minDistance)
							{
								minDistance  =  minDistance2;
								candidate  =  neighbor->label;
							}
							neighbor  =  neighbor->next;
						}
						int iCanEl  =  i, neighCanEl	 =  candidate;
						while(raList[iCanEl].label ! =  iCanEl) iCanEl  =  raList[iCanEl].label;
						while(raList[neighCanEl].label ! =  neighCanEl) neighCanEl  =  raList[neighCanEl].label;
						if(iCanEl < neighCanEl)
							raList[neighCanEl].label	 =  iCanEl;
						else
						{
							//raList[raList[iCanEl].label].label	 =  neighCanEl;
							raList[iCanEl].label  =  neighCanEl;
						}
					}
					for(int i  =  0; i < regionCount; i++)
					{
						int iCanEl	 =  i;
						while(raList[iCanEl].label ! =  iCanEl)
							iCanEl	 =  raList[iCanEl].label;
						raList[i].label	 =  iCanEl;
					}
					memset(modePointCounts_buffer, 0, regionCount*sizeof(int));
					for(int i  =  0; i < regionCount; i++)
					{
						label_buffer[i]	 =  -1;
						mode_buffer[3*i+0]	 =  0;
						mode_buffer[3*i+1]	 =  0;
						mode_buffer[3*i+2]	 =  0;
					}
					for(int i = 0;i<regionCount; i++)
					{
						int iCanEl	 =  raList[i].label;
						modePointCounts_buffer[iCanEl] +=  modePointCounts[i];
						for(int k = 0;k<3;k++)
							mode_buffer[iCanEl*3+k] +=  mode[i*3+k]*modePointCounts[i];
					}
					int	label  =  -1;
					for(int i  =  0; i < regionCount; i++)
					{
						int iCanEl	 =  raList[i].label;
						if(label_buffer[iCanEl] < 0)
						{
							label_buffer[iCanEl] =  ++label;

							for(int k  =  0; k < 3; k++)
								mode[label*3+k]	 =  (mode_buffer[iCanEl*3+k])/(modePointCounts_buffer[iCanEl]);

							modePointCounts[label]	=  modePointCounts_buffer[iCanEl];
						}
					}
					regionCount  =  label+1;
					for(int i  =  0; i < nl; i++)
						for(int j  =  0; j < nc; j++)
							labels[i][j] = label_buffer[raList[labels[i][j]].label];

					//Destroy RAM
					delete[] raList;
					delete[] raPool;
					std::cout<<"Mean Shift(Prune):"<<regionCount<<std::endl;
		}while(minRegionCount > 0);

		delete [] mode_buffer;
		delete [] modePointCounts_buffer;
		delete [] label_buffer;
	}

	// Output
	STOP_TIMING(timer);
	std::cout<<"Mean Shift(ms):"<<GET_TIMING(timer)<<std::endl;

	cvReleaseImage(&result);
	delete []mode;
	delete []modePointCounts;
	return regionCount;
}*/	
