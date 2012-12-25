#ifndef MY_FEATURES_H
#define MY_FEATURES_H

#include "globalInclude.h"

void opticalFeature(Mat& feature, vector<string> files, string writer);
void socialForceFeature(Mat& feature, vector<string> files, string writer);
void gradientHistomFeature(Mat& feature, vector<string> files, string writer);
void myNewFeature(Mat& feature, vector<string> files, string writer);
void gridImage(const Mat& src, Mat& dest, int rows, int cols);
void setGrids(Mat& pts);
void getImageStrings(string foldName, vector<string>& imgStrs);
bool initialVideo(string folder, bool directory);
bool getFrame(int counts, bool directory, Mat& frame);
void getAverageMat(const Mat& in, Mat& out);
Mat readFeatureFromFile(string filename);

#endif // FEATURES_H
