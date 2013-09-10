#include "HogFeature.h"
#include "FileProcess.h"
#include "MatrixDataIo.h"

HogFeature::HogFeature(vector<string> files, string writer, ExtractFunType method)
{
	this->files = files;
	this->writer = writer;
	this->method = method;
}

void HogFeature::doExtrator()
{
	if(!feature.empty())
        feature.release();
   	
    if(fileExist(writer.c_str()))
    {
    	printf("tag\n");
    	string command = "rm ";
    	command += writer;
        programPause();
    	system(command.c_str());
    }
    
	directory = directoryAutoSet(files[0]);
	fileNumber = files.size();
	featureSize = GridRows * GridCols;
	
	time_t startTime = time(0);
	int readNumber = 0;

	for(int i = 0; i < fileNumber; i++)
	{
		imgStrs.clear();
		if(!initialVideo(files[i]))
		{
		    printf("!!!!!!!!!!!\n");
		    continue;
		}
        int countss = hogFeature();
        
		if(vc.isOpened())
            vc.release();
        readNumber += countss;
	}
    MatrixDataIo mio(writer.c_str(), true, feature);
	if(feature.empty())
    {
        printf("social force feature extraction process may have some errors!\n");
        exit(1);
    }
	printf("readNumber: %d, rows: %d, cols: %d\n", readNumber, feature.rows, feature.cols);
	printUsedTime(startTime);
    programPause();
}


int HogFeature::hogFeature()
{
	printf("use hog for feature ...\n"); 

    Mat frame;
    Mat current;
    vector<float> f;
    Size cellSize(16, 16);
    Size blockSize(cellSize.width*2, cellSize.height*2);
    Size blockStride(cellSize.width*1, cellSize.height*1);
    
    int windowWidth = (frameWidth-blockSize.width)/blockStride.width * blockStride.width + blockSize.width;
    int windowHeight = (frameHeight-blockSize.height)/blockStride.height*blockStride.height+blockSize.height;
    Size winSize(windowWidth, windowHeight);
    HOGDescriptor desc(winSize, blockSize, blockStride, cellSize, 9);
    
    int countss = 0;
    while(1)
    {
        if(!getFrame(frame, countss))
            break;
        countss ++;
        printf("* ");
        current = frame.clone();
        if(current.channels() > 1)
            cvtColor(current, current, CV_BGR2GRAY);
		//visualTrajectory(previous, prevPts);
        desc.compute(current, f, Size(16, 16));
        Mat r = Mat(f).t();
        //printf("%d %d\n", r.rows, r.cols);
        feature.push_back(r);
    }
    return countss;
}

//video struction initial
bool HogFeature::initialVideo(string folder)
{
    //printf("folder: %s\n", folder.c_str());
    if(directory)
    {
        getImageStrings(folder, imgStrs);
        if(imgStrs.size() == 0)
            return false;
    }
    else
    {
        vc.open(folder);
        if(!vc.isOpened())
        {
            printf("error occures: video file can't open!\n");
            return false;
        }
    }
    return true;
}


//get a image
bool HogFeature::getFrame(Mat& frame, int counts)
{
    if(directory)
    {
        if(counts >= imgStrs.size())
            return false;
        frame = imread(imgStrs[counts]);
        if(frame.empty())
        {
            printf("??????????????????\n");
            return false;
        }
    }
    else
    {
        if(!vc.read(frame))
            return false;
    }
    return true;
}