#include <iostream>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;


void help()
{
	cout << "	usage: " << endl;
	cout << "		likelyhood inFile outFile theta tagFile" << endl;
	cout << "	introduction: " << endl;
	cout << "		inFile: a log likelyhood value file" << endl;
	cout << "		outFile: the correspond abnormal state(1 or 0)" << endl;
	cout << "		theta: the correspond threthod gap" << endl;
	cout << "		tagFile: the lables" << endl;
}

void analysis(char* compareFile, bool* abandon, int n)
{
	int posN = 0, negN = 0;
	for(int i = 0; i < n; i ++)
	{
		if(abandon[i])
			posN ++;
		else
			negN ++;
	}
	printf("abandon: %d, normal: %d.\n", posN, negN);

	//
	FILE* in = fopen(compareFile, "r");
	int number;
	int tp, tn, fp, fn;
	tp = tn = fp = fn = 0;
	int actp = 0, actn = 0;
	for(int i = 0; i < n; i ++)
	{
		fscanf(in, "%d", &number);
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
	fclose(in);
	printf("actual anormal: %d, actual normal: %d\n", actp, actn);	
	printf("tp: %d, tn: %d, fp: %d, fn: %d\n", tp, tn, fp, fn);
	printf("precison: %f, recall: %f\n", tp*1.0/posN, tp*1.0/(tp+fn));
}

void distinguish(char* inName, char* outName, double theta, char* compareFile)
{		
	ifstream ifs(inName, ifstream::in);	
	if(!ifs.is_open())
		exit(1);
	ofstream ofs(outName, ofstream::out);
	if(!ofs.is_open())
		exit(1);

	bool abnormal[20000];
	double likely;
	int i = 0;
	while(!ifs.eof())
	{
		ifs >> likely;
		//printf("%lf\n", likely);
		if(likely < theta)
			abnormal[i] = true;
		else
			abnormal[i] = false;
		i++;
		if(i >= 20000)
		{
			for(int t = 0; t < 20000; t++)
			{
				if(abnormal[t])
					ofs << '1' << ' ';
				else
					ofs << '0' << ' ';
			}
			analysis(compareFile, abnormal, 20000);
			i = 0;	
		}
	}

	for(int t = 0; t < i-1; t++)
	{
		if(abnormal[t])
			ofs << '1' << ' ';
		else
			ofs << '0' << ' ';
	}
	analysis(compareFile, abnormal, i);

	ofs.close();
	ifs.close();
}


int main(int argc, char** argv)
{
	if(argc < 5)
	{
		help();
		exit(1);
	}
	distinguish(argv[1], argv[2], atof(argv[3]), argv[4]);
}