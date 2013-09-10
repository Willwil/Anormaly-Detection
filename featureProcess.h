#ifndef LIAN_FEATURE_PROCESS_H
#define LIAN_FEATURE_PROCESS_H

#include "globalInclude.h"

void normalizeFeature1(Mat& feature, bool type);
void normalizeFeature2(Mat& feature);
void quantify(Mat& feature, int level, Mat& quantifyFeature);
void quantify_Two(const Mat& feature, int level, Mat& quantifyFeature);
void overlappedImage(const Mat& frame, Mat& out);
void pcaFeature(const Mat& feature, Mat& compressFeature, int maxComponents);

#endif // DETECTALG_H
