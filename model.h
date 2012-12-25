#ifndef MY_MODEL_H
#define MY_MODEL_H

#include "globalInclude.h"
#include <vector>
#include <string>

using namespace std;

int trainSVMModel();
int predictSVM(int i = 1);
void LDAEstimation();
void trainDictionary();
void predictSparseCoeffient();

void showResult(string outputFile, int start, int end);
void shellResultParse(int status);

void cluster(Mat& feature, int k, Mat& indices, Mat& centers);
void k_means(const Mat& feature, int k, Mat& indices, Mat& centers);
void spectralClustering(Mat& feature, int k, Mat& indices, Mat& centers);


#endif // MY_MODEL_H

