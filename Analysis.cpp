#include "Analysis.h"
#include "utils.h"

void Analysis::count(bool* abandon, int n, int& posN, int& negN)
{
	posN = 0, negN = 0;
	for(int i = 0; i < n; i ++)
	{
		if(abandon[i])
			posN ++;
		else
			negN ++;
	}
	printf("abandon: %d, normal: %d.\n", posN, negN);
}

void Analysis::compare(const char* compareFile, bool* abandon, int n)
{
	ifstream in(compareFile, ios::in);
	int number;
	int tp, tn, fp, fn;
	tp = tn = fp = fn = 0;
	int actp = 0, actn = 0;
	for(int i = 0; i < n; i ++)
	{
		in >> number;
		if(number == 1)
		{
			if(abandon[i])
				tp++;
			else
				fn++;
			actp++;
		}
		if(number == 0)
		{
			if(abandon[i])
				fp++;
			else
				tn++;
			actn++;
		}
	}
	in.close();
	double precision = tp*1.0/(tp+fp);
	double recall = tp*1.0/(tp+fn);
	double accuracy = (tp+tn)*1.0/(actp + actn);
	double f1_score = 2*precision*recall/(precision + recall);
	printf("actual anormal: %d, actual normal: %d\n", actp, actn);	
	printf("tp: %d, tn: %d, fp: %d, fn: %d\n", tp, tn, fp, fn);
	printf("precision: %lf, recall: %lf\n", precision, recall);
	printf("accuracy: %lf, f1 score: %lf\n", accuracy, f1_score);
}

void Analysis::compare(bool* compareState, bool* abandon, int n)
{
	int tp, tn, fp, fn;
	tp = tn = fp = fn = 0;
	int actp = 0, actn = 0;
	for(int i = 0; i < n; i++)
	{
		if(compareState[i])
		{
			if(abandon[i])
				tp++;
			else
				fn++;
			actp++;
		}
		else
		{
			if(abandon[i])
				fp++;
			else
				tn++;
			actn++;
		}
	}
	printf("tag\n");
	double precision = tp*1.0/(tp+fp);
	double recall = tp*1.0/(tp+fn);
	double accuracy = (tp+tn)*1.0/(actp + actn);
	double f1_score = 2*precision*recall/(precision + recall);
	printf("actual anormal: %d, actual normal: %d\n", actp, actn);	
	printf("tp: %d, tn: %d, fp: %d, fn: %d\n", tp, tn, fp, fn);
	printf("precision: %lf, recall: %lf\n", precision, recall);
	printf("accuracy: %lf, f1 score: %lf\n", accuracy, f1_score);
}

void Analysis::compare(bool* compareState, bool* abandon, int n, double& tpr, double& fpr)
{
	int tp, tn, fp, fn;
	tp = tn = fp = fn = 0;
	int actp = 0, actn = 0;
	for(int i = 0; i < n; i ++)
	{
		if(compareState[i])
		{
			if(abandon[i])
				tp++;
			else
				fn++;
			actp++;
		}
		else
		{
			if(abandon[i])
				fp++;
			else
				tn++;
			actn++;
		}
	}
	double precision = tp*1.0/(tp+fp);
	double recall = tp*1.0/(tp+fn);
	double accuracy = (tp+tn)*1.0/(actp + actn);
	double f1_score = 2*precision*recall/(precision + recall);
	printf("actual anormal: %d, actual normal: %d\n", actp, actn);	
	printf("tp: %d, tn: %d, fp: %d, fn: %d\n", tp, tn, fp, fn);
	printf("precision: %lf, recall: %lf\n", precision, recall);
	printf("accuracy: %lf, f1 score: %lf\n", accuracy, f1_score);
	tpr = tp*1.0/(tp+fn);
    fpr = fp*1.0/(fp+tn);
    printf("true positive rate: %lf, false positive rate: %lf\n\n", tpr, fpr);
}


void Analysis::analysis(const char* testFile, const char* compareFile)
{
	int n = countLines(testFile);
	bool* abandon = new bool[n];
	

	ifstream in(testFile, ios::in);
	int number;
	int i = 0;
	while(!in.eof() && i < n)
	{
		in >> number;
		//printf("number: %d\n", number);
		if(number == 0)
			abandon[i] = false;
		else
			abandon[i] = true;
		i++;
		//if(i >= 7200)
			//printf("%d\n", i);
	}
	in.close();
	
	n = i;
	compare(compareFile, abandon, n);
}

void Analysis::collect(const Mat& feature)
{
	int nl = feature.rows;
    int nc = feature.cols;
    float max, min;
    pair<int, int> s;
    maxArray(feature, max, s);
    minArray(feature, min, s);
    printf("max: %f, min: %f\n", max, min, sum(feature)[0]);
}