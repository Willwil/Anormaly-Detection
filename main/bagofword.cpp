#include <fstream>
#include <iostream>
#include "Kmeans.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;


void help()
{
	cout << "	usage: " << endl;
	cout << "		bagword inputfilename savename vocabNumber segment pieces" << endl << endl;
	cout << "	introduction: " << endl;
	cout <<  "		func: cal the bag-of-word feature and save it" << endl << endl;
	cout <<  "		inputfilename: orginal feature" << endl << endl;
	cout <<  "		savename: corpus feature" << endl << endl;
	cout <<  "		segment: the feature number in a document" << endl << endl;
	cout <<  "		pieces: partion feature dimension into pieces" << endl << endl;
}

void readFeature(char* inName, Mat& feature)
{
	ifstream ifs(inName, ifstream::in);
	if(!ifs.is_open())
		exit(1);
	
	string buffer;
	getline(ifs, buffer);
	//printf("buffer: %s\n", buffer.c_str());
	int bs = buffer.size();
	int i = bs -1;
	int j = 0;
	//get rid of the space charactor at end
	for(; i >= 0; i--)
	{
		if(buffer[i] != ' ')
			break;
	}
	//get rid of the space charactor at start
	for(; j < i; j++)
	{
		if(buffer[j] != ' ')
			break;
	}
	int cols = 1;
	for(int k = j; k <= i; k ++)
	{
		if(buffer[k] == ' ')
			cols ++;
	}
	
	
	Mat t;
	t.create(1, cols, CV_32FC1);
	float* data = (float*)t.data;
	int rows = 0;
	ifs.seekg(0, ios::beg);
	while(!ifs.eof())
	{
		for(i = 0; i < cols; i++)
			ifs >> data[i];
		feature.push_back(t);
		rows++;
	}
	ifs.close();
	printf("rows: %d, cols: %d\n", rows, cols);	
}

void saveCorpus(char* outName, const Mat& indices, int vocab, int segment)
{
	ofstream ofs;
	ofs.open(outName, ofstream::out);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", outName);
        exit(1);
	}
	uint nl = indices.rows; 
	int* count = new int[vocab];
	for(int t = 0; t < vocab; t++)
		count[t] = 0;
	int terms;
	for(int i =0 ; i < nl; i ++)
	{
		count[indices.at<int>(i, 0)]++;
		if((i+1) % segment == 0)
		{
			//no zero term
			terms=0;
			for(int t = 0; t < vocab; t++)
			{
				if(count[t] != 0)
					terms++;
			}
			//putput
			ofs << terms;
			for(int t = 0; t < vocab; t++)
			{
				if(count[t] != 0)
					ofs << " " << t << ":" << count[t];
			}
			ofs << endl;
			///clear
			for(int t = 0; t < vocab; t++)
				count[t] = 0;
		}
	}
	ofs.close();
}

void bagwords(char* inName, char* outName, int vocab, int segment, int pieces)
{
	Mat feature;
	readFeature(inName, feature);
	feature = feature.rowRange(0, feature.rows-1);
	int part = feature.cols / pieces;
	int start = 0;
	int end = start + part;
	Mat document;
	for (int i = 1; i <= pieces; ++i)
	{
		document.push_back(feature.colRange(start, end));
		start = end;
		end = start + part;
	}
	Kmeans km(document, vocab);
	for (int i = 1; i <= vocab; ++i)
	{
		printf("%d\n", km.nums[i-1]);
	}
	char writer[500];
	sprintf(writer, "%s", outName);
	saveCorpus(writer, km.indices, vocab, segment);
	// for (int i = 1; i <= pieces; ++i)
	// {
	// 	char writer[500];
	// 	sprintf(writer, "%s.%d", outName, i);
	// 	saveCorpus(writer, km.indices, vocab, segment);
	// }
}

int main(int argc, char** argv)
{
	if(argc < 6)
	{
		help();
		exit(1);
	}
	bagwords(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
}