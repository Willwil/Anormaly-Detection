#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;


void help()
{
	cout << "	usage: " << endl;
	cout << "		analysis testFile compareFile" << endl;
	cout << "	introduction: " << endl;
	cout << "		testFile: experiment file" << endl;
	cout << "		compareFile: the actual correspond abnormal state(1 or 0)" << endl;
}

void count(bool* abandon, int n, int& posN, int& negN)
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

void compare(char* compareFile, bool* abandon, int n)
{
	int posN, negN;
	count(abandon, n, posN, negN);
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
	printf("actual anormal: %d, actual normal: %d\n\n", actp, actn);	
	printf("tp: %d, tn: %d, fp: %d, fn: %d\n", tp, tn, fp, fn);
	printf("precison: %f, recall: %f\n", tp*1.0/posN, tp*1.0/(tp+fn));
}


void analysis(char* testFile, char* compareFile)
{
	char cmdline[500];
	sprintf(cmdline, "cat %s | wc -w", testFile);
	FILE* pipe = popen(cmdline, "r");
	char buffer[100];
	fgets(buffer, sizeof(buffer), pipe);
	printf("%s\n", buffer);
	pclose(pipe);

	int n = atoi(buffer);
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



int main(int argc, char** argv)
{
	if(argc < 3)
	{
		help();
		exit(1);
	}
	analysis(argv[1], argv[2]);
	return 0;
}