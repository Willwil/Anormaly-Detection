#ifndef _EXCUTABLE_ALG
#define _EXCUTABLE_ALG

#include "gloableInclude.h"

void Alg_SVM(vector<string>& trainFiles, vector<string>& testFiles, vector<int> labels);
void Alg_LDA(vector<string>& file);
void Alg_Sparse(vector<string>& file);
void Alg_GMM(vector<string>& file);
void Alg_MeanShift(vector<string>& file);

#endif

