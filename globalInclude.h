#ifndef GLOBALINCLUDE_H
#define GLOBALINCLUDE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;
using namespace cv;

extern int frameWidth;
extern int frameHeight;
const int FeatureRows = 10;
const int FeatureCols = 10;
const int MaxStringLength = 500;

enum ExtractFunType
{
    OPTICAL,
    SORCIALFORCE,
    MOTIONHISTOM,
    MYNEW
};

enum DataSet
{
    UMN,
    BEHAVE,
    UCSD_PED1,
    UCSD_PED2
};

enum ClusterMethod
{
	K_Means,
	Spectral_Cluster
};

bool isVideoFormat(string s);                                             
bool isImageFormat(string s);
bool directoryAutoSet(string s);


#endif // GLOBALINCLUDE_H
