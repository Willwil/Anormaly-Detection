#include "cluster.h"
#include "utils.h"
#include "NeighborInfo.h"
#include "ClusterNode.h"
#include "hierarchical.h"
#include "Kmeans.h"


int convexTime;


void spectralClustering(const Mat& feature, int k, Mat& indices, Mat& centers)
{
	//use for sparse the graph
	//so can decrease the graph complexity
	float sparseDistance;
	
	Mat D;
	Mat W;
	Mat L;
	
	int n  =  feature.rows;
	int cols  =  feature.cols;
	//weight matrix
    W.create(n, n, CV_32FC1);
    //a square and diag matrix
	D  =  Mat::zeros(n, n, CV_32FC1); 
	float mean, mean_square;
	mean = mean_square = 0;
	
    for(int i  =  0 ; i < n; i++)
    {
        float* wdata  =  W.ptr<float>(i);
        const float* data  =  feature.ptr<float>(i);
        for(int j  =  0;  j < n; j++)
        {
        		wdata[j]  =  0;
        		if(i  ==  j)
        		{
        			wdata[j]  =  1;
        			continue;
        		}
        		for(int k  =  0; k < cols; k ++)
        			wdata[j] +=  (data[k] - feature.at<float>(j, k)) * (data[k] - feature.at<float>(j, k));
        		wdata[j]  =  1/wdata[j];
        		mean += wdata[j];
        		mean_square += wdata[j] * wdata[j];
        }
    }
    mean = mean/(n * n);
    float delta = sqrt(mean_square/(n*n) - mean*mean);
    double min_prob = 0.01;
	const float pi = 3.14159265;
	sparseDistance = (mean - 2 * delta);
    
    for(int i  =  0 ; i < n; i++)
    {
        float* wdata  =  W.ptr<float>(i);
        float sum  =  0.0;
        for(int j  =  0;  j < n; j++)
        {	
        		if(wdata[j] < sparseDistance)
        			wdata[j]  =  0;
        		sum +=  wdata[j];
        }		
        D.at<float>(i, i)  =  sum;
    }
    
    
    Mat eigenValues;
    Mat eigenVectors;
    //SVD factor for matrix L
    L  =  D - W;
    eigen(L, eigenValues, eigenVectors);
    //printInfo(eigenValues);
    Mat pcaResult;
    //get the last k miniest eiggenvalue rows for store in matrix pcaResult
    transpose(eigenVectors.rowRange(eigenVectors.rows - k, eigenVectors.rows), pcaResult);
    //kmeans
    Kmeans km(pcaResult, k);
    indices = km.indices;
    centers = km.centers;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//may be more faster
//first, spectral cluster
//second, the nearest neighbor cluster or hierarchical cluster
double* twoStepCluster(const Mat& convexPoints)
{
	printf("start to cluster the convex points using two tep method ...\n");
	
	time_t startTime = time(0);
	
	int nl = convexPoints.rows;
	int k = 200;
	Kmeans km(convexPoints, k);
    //printf("centers: %d\n", km.centers.rows);
    //std::cout << "centers: \n" << km.centers << endl;
    //cout << "nums: \n";
    //cout << km.nums;
    //cout << endl;
	//spectralClustering(convexPoints, k, indices, centers);
	printf("tag 1...\n");
	
	//further process
	Hierarchical hc(km.centers, km.nums);
	printf("tag 2...\n");
	
	double* score = hc.collectScore(km.indices);
    //double* score = km.collectScore();
	printf("end of cluster the convex points using method 2...\n");
	return score;
}
