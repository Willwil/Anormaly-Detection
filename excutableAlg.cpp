#include "excutableAlg.h"

#include "FeatureExtractor.h"
#include "HogFeature.h"
#include "featureProcess.h"
#include "ExtractFunType.h"
#include "cluster.h"
#include "outlier.h"
#include "utils.h"
#include "Kmeans.h"
#include "Analysis.h"
#include "MatrixDataIo.h"
#include "BoolArrayDataIo.h"
#include "Visualize.h"


//#define LOAD_PROCESS_DATA
#define LOAD_FEATURE
//#define ANALYSIS_DENSITY

void train(vector<string>& file)
{
	int files = file.size();
	bool load_feature = false;
	if(files == 0)
		load_feature = true;
	bool load_process = false;
	if(load_feature)
	{
		#ifdef LOAD_PROCESS_DATA
			load_process = true;
		#endif
	}
	
	Mat feature;
	if(load_process)
	{
		printf("reading compressed feature ...\n");
		string reader = "./output/compress.dat";
		MatrixDataIo mio(reader.c_str());
		feature = mio.data;
	}
	else 
	{
		if(load_feature)
		{
			printf("reading feature ...\n");
			string reader = "./output/trainFeature.dat";
			MatrixDataIo mio(reader.c_str());
			feature = mio.data;
		}
		else
		{
			string writer = "./output/trainFeature.dat";
			ExtractFunType ef = (ExtractFunType)SORCIALFORCE;
			FeatureExtractor fe(file, writer.c_str(), ef);
			fe.frameWidth = frameWidth;
			fe.frameHeight = frameHeight;
			fe.doExtrator();
			feature = fe.feature;
		}
		printf("processing feature ...\n");
		string writer = "./output/trainCompress.dat";
		laterProcess(feature, writer, true);
	}

	int k = 200;
	Kmeans km(feature, k);
}

void predict(vector<string>& file)
{
	int files = file.size();
	bool load_feature = false;
	if(files == 0)
		load_feature = true;
	bool load_process = false;
	if(load_feature)
	{
		#ifdef LOAD_PROCESS_DATA
			load_process = true;
		#endif
	}
	
	Mat feature;
	if(load_process)
	{
		printf("reading compressed feature ...\n");
		MatrixDataIo mio("./output/compress.dat");
		feature = mio.data;
	}
	else 
	{
		if(load_feature)
		{
			printf("reading feature ...\n");
			string reader = "./output/testFeature.dat";
			MatrixDataIo mio(reader.c_str());
			feature = mio.data;
		}
		else
		{
			string writer = "./output/testFeature.dat";
			ExtractFunType ef = (ExtractFunType)SORCIALFORCE;
			FeatureExtractor fe(file, writer, ef);
			fe.frameWidth = frameWidth;
			fe.frameHeight = frameHeight;
			fe.doExtrator();
			feature = fe.feature;
		}
		printf("processing feature ...\n");
		string writer = "./output/testCompress.dat";
		laterProcess(feature, writer, false);
	}
	// BoolArrayDataIo bio("./output/result.out");
	// printf("loading compare file ...\n");
	// BoolArrayDataIo bio2("./dataset/label_test_uscd1.dat");
	// Analysis::compare(bio2.data, bio.data, bio.n);
}

void laterProcess(Mat& data, string writer, bool train)
{
	//PCA reduce dimension
	printf("reduce feature dimension ...\n");
	if(train)
	{
		double retainedVariance = 0.95;
		PCA pca(data, noArray(), CV_PCA_DATA_AS_ROW, retainedVariance);
		MatrixDataIo mio("./output/pca.dat", true, pca.eigenvectors);
		printf("%d %d\n", pca.eigenvectors.rows, pca.eigenvectors.cols);
		pca.project(data, data);
	}
	else
	{
		MatrixDataIo mio("./output/pca.dat");
		//data = data * mio.data.t();
		Mat pcaMat = mio.data.t() * data;
		Mat constructMat = pcaMat * mio.data;
		method_pca(data, pcaMat);
	}
	MatrixDataIo mio(writer.c_str(), true, data);
}

void method_pca(Mat& orginalMat, Mat& pcaMat)
{
	float* cost = l2norm(orginalMat-pcaMat);
	int n = orginalMat.rows;
	int m = orginalMat.cols;
	double* score = new double[n];
	for (int i = 0; i < n; ++i)
	{
		score[i] = sqrt(m/cost[i]);
	}
	
	bool* anormal = decideAnormal(score, sum(score, n)/n *0.9, n);
	resultProcess(anormal, n, "./output/result.out", "./dataset/label_test_uscd1.dat");
	//computeAUC(score, n, "./dataset/label_test_uscd1.dat");
	delete[] anormal;
	anormal = 0;
	delete[] score;
	score = 0;
}


void resultProcess(bool* abandon, int n, string writer, string reader)
{
	BoolArrayDataIo bio(writer.c_str(), true, abandon, n);
	int posN = 0, negN = 0;
	Analysis::count(abandon, n, posN, negN);
	printf("loading compare file ...\n");
	BoolArrayDataIo bio2(reader.c_str());
	if(bio2.n != n)
	{
		printf("the compare size is not compatible to result.\n");
		printf("%d, %d\n", bio2.n, n);
		exit(1);
	}
	printf("comparing ...\n");
	Analysis::compare(bio2.data, abandon, n);
}

void computeAUC(double* score, int n, string reader)
{
	BoolArrayDataIo bio2(reader.c_str());
	int repeat = 30;
	double threshod = 0.0;
	double theta = 0.3;
	double* tpr = new double[repeat];
	double* fpr = new double[repeat];
	double auc = 0;
	int posN, negN;
	for(int t = 0; t < repeat; t++)
	{
		threshod += theta;
		bool* anormal = decideAnormal(score, threshod, n);
		Analysis::count(anormal, n, posN, negN);
		Analysis::compare(bio2.data, anormal, n, tpr[t], fpr[t]);
		if(t > 0)
			auc += 0.5*(tpr[t-1] + tpr[t])*(fpr[t] - fpr[t-1]);
		delete[] anormal;
		anormal = 0;
	}
	printf("area under curve: %lf\n", auc);
	Visualize::visualPointsLine(fpr, tpr, repeat);
	delete[] tpr;
	tpr = 0;
	delete[] fpr;
	fpr = 0;
}

