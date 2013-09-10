#ifndef _LIAN_KMEANS
#define _LIAN_KMEANS

#include "globalInclude.h"


class Kmeans
{
	public:
		Kmeans(const Mat& points, int k, string fileName = "./output/kmeans.model");
		Kmeans(string fileName = "./output/kmeans.model");
		double* collectScore();
		double* collectScore(const Mat& data);
		
	private:
		void initial(const Mat& points);
		void k_means(const Mat& feature);
		void collect();
		void save(const char* fileName);
		void load(const char* fileName);
		void updateMeasure();
		
		
	public:
		int k;
		int n, m; //n*m
		Mat centers;//CV_32FC1
		Mat indices;//indices type: CV_32SC1
		vector<int> nums;	
		double* cohency;
		double* sepration;
};


#endif
