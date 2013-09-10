#ifndef _LIAN_MATRIXDATA_IO
#define _LIAN_MATRIXDATA_IO

#include "DataIo.h"
#include "globalInclude.h"

class MatrixDataIo: DataIo
{
public:
	MatrixDataIo(const char* s, bool write=false, const Mat& data=Mat());
	void copy(Mat& out);
protected:
	void read();
	void write();	

public:
	Mat data;
};

#endif