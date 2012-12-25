#ifndef DETECTALG_H
#define DETECTALG_H

#include "globalInclude.h"

//void featureExtrator(vector<string> files, bool type, int startFrame, int endFrame);
Mat& featureExtrator(Mat& feature, vector<string> files, string writer);
void gridImage(const Mat& src, Mat& dest, int rows, int cols);
//bool writeFeatureToFile(const Mat& feature, const vector<int>& label, bool type);
void normalizeFeature1(Mat& feature, bool type);
void normalizeFeature2(Mat& feature);
void quantify(Mat& feature, int level);
void overlappedImage(const Mat& frame, Mat& out);


#endif // DETECTALG_H
