#ifndef _LIAN_VISUALIZE
#define _LIAN_VISUALIZE

#include "globalInclude.h"

class Visualize
{
public:
	void static visualSorcialForce(const Mat& img, Mat pts, Mat f);
	void static visualTrajectory(const Mat& img, Mat pts);
	void static visualPointsLine(const double* x, const double* y, int n);
	void static visualFeature(const Mat feature);
	void static visualFeature(const Mat& feature, int width, int height, int delay);
};

#endif