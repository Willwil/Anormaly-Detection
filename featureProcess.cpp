#include "featureProcess.h"
#include "utils.h"


//to [0, 1].
void normalizeFeature1(Mat& feature, bool type)
{
    
    //cout << feature << endl;
    int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    float* min = new float[featureSize];   //
    float* max = new float[featureSize];  //
    string scaleFac = "scale-factor";
    FILE* file;
    
    if(!type)
    {
        file = fopen(scaleFac.c_str(), "r");
        if(!file)
        {
            printf("%s can't open!\n", scaleFac.c_str());
            exit(1);
        }
        for(int i = 0; i < featureSize; i ++)
        {
            if(fscanf(file, "%f %f", &min[i], &max[i]) == EOF)
            {
                printf("the normalise file %s exist error!\n", scaleFac.c_str());
                exit(1);
            }
        }
        
    }
    else
    {
        for(int i = 0; i < featureSize; i ++)
        {
            min[i] = feature.at<float>(0, i);
            max[i] = min[i];
        }
        for(int i = 1; i < sampleNumber; i ++)
        {
            float* data = feature.ptr<float>(i);
            for(int j =0; j < featureSize; j ++)
            {
                if(data[j] < min[j])
                    min[j] = data[j];
                if(data[j] > max[j])
                    max[j] = data[j];
            }
        }
        file = fopen(scaleFac.c_str(), "w");
        for(int i = 0; i < featureSize; i ++)
        {
            fprintf(file, "%f %f\n", min[i], max[i]);
        }
    }
    
    for(int i = 0; i < sampleNumber; i ++)
    {
        float* data = feature.ptr<float>(i);
        for(int j =0; j < featureSize; j ++)
        {
            data[j] = (data[j]-min[j]) / (max[j]-min[j]);
        }

    }
    
    delete[] min;
    delete[] max;
    min = 0; 
    max = 0;
}

//mean:0, delta=1;
void normalizeFeature2(Mat& feature)
{
	cout << feature << endl;
    int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    //expectation
    double* mean = new double[featureSize];  
    //variance
    double* delta = new double[featureSize];  
    
    for(int i = 0; i < featureSize; i ++)
    {
        mean[i] = 0;
        delta[i] = 0;
    }
    
    for(int i = 0; i < sampleNumber; i ++)
    {
        float* data = feature.ptr<float>(i);
        for(int j =0; j < featureSize; j ++)
        {
            //printf("%d ", feature.at<float>(i, j));
            mean[j] += data[j];
            delta[j] += data[j]*data[j];
        }
        //printf("\n");
    }
    
    for(int i = 0; i < featureSize; i ++)
    {
        //printf("mean: %f, delta: %f\n", mean[i], delta[i]);
        mean[i] /= sampleNumber;
        delta[i] = delta[i]/sampleNumber - mean[i]*mean[i];
        delta[i] = sqrt(delta[i]);
        //printf("mean: %f, delta: %f\n", mean[i], delta[i]);
    }
    
    for(int i = 0; i < sampleNumber; i ++)
    {
        float* data = feature.ptr<float>(i);
        for(int j =0; j < featureSize; j ++)
        {
            data[j] -= mean[j];
            data[j] /= delta[j];
        }
    }
    
    delete[] mean;
    delete[] delta;
    mean = 0;
    delta = 0;
}


//level: 
//different dimension have different max and min, so is increment.
//the data is quantifyed to the nearest segment line 
void quantify(const Mat& feature, int level, Mat& quantifyFeature)
{
    if(feature.empty())
		exit(1);
	int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    
    float* min = new float[featureSize];   //
    float* max = new float[featureSize];  //
    float* diff = new float[featureSize]; //
    //row represent diffrent level, 
    //col represent different different dimension
    Mat points(featureSize, level, CV_32FC1);
    
    
    for(int i = 0; i < featureSize; i ++)
    {
        min[i] = feature.at<float>(0, i);
        max[i] = min[i];
    }
    
    for(int i = 0; i < sampleNumber; i ++)
    {
        const float* data = feature.ptr<float>(i);
        for(int j =0; j < featureSize; j ++)
        {
            if(data[j] < min[j])
                min[j] = data[j];
            if(data[j] > max[j])
                max[j] = data[j];
        }
    }
    
    for(int j =0; j < featureSize; j ++)
    	diff[j] = max[j] - min[j];
    	
    for(int i = 0; i < featureSize; i ++)
    {
        diff[i] = int(level / diff[i]);
    	for(int j = 1; j < level; j ++)
    		points.at<float>(i, j) = diff[i] * j + min[i];
    }
    
    
    if(!quantifyFeature.empty())
    	quantifyFeature.release();
    quantifyFeature.create(sampleNumber, featureSize, feature.type());
    
    for(int i = 0; i < sampleNumber; i ++)
    {
		const float* in = feature.ptr<float>(i);
        float* out = quantifyFeature.ptr<float>(i);
        for(int j = 0; j < featureSize; j ++)
        {
        		int whichLevel = (int)(in[j] - min[j]) * diff[j];
        		//int whichLevel = (int)((in[j] - min[j]) * diff[j] + 0.5);
                out[j] = points.at<float>(j, whichLevel);
        }
    }
 
    
    delete[] min;
    delete[] max;
    delete[] diff;
    min = 0;  
    max = 0;  
    diff = 0; 
}


//level: 
//regard different dimension come from same range
//and the data is quantifyed to the level ID
void quantify_Two(const Mat& feature, int level, Mat& quantifyFeature)
{
	//printf("tag1\n");
    if(feature.empty())
		exit(1);
	int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    
    float min, max;
    max = min = feature.at<float>(0, 0);


    for(int i = 0; i < sampleNumber; i ++)
    {
        const float* data = feature.ptr<float>(i);
        for(int j =0; j < featureSize; j ++)
        {
            if(data[j] < min)
                min = data[j];
            if(data[j] > max)
                max = data[j];
        }
    }
    
    
    float diff = max - min;
    diff = level / diff;

    
    if(!quantifyFeature.empty())
    	quantifyFeature.release();
    quantifyFeature.create(sampleNumber, featureSize, CV_32FC1);
    
	//printf("tag2\n");
    for(int i = 0; i < sampleNumber; i ++)
    {
		const float* in = feature.ptr<float>(i);
        float* out = quantifyFeature.ptr<float>(i);
        for(int j = 0; j < featureSize; j ++)
        {
        		out[j] = (int)((in[j] - min) * diff);
                //value = (value - int(value) <= 0.5)? value : value+0.5;
        }
    }
}



//
void overlappedImage(const Mat& frame, Mat& out)
{
    if(frame.empty())
		exit(1);
    HOGDescriptor desc;
    //desc.getDescriptors(frame, Size(8, 8), out);
}


void pcaFeature(const Mat& feature, Mat& compressFeature, int maxComponents)
{
	printf("start to enter pca ...\n");
	//int maxComponents = 100;
	//Mat w, u, vt;
	//SVD::compute(feature, w, u, vt);
	//w: min(rows, cols) * 1, descend
	//printf("eigen value: %d, %d\n", w.rows, w.cols);
	//for(int i = 0; i < w.rows; i ++)
		//printf("%f ", w.at<float>(i, 0));
	
	PCA pca(feature, Mat(), CV_PCA_DATA_AS_ROW, maxComponents);
	pca.project(feature, compressFeature);
	printf("feature cols: %d, compressed cols: %d.\n", feature.cols, compressFeature.cols);
	programPause();
}
                                                                        









