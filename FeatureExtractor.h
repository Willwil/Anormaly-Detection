#ifndef LIAN_FEATURE_EXTRACTOR_H
#define LIAN_FEATURE_EXTRACTOR_H

#include "globalInclude.h"
#include "ExtractFunType.h"


class FeatureExtractor
{
	public:
		FeatureExtractor(vector<string> files, string writer, ExtractFunType method);
		void doExtrator();
		int socialForceFeature();
		int myNewFeature();
		
	private:
		bool initialVideo(string folder);
		bool getFrame(Mat& frame, int counts);
		
		void setScales(int scales[], int n, int m);
		void setGrids(Mat& pts, int n, int m);	
		void getAverageMat(const Mat& in, const Mat& pts, Mat& out);
		void initialParticle(const Mat& frame, const Mat& previous, Mat& pts);

		int areaFromIntegral(const Mat& sum, int x1, int y1, int x2, int y2)
		{
			//assert(x2 >= x1 && y2 >= y1);
			int t1 = sum.at<int>(x2, y2)/255;
			int t2 = sum.at<int>(x1, y1)/255;
			int t3 = sum.at<int>(x1, y2)/255;
			int t4 = sum.at<int>(x2, y1)/255;
			int area = t1 - t4 - t3 + t2;
			area = area;
			//printf("-- %d\n", area);
			if(area < 0)
			{
				printf("%d, %d, %d, %d\n", t1, t2, t3, t4);
				printf("%d, %d, %d, %d\n", x1, x2, y1, y2);
				programPause();
			}
				
			return area;
		}
		
	private:
		VideoCapture vc;
		vector<string> imgStrs;
		bool directory;
		int fileNumber;
		int scales[36];
		
	public:
		vector<string> files;
		string writer;
		ExtractFunType method;
		
		Mat feature;
	    int featureSize;
		int restartTime;
		int frameWidth, frameHeight;
};


#endif // FEATURES_H
