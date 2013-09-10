#ifndef _LIAN_EXCUTABLE_ALG
#define _LIAN_EXCUTABLE_ALG

#include "globalInclude.h"


void train(vector<string>& file);
void predict(vector<string>& file);

void laterProcess(Mat& feature, string writer, bool train);
void method_pca(Mat& orginalMat, Mat& pcaMat);
void resultProcess(bool* abandon, int n, string writer, string reader);
void computeAUC(double* score, int n, string reader);

#endif

