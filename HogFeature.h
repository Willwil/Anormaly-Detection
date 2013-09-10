#ifndef LIAN_HOG_FEATURE_H
#define LIAN_HOG_FEATURE_H

#include "globalInclude.h"
#include "ExtractFunType.h"

class HogFeature
{
	public:
		HogFeature(vector<string> files, string writer, ExtractFunType method);
		void doExtrator();

	private:
		int hogFeature();
		bool initialVideo(string folder);
		bool getFrame(Mat& frame, int counts);
	private:
		VideoCapture vc;
		vector<string> imgStrs;
		bool directory;
		int fileNumber;
		
	public:
		vector<string> files;
		string writer;
		ExtractFunType method;
		
		Mat feature;
	    int featureSize;
		int frameWidth, frameHeight;
};

#endif

