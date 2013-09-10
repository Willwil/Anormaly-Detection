#include "MatrixDataIo.h"
#include "DataIo.h"

MatrixDataIo::MatrixDataIo(const char* s, bool write, const Mat& data):DataIo(s, write)
{
	if(write)
	{
		this->data = data;
		this->write();
	}	
	else
		read();
}

void MatrixDataIo::read()
{
	ifstream ifs(filename, ifstream::in);
	if(!ifs.is_open())
		exit(1);
	
	int rows = countLines(filename);
	int cols = countWords(filename)/rows;
	data.create(rows, cols, CV_32FC1);
	
	for (int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; j++)
			ifs >> data.at<float>(i, j);
	}
	ifs.close();
}


void MatrixDataIo::write()
{
	ofstream ofs;
	ofs.open(filename, ofstream::out);
	if(!ofs.is_open())
	{
		errorInfo();
        exit(1);
	}
	uint nl = data.rows; 
	uint nc = data.cols; 
	for(int i = 0 ; i < nl; i++)
	{
		for(int j = 0 ; j < nc; j++)
			ofs << data.at<float>(i, j) << " ";
		ofs << endl;
	}
	ofs.close();
}

void MatrixDataIo::copy(Mat& out)
{
	out = data.clone();
}