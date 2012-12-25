#ifndef _FEATURE_IO_H
#define _FEATURE_IO_H

bool writeInSVM(string writer, string reader, vector<int> label);
bool writeInNormal(string writer);
bool writeFeatureToFile(const Mat& feature, string writer);
double* readLdaResult(string fileName);

#endif
