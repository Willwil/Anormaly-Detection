#ifndef _LIAN_ANALYSIS
#define _LIAN_ANALYSIS

#include "globalInclude.h"

class Analysis
{
public:
	static void count(bool* abandon, int n, int& posN, int& negN);
	static void compare(const char* compareFile, bool* abandon, int n);
	static void compare(bool* compareState, bool* abandon, int n);
	static void compare(bool* compareState, bool* abandon, int n, double& tpr, double& fpr);
	static void analysis(const char* testFile, const char* compareFile);
	static void collect(const Mat& feature);
};

#endif
