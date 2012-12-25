#include "detectAlg.h"
#include "feature.h"


Mat& featureExtrator(Mat& feature, vector<string> files, string writer)
{
    printf("start to extract features...\n");
    enum ExtractFunType featureFunction = SORCIALFORCE;
    switch(featureFunction)
    {
        case OPTICAL:
            opticalFeature(feature, files, writer);
            break;
        case SORCIALFORCE:
            socialForceFeature(feature, files, writer);
            break;
        case MOTIONHISTOM:
            gradientHistomFeature(feature, files, writer);
            break;
        case MYNEW:
            myNewFeature(feature, files, writer);
            break;
        default:
            break;
    }
    printf("end of extract feature.\n");
    return feature;
   // normalizeFeature(feature, type);
   /* vector<int> labels;
    for(int i = 0; i < feature.rows; i ++)
        labels.push_back(1);   //正样本
    if(!type && startFrame > 0 && endFrame > 0)
    {
        if(endFrame > labels.size())
        {
            printf("lable size error !\n");
            exit(1);
        }
        startFrame = max(1, startFrame);
        for(int i = startFrame; i <= endFrame; i ++)
            labels[i-1] = -1;      //负样本
    }
    //format to standard libsvm format
    if(!writeFeatureToFile(feature, labels, type))
    {
        printf("error occures: failed to write feature to files.\n");
        exit(1);
    }*/
   
}


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
    min = 0;
    delete[] max;
    max = 0;
}

//mean:0, delta=1;
void normalizeFeature2(Mat& feature)
{
	cout << feature << endl;
    int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    double* mean = new double[featureSize];   //方差
    double* delta = new double[featureSize];  //标准差
    
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


//level: 量化级数
void quantify(Mat& feature, int level)
{
    if(feature.empty())
		exit(1);
	int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    float* min = new float[featureSize];   //
    float* max = new float[featureSize];  //
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
    
    float* increment = max;
    float** points = new float*[featureSize];
    for(int i = 0; i < featureSize; i ++)
    {
        increment[i] = int((max[i] - min[i])/level);
        points[i] = new float[level];
        	for(int j = 1; j < level; j ++)
        		points[i][j] = increment[i]*j + min[i];
    }
    
    float value;
    for(int i = 1; i < sampleNumber; i ++)
    {
        float* data = feature.ptr<float>(i);
        for(int j = 0; j < featureSize; j ++)
        {
        		value = (data[j] - min[j])/increment[j];
                value = (value - int(value) <= 0.5)? value : value+0.5;
                data[j] = points[j][int(value)];
        }
    }
}

/*void overlappedImage(const Mat& frame, vectorint cellWidth=8, int cellHeight=8, int bins=9, )
{
	if(featue.empty())
		exit(1);
	int blockWidth = cellWidth * 2;
    int blockHeight = cellHeight * 2;
    
	int featureSize = feature.cols;
    int sampleNumber = feature.rows;
    

}*/

//
void overlappedImage(const Mat& frame, Mat& out)
{
    if(frame.empty())
		exit(1);
    HOGDescriptor desc;
    //desc.getDescriptors(frame, Size(8, 8), out);
}
                                                                         










