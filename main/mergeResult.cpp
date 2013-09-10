#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


using namespace std;


void help()
{
	cout << "	usage: " << endl;
	cout << "		merge operator comparefile savename filelist" << endl << endl;
	cout << "	introduction: " << endl;
	cout <<  "		func: merge result using operator" << endl << endl;
	cout <<  "		operator: or, and" << endl << endl;
	cout <<  "		comparefile: the standard result" << endl << endl;
	cout <<  "		savename: finally result" << endl << endl;
	cout <<  "		filelist: a series of file name contains 0 and 1" << endl << endl;
	
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


void readFile(char* inName, bool* state)
{
	ifstream ifs(inName, ifstream::in);
	if(!ifs.is_open())
		exit(1);
	
	int i = 0;
	int value;
	while(!ifs.eof())
	{
		ifs >> value;
		if(value == 0)
			state[i] = false;
		else
			state[i] = true;
		i++;
	}
	ifs.close();
}

void saveResult(char* outName, bool* state, int n)
{
	ofstream ofs;
	ofs.open(outName, ofstream::out);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", outName);
        exit(1);
	}
	for (int i = 0; i < n; ++i)
	{
		if(state[i])
			ofs << "1 ";
		else
			ofs << "0 ";
	}
	ofs.close();
}

void mergeTwo(bool op, bool* state1, bool* state2, int n)
{
	for (int i = 0; i < n; ++i)
	{
		if(op)
			state1[i] = state1[i] || state2[i];
		else
			state1[i] = state1[i] && state2[i];
	}
}


void merge(int argc, char** param)
{
	bool op;
	if(strcmp(param[1], "or") == 0)
		op = true;
	else if(strcmp(param[1], "and") == 0)
		op = true;
	else
	{
		help();
		exit(1);
	}

	
	char cmdline[500];
	sprintf(cmdline, "cat %s | wc -w", param[3]);
	FILE* pipe = popen(cmdline, "r");
	char buffer[100];
	fgets(buffer, sizeof(buffer), pipe);
	printf("%s\n", buffer);
	pclose(pipe);

	int n = atoi(buffer);
	bool* state1 = new bool[n];
	bool* state2 = new bool[n];
	readFile(param[4], state1);
	for (int i = 5; i < argc; ++i)
	{
		readFile(param[i], state2);
		mergeTwo(op, state1, state1, n);
		analysis(param[2], state1, n);
	}
	
	char writer[500];
	sprintf(writer, "%s", param[3]);
	saveResult(writer, state1, n);
}

int main(int argc, char** argv)
{
	if(argc < 6)
	{
		help();
		exit(1);
	}
	merge(argc, argv);
}